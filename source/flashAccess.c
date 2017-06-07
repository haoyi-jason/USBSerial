#include "flashAccess.h"

typedef void (*FuncPtr)(void);

void jumpTOUser(uint32_t address)
{
  typedef void (*pFunction)(void);
  
  pFunction Jump_To_Application;
  
  vu32* JumpAddress;
  const vu32* ApplicationAddress = (vu32*)address;
  
  JumpAddress = (vu32*)ApplicationAddress[1];
  Jump_To_Application = (pFunction)JumpAddress;
  SCB->VTOR = SCB_ICSR_PENDSVCLR_Msk;
  //SCB_ICSR = ICSR_PENDSVCLR;
  //SCB_BTOR = 1;
  
  // disable all interrupts
  uint8_t i;
  for(i=0;i<8;i++){
    NVIC->ICER[i] = NVIC->IABR[i];
  }
  
  __set_MSP((u32) (ApplicationAddress[0]));
  Jump_To_Application();
}

uint8_t erasfFlash(uint32_t len)
{
  uint32_t addr;
  if(len %4) return 1;
  
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  
  for(addr = USER_APP_ADDR; addr < USER_APP_ADDR + (64*1024);addr+=FLASH_PAGE_SIZE){
    if(FLASH_ErasePage(addr) != FLASH_COMPLETE){
      FLASH_Lock();
      return 1;
    }
  }
  
  FLASH_Lock();
  return 0;
}

uint8_t writeFlash(uint32_t addr, uint32_t *buf, uint8_t len)
{
  uint8_t i;
  
  if(addr > 240*1024 || buf == NULL || !len) return 1;
  
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    
  for(i=0;i<len;i++){
    if(FLASH_ProgramWord(USER_APP_ADDR + addr + (i*4),buf[i]) != FLASH_COMPLETE){
      FLASH_Lock();
      return 1;
    }
  }
  
  FLASH_Lock();
  return 0;
}

uint32_t leToInt(uint8_t *ptr) {

  return ((uint32_t)ptr[3] << 24) |
      ((uint32_t)ptr[2] << 16) |
      ((uint32_t)ptr[1] << 8) |
      (uint32_t)ptr[0];
}

uint32_t beToInt(uint8_t *ptr) {

  return ((uint32_t)ptr[0] << 24) |
      ((uint32_t)ptr[1] << 16) |
      ((uint32_t)ptr[2] << 8) |
      (uint32_t)ptr[3];
}
