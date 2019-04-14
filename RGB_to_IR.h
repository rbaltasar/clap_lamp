#if !defined RGBTOIR_H
#define RGBTOIR_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <IRremote.h>
#include "config.h"

#define NUM_IR_COLORS 15

class RGB_to_IR
{

private:
  
  IRsend m_irsend;
  HardwareSerial* m_hwPrint;

  const uint32_t m_codes_list[NUM_IR_COLORS] = {0xFFD02F, 0xFF8877, 0xFF9867, 0xFFA857, 0xFFB04F, 0xFF48B7, 0xFF906F, 0xFF28D7, 0xFF58A7, 0xFF6897, 0xFF30CF, 0xFF708F, 0xFF10EF, 0xFF18E7, 0xFF50AF};

  void send_ir_command(uint32_t command, uint8_t repetitions);
 
public:

  void configure(HardwareSerial* hwPrint);
  void send_ir_on();
  void send_ir_off();
  void send_ir_colorIdx(uint8_t idx);


};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
