#include "ch.h"
#include "hal.h"

#include "halcanio.h"
#include "hal_ad5593r.h"
#include "j1939_hy.h"
#include "j1939_config.h"
#include "i2c_eeprom.h"

#define GPT_BASE_CLOCK 18000000
#define J1939_TIMING_ADJ        -100
#define GPT_INTERVAL 1800+J1939_TIMING_ADJ       // 10KHz

#define EEP_WRITE_BASE 0x0
#define EEP_BUFF_SIZE   16

static uint8_t eeBuff[EEP_BUFF_SIZE];
static uint8_t eepModified = 0;

static I2CConfig i2cCfg = {
  OPMODE_I2C,
  100000,
  STD_DUTY_CYCLE,
};

typedef struct pgn_callback_s{
  uint32_t pgnNumber;
  cbRecvMessage fcn;
  struct pgn_callback_s *next;   
}pgn_callback_t;


typedef struct msg_list_s{
  j1939_msg_t *msg;
  struct msg_list_s *next;
}msg_list_t;

typedef struct periodic_callback_s{
  uint32_t start;
  uint32_t interval;
  uint32_t iters;
  cbFunction fcn;
  struct periodic_callback_s *next;
}periodic_callback_t;
  

pgn_callback_t *allocatePgnCallback(void);
msg_list_t *allocateMsgList(void);
periodic_callback_t *allocatePeriodicCallback(void);
static void gptCallBack(GPTDriver *gptp);
static uint8_t canSend(j1939_msg_t *m);

// threads parameters
#define WA_SIZE_256 THD_WORKING_AREA_SIZE(256)

static THD_WORKING_AREA(waJ1939MainThd,WA_SIZE_256);
//static tfunc_t J1939MainThread(void *arg);
static THD_FUNCTION(J1939MainThread,p);
static thread_t *mainThread;


static THD_WORKING_AREA(waJ1939TimeThd,WA_SIZE_256);
//static tfunc_t J1939TimedThread(void *arg);
static THD_FUNCTION(J1939TimedThread,p);
static thread_t *timedThread;


static THD_WORKING_AREA(waCANRecvThd,WA_SIZE_256);
//static tfunc_t CANRecvThread(void *arg);
static THD_FUNCTION(CANRecvThread,p);
static thread_t *recvThread;

pgn_callback_t *pgnCbFcns;
msg_list_t *msgList;
periodic_callback_t *userCbFcns;
cbRecvMessage messageRecvCb;

// can config for 500K baud
/*
static CANConfig canConfig = {
  0,
  CAN_BTR_BRP(7) | CAN_BTR_TS1(4) | CAN_BTR_TS2(2) | CAN_BTR_SJW(0)
};
*/
// can config for 250K baud
static CANConfig canConfig = {
  0,
  CAN_BTR_BRP(8) | CAN_BTR_TS1(11) | CAN_BTR_TS2(2) | CAN_BTR_SJW(0)
};

static GPTConfig gptcfg={
  GPT_BASE_CLOCK,
  gptCallBack
};
/*
  J1939 Program Interface


*/

/*
Function: j1939Init
Description: initial can peripheral and process threads
*/

static THD_FUNCTION(thread,p){
  
}

void j1939Init(void)
{
  // initial chibios 
  halInit();
  halJ1939Init();
  halCANLedOff();
  chSysInit();
  // CAN TX/RX GPIO init
  //palSetPadMode(GPIOA,11,PAL_MODE_INPUT_PULLUP);
  //palSetPadMode(GPIOA,12,PAL_MODE_STM32_ALTERNATE_PUSHPULL);

  canInit();
  canStart(&CAND1,&canConfig);
  pgnCbFcns = allocatePgnCallback();
  userCbFcns = allocatePeriodicCallback();
  gptStart(&GPTD1,&gptcfg);
  messageRecvCb = 0;
  
  // start I2C
  //i2cStop(&I2CD1);
  //i2cInit();
  i2cStart(&I2CD1,&i2cCfg);
  // start digital & analog peripheral
  //halJ1939Init();
  ad5593rInit(&I2CD1);
  eepRead32(EEP_WRITE_BASE,EEP_BUFF_SIZE,eeBuff);
//  eepTest();
  
  // create working threads
  mainThread = chThdCreateStatic(waJ1939MainThd,
                                 WA_SIZE_256,
                                 NORMALPRIO,
                                 J1939MainThread,
                                 0);
  
  timedThread = chThdCreateStatic(waJ1939TimeThd,
                                 WA_SIZE_256,
                                 NORMALPRIO,
                                 J1939TimedThread,
                                 0);
  recvThread = chThdCreateStatic(waCANRecvThd,
                                 WA_SIZE_256,
                                 NORMALPRIO,
                                 CANRecvThread,
                                 0);
  gptStartContinuous(&GPTD1,GPT_INTERVAL);
  /*
  while(1){
    chThdSleepMilliseconds(10);
  }
  */
}

