#ifndef _J1939_HY_
#define _J1939_HY_

enum j1939State{
  J1939_NONE,
  J1939_OK,
  J1939_ERROR
};

typedef struct{
  uint32_t pgn;
  uint8_t buf[8];
  uint8_t bufLen;
  uint8_t dst;
  uint8_t src;
  uint8_t prio;
}j1939_msg_t;

typedef uint8_t(*cbFunction)(void);
typedef void(*cbRecvMessage)(j1939_msg_t *m);

// main apis for J1939 protocol
void j1939Init(void);
void j1939Process();
uint8_t j1939Transmit(j1939_msg_t *msg);
uint8_t j1939PGNCbRegister(uint32_t pgnNumber,cbRecvMessage f);
uint8_t j1939PeriodicCbRegister(uint32_t start, uint32_t interval, cbFunction f);
void j1939RegisterMessageCb(cbRecvMessage f);

// persistance memory apis
uint8_t j1939MemWrite(uint16_t addr, uint8_t data);
uint8_t j1939MemRead(uint8_t addr,uint8_t *data);

// i/o apis
float j1939ReadVoltage(uint8_t ch);
uint16_t j1939ReadRaw(uint8_t ch);
uint8_t j1939ReadPin(uint8_t pin);
uint32_t j1939ReadAllPins();

// led apis
void j1939GLED_ON();
void j1939GLED_OFF();
void j1939GLED_TOG();

// type conversion apis
uint8_t bToU8(uint8_t *b);
uint16_t bToU16(uint8_t *b);
uint32_t bToU32(uint8_t *b);
int8_t bToI8(uint8_t *b);
int16_t bToI16(uint8_t *b);
int32_t bToI32(uint8_t *b);
void bFromU16(uint8_t *b, uint16_t v);
void bFromU32(uint8_t *b, uint32_t v);
void bFromI16(uint8_t *b, int16_t v);
void bFromI32(uint8_t *b, int32_t v);


#endif