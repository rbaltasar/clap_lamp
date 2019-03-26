#include "RGB_to_IR.h"

void RGB_to_IR::configure(HardwareSerial* hwPrint)
{
  m_hwPrint = hwPrint;
}


void RGB_to_IR::send_ir_command(uint32_t command, uint8_t repetitions)
{
  digitalWrite(LED_BUILTIN, LOW);
  for(uint8_t i = 0; i < repetitions; i++)
  {
    m_irsend.sendNEC(command, 32);
    delay(SEND_REPETITION_PERIOD);
    yield();
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void RGB_to_IR::send_ir_on()
{
  send_ir_command(0xFFE01F,SEND_REPETITIONS);
  m_hwPrint->println("Sending ON command");
}

void RGB_to_IR::send_ir_off()
{
  send_ir_command(0xFF609F,SEND_REPETITIONS);
  m_hwPrint->println("Sending OFF command");
}