static THD_FUNCTION(J1939MainThread,p)
//static tfunc_t J1939MainThread(void *arg)
{
  
  while(true){
    chThdSleepMilliseconds(10);
  }
}

static THD_FUNCTION(J1939TimedThread,p)
//static tfunc_t J1939TimedThread(void *arg)
{

  uint32_t reload = 0;
  chRegSetThreadName("TimedThread");
  periodic_callback_t *cb;
  
  while(true){
    chEvtWaitAny((eventmask_t)1);
    //j1939PeriodicCbFcn();
    cb = userCbFcns;
    
    while(cb){
      if(cb->iters == 0 && cb->fcn){
        cb->fcn();
        //halCANLedToggle();
      }
      cb->iters++;
      if(cb->iters == cb->interval) cb->iters = 0;
      cb = cb->next;  
    }
    
    if(reload == 0){
      ad5593rADCScan();
    }
    reload++;
    if(reload == ADC_SCAN_RATE) reload = 0;
    
    if(eepModified == 1){
      eepModified = 0;
      eepWrite32(EEP_WRITE_BASE,EEP_BUFF_SIZE,eeBuff);
    }
  }
}

static THD_FUNCTION(CANRecvThread,p)
//static tfunc_t CANRecvThread(void *arg)
{
  
  chRegSetThreadName("CANReceiveThread");
  CANRxFrame rxp;
  event_listener_t canRecvListener;
  //flagsmask_t flags;
  eventmask_t me;
  msg_t ret;
  uint8_t i;
  j1939_msg_t m;
  (void)p;
  chEvtRegisterMask(&CAND1.rxfull_event,
                &canRecvListener,
                0);
  pgn_callback_t *cb;
  while(true){
    chEvtWaitOne(EVENT_MASK(1));
    //flags = chEvtGetAndClearFlags(&canRecvListener);
    while(canReceive(&CAND1,CAN_ANY_MAILBOX,&rxp,TIME_IMMEDIATE) == MSG_OK){
      m.pgn = (rxp.EID >> 8) & 0x1ffff;
      m.src = (rxp.EID & 0xff);
      m.bufLen = rxp.DLC;
      
      for(i=0;i<rxp.DLC;i++)
        m.buf[i] = rxp.data8[i];
      
      // find whether an callback function registed
      cb = pgnCbFcns;
      i = 0;
      while(cb){
        if(cb->pgnNumber == m.pgn){
          i = 1;
          break;
        }
        cb = cb->next;
      }
      if(i == 1){
        if(cb->fcn) cb->fcn(&m);
      }
      else{
        // call user function for processing
        if(messageRecvCb) messageRecvCb(&m);
  //      j1939Process(&m);
      }      
    }
  }
}


uint8_t j1939Transmit(j1939_msg_t *msg)
{
  // push msg into queue or not?
  
  return canSend(msg);
}
uint8_t j1939PGNCbRegister(uint32_t pgnNumber,cbRecvMessage f)
{
  pgn_callback_t *pgncb = pgnCbFcns;
  if(pgnNumber == 0) return 0;
  
  if(pgncb->pgnNumber != 0){
    while(pgncb->next != 0) pgncb = pgncb->next;
    
    pgncb->next = allocatePgnCallback();
//    if(pgncb->next == 0) return 0;
    pgncb = pgncb->next;
  }
  
  if(pgncb != 0){
    pgncb->fcn = f;
    pgncb->pgnNumber = pgnNumber;
    return 1;
  }
  return 0;
}
uint8_t j1939PeriodicCbRegister(uint32_t start, uint32_t interval, cbFunction f)
{
  periodic_callback_t *cb = userCbFcns;
  if(interval == 0) return 0;
  if(f == 0) return 0;
  if(cb->interval != 0){
    while(cb->next) cb = cb->next;
    cb->next = allocatePeriodicCallback();
    cb = cb->next;
  }
  if(cb){
    cb->fcn = f;
    cb->interval = interval;
    cb->start = 0;
    return 1;
  }
  return 0;
}

void 
j1939RegisterMessageCb(cbRecvMessage f)
{
  messageRecvCb = f;
}

void j1939Process(){
  chThdSleepMilliseconds(10);
}

static uint8_t canSend(j1939_msg_t *m)
{
  CANTxFrame ptx;
  uint8_t i;
  
  ptx.EID = (m->prio << 26) | (m->pgn<<8) | m->src;
  /*
  ptx.EID = m->prio;
  ptx.EID <<= 8;
  ptx.EID |= m->pgn;
  ptx.EID <<= 8;
  ptx.EID |= m->src;
  */
  ptx.IDE = CAN_IDE_EXT;
  ptx.RTR = CAN_RTR_DATA;
  ptx.DLC = m->bufLen;
  
  for(i=0;i<m->bufLen;i++) 
    ptx.data8[i] = m->buf[i];
  
  if(canTransmit(&CAND1,0,&ptx,0) == MSG_OK){
    return 0xff;
  }
  else{
    return 0x00;
  }
  
  
  return 0;
}

