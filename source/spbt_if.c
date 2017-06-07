#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "spbt_if.h"

typedef struct cmd_sequency_s{
  char cmd[64];
  char resp[64];
  int8_t responsed;
}cmd_sequency_t;

cmd_sequency_t cmdSeq[] = {
  {"AT-AB ChangeBaud 2000000\r\n","AT-AB ChangeBaud 2000000\r\n",0},
};

static SerialConfig serCfg_ls = 
{
  115200,       // default speed  
};

static SerialConfig serCfg_fs = 
{
  2000000,      // 2M
  0,
  0,
  (1 << 9) |    // CTS enable
  (1 << 8)      // RTS enable
};

static bool cmdResponsed;
thread_t *thdspbt;

static void cmd_view(BaseSequentialStream *chp, int argc, char *argv[])
{
  
}

static void cmd_atResp(BaseSequentialStream *chp, int argc, char *argv[])
{
  if(argc > 0){
    if(strcmp("AutoReconneceDone",argv[0]) == 0){
      if(strcmp("Enabled",argv[1]) == 0){
        cmdResponsed = true;
      }
    }else if(strcmp("Baudrate",argv[0]) == 0){
      // change baudrate
      
    }
    
  }
}

static void cmd_setLocalName(BaseSequentialStream *chp, int argc, char *argv[])
{

}
static const ShellCommand inertial_cmds[] = {
  {"view",cmd_view},
  {"AT-AB",cmd_atResp},
  {"SETNAME",cmd_setLocalName},
};

static const ShellConfig shell_cfg = {
  (BaseSequentialStream*)&SD1,
  inertial_cmds
};

static THD_WORKING_AREA(waspbtif,1024);
static THD_FUNCTION(thspbt ,p)
{
  eventmask_t evt;
  size_t retSize;
  char resp[32];
  uint8_t initDone = 0;
  uint8_t cmdIndex = 0;
  uint8_t dly = 100;
  do{
    sdWrite(&SD1,cmdSeq[cmdIndex].cmd,strlen(cmdSeq[cmdIndex].cmd));
    retSize = sdReadTimeout(&SD1,resp,32,MS2ST(500));
    if(retSize > 0){
//      initDone = 1;
    }
  }while(initDone == 0);
  
  sdStop(&SD1);
  sdStart(&SD1,&serCfg_fs);
  
  cmdIndex = 10;
  while(1){
    //evt = chEvtWaitAnyTimeout(ALL_EVENTS,10);
    sdWrite(&SD1,cmdSeq[0].cmd,strlen(cmdSeq[0].cmd));
    chThdSleepMilliseconds(dly);
  }
  
}

void spbtInit(void)
{
  uint8_t retry = 0;
  uint8_t done = 0;
  uint8_t msg[256];
  sdStart(&SD1,&serCfg_ls);
  
//  do{
//    chprintf((BaseSequentialStream *)&SD1.oqueue,"AT+AB AutoReconnect enable");
//    sdReadTimeout(&SD1,
  //shellCreate(&shell_cfg,512, NORMALPRIO);
  
  // try to change the baudrate of the 
  thdspbt = chThdCreateStatic(waspbtif,sizeof(waspbtif),NORMALPRIO,thspbt,NULL);

}