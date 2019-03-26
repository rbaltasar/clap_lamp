#if !defined RGBTOIR_H
#define RGBTOIR_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <IRremote.h>
#include "config.h"

class RGB_to_IR
{

private:
  
  IRsend m_irsend;
  HardwareSerial* m_hwPrint;

  void send_ir_command(uint32_t command, uint8_t repetitions);
 
public:

  void configure(HardwareSerial* hwPrint);
  void send_ir_on();
  void send_ir_off();


};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