pgn_callback_t *allocatePgnCallback(void)
{
  pgn_callback_t *pgn;
  pgn = (pgn_callback_t*)malloc(sizeof(pgn_callback_t));
  if(pgn){
    pgn->fcn = 0;
    pgn->pgnNumber = 0;
    pgn->next = 0;
    return pgn;
  }
  return 0;
}
msg_list_t *allocateMsgList(void)
{
  msg_list_t *f;
  f = (msg_list_t*)malloc(sizeof(msg_list_t));
  if(f){
    f->msg = 0;
    f->next = 0;
    return f;
  }
  return 0;
}

periodic_callback_t *allocatePeriodicCallback(void)
{
  periodic_callback_t *f;
  f = (periodic_callback_t*)malloc(sizeof(periodic_callback_t));
  if(f){
    f->start = f->interval = 0;
    f->iters = 0;
    f->fcn = 0;
    f->next = 0;
    return f;
  }
  return 0;
}

static void gptCallBack(GPTDriver *gptp)
{
  chSysLockFromISR();
  chEvtSignalI(timedThread,(eventmask_t)1);
  chSysUnlockFromISR();
}

// helper functions
uint8_t mapBitValue(uint8_t in, uint8_t offset)
{
  return (in << offset);
}

uint8_t bToU8(uint8_t *b)
{
  return (uint8_t)(*b);
}

uint16_t bToU16(uint8_t *b)
{
  uint16_t v;
  v = *b++;
  v <<= 8;
  v |= *b;
  return v;
}


uint32_t bToU32(uint8_t *b)
{
  uint32_t v;
  v = *b++;
  v <<= 8;

  v |= *b++;
  v <<= 8;

  v |= *b++;
  v <<= 8;

  v |= *b++;
  return v;
}

int8_t bToI8(uint8_t *b)
{
  int8_t v;
  v = *b;
  return v;
}

int16_t bToI16(uint8_t *b)
{
  int16_t v;
  v = *b++;
  v <<= 8;
  v |= *b;
  return v;
}

int32_t bToI32(uint8_t *b)
{
  int32_t v;
  v = *b++;
  v <<= 8;
  v |= *b++;
  v <<= 8;
  v |= *b++;
  v <<= 8;
  v |= *b++;
  return v;
}

void bFromU16(uint8_t *b, uint16_t v)
{
  *b++ = (v >> 8);
  *b = (v & 0xff);
}

void bFromU32(uint8_t *b, uint32_t v)
{
  *b++ = (v >> 24) & 0xff;
  *b++ = (v >> 16) & 0xff;
  *b++ = (v >> 8) & 0xff;
  *b = (v & 0xff);
}

void bFromI16(uint8_t *b, int16_t v)
{
  *b++ = (v >> 8);
  *b = (v & 0xff);
}

void bFromI32(uint8_t *b, int32_t v)
{
  *b++ = (v >> 24) & 0xff;
  *b++ = (v >> 16) & 0xff;
  *b++ = (v >> 8) & 0xff;
  *b = (v & 0xff);
}

/*
  analog/digital i/o access helper apis
*/
float j1939ReadVoltage(uint8_t ch)
{
#ifdef USE_MASTER
  return (ad5593rReadChFloat(ch));
#else
  return 0;
#endif
}

uint16_t j1939ReadRaw(uint8_t ch)
{
#ifdef USE_MASTER
  return (ad5593rReadChRaw(ch));
#else
  return 0;
#endif
}

uint8_t j1939ReadPin(uint8_t pin)
{
  uint8_t retv;
#ifdef USE_MASTER
  retv = halCANMasterReadInput(pin);
#elif USE_SLAVE
  retv = halCANSlaveReadInput(pin);
#else
  retv = 0;
#endif
  return retv;
}

uint32_t j1939ReadAllPins()
{
  uint32_t retv;
#ifdef USE_MASTER
  retv = halCANMasterReadInputs();
#elif USE_SLAVE
  retv = halCANSlaveReadInputs();
#else
  retv = 0;
#endif
  return retv;
}

//uint8_t j1939MemRead(uint16_t addr, uint8_t len, uint8_t *data)
uint8_t j1939MemRead(uint8_t addr, uint8_t *data)
{
  //eepRead32(addr,len,data);
  if(addr < EEP_BUFF_SIZE){
    *data = eeBuff[addr];
    return 1;
  }
  return 0;
}

uint8_t j1939MemWrite(uint16_t addr, uint8_t data)
{
  if(addr < EEP_BUFF_SIZE){
    eeBuff[addr] = data;
    eepModified = 1;
    return 1;
  }
  return 0;
}

void j1939GLED_ON()
{
  halCANLedOn();
}
void j1939GLED_OFF()
{
  halCANLedOff();
}
void j1939GLED_TOG()
{
  halCANLedToggle();
}


