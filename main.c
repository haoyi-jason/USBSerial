#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "evtimer.h"
#include "chprintf.h"
#include "stdlib.h"
#include "spbt_if.h"
//#include "usbserial.h"
//#include "comm.h"
//#include "hal_ad7124.h"
//#include "mbtask.h"
#include "sysParam.h"
#include "usbcfg.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE(256)

#define VDD1_ON()   palSetPad(GPIOC,8)
#define VDD2_ON()   palSetPad(GPIOC,9)
#define VDD1_OFF()   palClearPad(GPIOC,8)
#define VDD2_OFF()   palClearPad(GPIOC,9)

uint8_t reset_flags = 0;
extern thread_t *thdAD7124;

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state time\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%08lx %08lx %4lu %4lu %9s\r\n",
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state]);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
  thread_t *tp;

//  (void)argv;
//  if (argc > 0) {
//    chprintf(chp, "Usage: test\r\n");
//    return;
//  }
//  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriorityX(),
//                           TestThread, chp);
//  if (tp == NULL) {
//    chprintf(chp, "out of memory\r\n");
//    return;
//  }
//  chThdWait(tp);
}

/* Can be measured using dd if=/dev/xxxx of=/dev/null bs=512 count=10000.*/
static void cmd_write(BaseSequentialStream *chp, int argc, char *argv[]) {
  static uint8_t buf[] =
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: write\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
#if 1
    /* Writing in channel mode.*/
    chnWrite(&SDU1, buf, sizeof buf - 1);
#else
    /* Writing in buffer mode.*/
    (void) obqGetEmptyBufferTimeout(&SDU1.obqueue, TIME_INFINITE);
    memcpy(SDU1.obqueue.ptr, buf, SERIAL_USB_BUFFERS_SIZE);
    obqPostFullBuffer(&SDU1.obqueue, SERIAL_USB_BUFFERS_SIZE);
#endif
  }
  chprintf(chp, "\r\n\nstopped\r\n");
}

static void cmd_i2cRead(BaseSequentialStream *chp, int argc, char *argv[])
{

}

static void cmd_i2cWrite(BaseSequentialStream *chp, int argc, char *argv[])
{

}

static void cmd_Power(BaseSequentialStream *chp, int argc, char *argv[])
{
  if(argv > 0){
    if(strcmp(argv[0],"ON") == 0){

    }else if(strcmp(argv[0],"OFF") == 0){

    }
  }
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"write", cmd_write},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};


static THD_WORKING_AREA(waThread1,128);
static msg_t Thread1(void *arg)
{
  (void)arg;
  chRegSetThreadName("Blinker");
  while(TRUE){
    chThdSleepMilliseconds(10);
  }
}


/*
 * Application entry point.
 */
int main(void) {
  thread_t *shelltp = NULL;
   // initial chibios 
  halInit();
  chSysInit();
//  sysParamInit();
  
  sduObjectInit(&SDU1);
  sduStart(&SDU1,&serusbcfg);

  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp,&usbcfg);
  usbConnectBus(serusbcfg.usbp);

  shellInit();

  // enable usb bus
  palClearPad(GPIOA,15);
//  palSetPad(GPIOA,15);

  VDD1_ON();
  VDD2_ON();
  VDD1_OFF();
  VDD2_OFF();

  while(1){
    if(!shelltp && (SDU1.config->usbp->state == USB_ACTIVE))
      shelltp = shellCreate(&shell_cfg1,SHELL_WA_SIZE,NORMALPRIO);
    else if(chThdTerminatedX(shelltp)){
      chThdRelease(shelltp);
      shelltp = NULL;
    }
    chThdSleepMilliseconds(1000);

  }
   
}
