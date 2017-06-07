#include "loader.h"
#include "flashAccess.h"

uint8_t checksum(uint8_t *data, uint8_t length)
{
  uint8_t i;
  uint8_t sum = 0;
  
  for(i=0;i<length;i++) sum += data[i];
  
  return sum;
}

uint8_t read_cmd(BaseChannel *chn, uint8_t flags)
{
  cmd_header_t header;
  uint8_t data_buf[DATA_BUF_SIZE];
  uint8_t err_buf = MASK_REPLY_ERR;
  
  if(chnReadTimeout(chn,(uint8_t*)&header,4,MS2ST(25)) < sizeof(cmd_header_t)){
    chnPutTimeout(chn,err_buf+1,MS2ST(25));
    return 1;
  }
  
  if(header.magic1 != MAGIC1 || header.magic2 != MAGIC2 || !(header.type & MASK_CMD) || header.len < 5){
    chSequentialStreamPut(chn,err_buf+2);
    return 2;
  }
  
  if((uint8_t)chnReadTimeout(chn,data_buf,header.len-4,MS2ST(25)) < (header.len - 4)){
    chnPutTimeout(chn,err_buf+3,MS2ST(25));
    return 3;
  }
  
  const uint8_t cs1 = chksum((uint8_t)&header,sizeof(header))+chksum(data_buf,header.len-5);
  const uint8_t cs2 = data_buf[header.len - 5];
  
  if(cs1 != cs2){
    chnPutTimeout(chn,err_buf+4,MS2ST(25));
    return 4;
  }
  
  uint8_t status = 0;
  switch(header.type){
  case MASK_CMD | CMD_ERASE:
    status = eraseHandler(chn,leToInt(data_buf));
    break;
  case MASK_CMD | CMD_READ:
    status = readHandler(chn,data_buf);
    break;
  case MASK_CMD | CMD_WRITE:
    status = writeHandler(chn,data_buf,header.len - 5);
    break;
  case MASK_CMD | CMD_RESET:
    status = resetHandler(chn);
    break;
  case MASK_CMD | CMD_GET_FLAGS:
    status = sendFlags(chn,flags);
    break;
  default:
    chnPutTimeout(chn,err_buf+5,MS2ST(25));
    break;
  }
  
  if(status){
    status += 5;
  }
  
  return status;              
}

uint8_t writeHandler(BaseChannel *chn, uint8_t *buf, uint8_t len)
{
  if((len - 4) %4){
    chnPutTimeout(chn,MASK_REPLY_ERR | CMD_WRITE,MS2ST(25));
    return 1;
  }
  
  uint32_t offset = leToInt(buf);
  uint32_t *data_buf = (uint32_t*)(buf+4);
  uint8_t replbuf = MASK_REPLY_OK;
  
  uint8_t res = writeFlash(offset,data_buf,(len-4)/4);
  if(res != 0) replbuf = MASK_REPLY_ERR;
  chnPutTimeout(chn,replbuf,MS2ST(25));
  
  return res;
}

uint8_t readHandler(BaseChannel *chn,uint8_t* buf)
{
  uint32_t address = leToInt(buf);
  uint32_t buf_len = leToInt(buf+4);
  chnWrite(chn,(uint8_t*)(address+USER_APP_ADDR),buf_len);
  return 0;
}

uint8_t sendFlags(BaseChannel *chn, uint8_t flags)
{
  uint8_t buf[2];
  buf[0] = MASK_REPLY_OK;
  buf[1] = flags;
  chnWrite(chn,buf,2);
  return 0;
}

uint8_t eraseHandler(BaseChannel *chn, uint8_t len)
{
  uint8_t buf[2];
  buf[0] = MASK_REPLY_OK;
  buf[1] = ERASE_OK;
  
  if(erashFlash(len)){
    buf[1] = 0;
  }
  
  chnWrite(chn,buf,2);
  return 0;
}

uint8_t resetHandler(BaseChannel *chn)
{
  chnPutTimeout(chn, MASK_REPLY_OK, MS2ST(25));
  chThdSleepMilliseconds(100);
  chSysDisable();
  
  NVIC_SystemReset();
  return 0;
}