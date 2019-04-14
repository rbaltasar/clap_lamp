
/* Includes */
#include "RGB_to_IR.h"
#include "frequency_utilities.h"
#include "clapDetection.h"

#if DEBUG_TRACES_FREQ
#include "serialFreqDisplay.h" //Only for frequency debugging
#endif

/* IR utilities */
RGB_to_IR ir_sender;

/* Audio signal spectrum display */
#if DEBUG_TRACES_FREQ
SerialFreqDisplay displ(THRESHOLD, NSAMPLES/2);
#endif

/* Audio signal analyzer */
FrequencyUtilities FreqUtilities;

/* Clap detection features */
clapDetection clap_detector(&FreqUtilities);

/* Clap detection variables */
uint8_t next_color_idx = 0;
bool lamp_state = false;

/* Setup */
void setup() {

#if DEBUG_TRACES_GENERAL
  Serial.begin(115200);
#endif
 
  /* Share the serial printer object for debugging functionalities */
  HardwareSerial* hwPrint;
  hwPrint = &Serial;
  ir_sender.configure(hwPrint);
  FreqUtilities.begin(hwPrint);

  /* LED */
  pinMode(LED_PIN, OUTPUT);

  /* Startup effect */
  set_led(true);
  delay(500);
  set_led(false);
  delay(500);
  set_led(true);
  delay(500);
  set_led(false);
  delay(500);
  set_led(true);
  delay(500);
  set_led(false);
}

/* Set LED */
void set_led(bool state)
{
  if(state)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else digitalWrite(LED_PIN, LOW);
}

/* Device-specific logic */
void device_logic()
{
 
  /* Process double clap detected --> Switch ON/OFF */
  if(clap_detector.getDetectionType() == DOUBLE_CLAP)
  {
    if(lamp_state)
    {
      set_led(true);
      ir_sender.send_ir_off();
      lamp_state = false;
      delay(1000);
      set_led(false);
    }
    else
    {
      set_led(true);
      ir_sender.send_ir_on();
      lamp_state = true;
      delay(1000);
      set_led(false);
    } 
    clap_detector.resetDetectionType();
  } 

  /* Process triple clap detected --> Change color */
  else if(clap_detector.getDetectionType() == TRIPLE_CLAP)
  {   
    ir_sender.send_ir_colorIdx(next_color_idx++);
    next_color_idx >= NUM_IR_COLORS ? next_color_idx = 0 : next_color_idx = next_color_idx;
    clap_detector.resetDetectionType();
    int i=0;
    while(i<10)
    {
      set_led(true);
      delay(100);
      set_led(false);
      delay(100);
      i++;
    } 
  }
}

void loop() 
{ 
  /* Feed the clap detector algorithm. Apply logic if anything detected */
  if(clap_detector.feed()) device_logic();
}
