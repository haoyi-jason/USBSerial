#ifndef _AD5593R_H_
#define _AD5593R_H_

#define AD5593R_ADDR_BASE       0x10
#define AD5593R_CHIP_ADDR_0     AD5593R_ADDR_BASE+0
#define AD5593R_CHIP_ADDR_1     AD5593R_ADDR_BASE+1
#define AD5593R_LSB     10000/4096
#define REG_CFG        0x00
#define REG_DACWR       0x10
#define REG_ADCRD       0x40
#define REG_DACRD       0x50
#define REG_GIORD       0x60
#define REG_REGRD       0x70

#define CFG_NOP         0x00
#define CFG_ADC_SEQ     0x02
#define CFG_GPCR        0x03
#define CFG_ADC_PIN     0x04
#define CFG_DAC_PIN     0x05
#define CFG_PULL_DN     0x06
#define CFG_LDAC_MODE   0x07
#define CFG_GPIO_CFG    0x08
#define CFG_GPIO_W      0x09
#define CFG_GPIO_RCFG   0x0a
#define CFG_PWR        0x0b

void ad5593rSetToDAC(uint8_t sla, uint8_t mask);
void ad5593rSetToADC(uint8_t sla, uint8_t mask);
uint16_t ad5593rReadChRaw(uint8_t ch);
float ad5593rReadChFloat(uint8_t ch);

#endif