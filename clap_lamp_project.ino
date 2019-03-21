
/* Includes */
#include "RGB_to_IR.h"
#include "frequency_utilities.h"

/* Debugging */
#define DEBUG_ENABLED 1
#if DEBUG_ENABLED
#include "serialFreqDisplay.h" //Only for frequency debugging
#endif

/* Clap detection variables */
unsigned long last_clap = 0;
uint8_t clap_number = 0;
unsigned long time_since_clap = 0;
bool doubleclap_detected = false;
bool lamp_state = false;

/* IR utilities */
RGB_to_IR ir_sender;

/* Audio signal spectrum display */
#if DEBUG_ENABLED
SerialFreqDisplay displ(THRESHOLD, NSAMPLES/2);
#endif

/* Audio signal analyzer */
FrequencyUtilities FreqUtilities;

/* Setup */
void setup() {

#if 1
  Serial.begin(115200);
#endif
 
  /* Share the serial printer object for debugging functionalities */
  HardwareSerial* hwPrint;
  hwPrint = &Serial;
  ir_sender.configure(hwPrint);
  FreqUtilities.begin(hwPrint);

  /* LED */
  pinMode(LED_PIN, OUTPUT);

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

/* Node-specific logic */
void node_logic()
{
  /* Process double clap detected */
  if(doubleclap_detected)
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

    doubleclap_detected = false;
  }  
}

/* State machine to handle the clap detection asynchronously */
void clap_detection_sm()
{
  unsigned long current_time = millis();
  //Time since last clap detected
  time_since_clap = current_time - last_clap;

  //Detection of a clap in the current iteration
  //This includes signal sampling, FFT and spectrum analysis
  bool clap = FreqUtilities.detect_single_clap();

  /* In case a clap is detected */
  if(clap)
  {
#if DEBUG_ENABLED
    Serial.print("Time since last clap: ");
    Serial.println(time_since_clap);
#endif

    //Last clap happened long time ago, the detected clap is the first of the sequence
    if( (time_since_clap > 2000) )
    {
      //First clap detected!
      clap_number = 1;
#if DEBUG_ENABLED
      Serial.println("First clap detected");
#endif
    }
    //A clap happened "recently"
    else
    {
      //A clap has already been detected before
      if(clap_number == 1)
      {
        //If the timing is right, this is the second clap of the sequence
        if( (time_since_clap > 80) && (time_since_clap < 600) )
        {
          clap_number = 2;
#if DEBUG_ENABLED
          Serial.println("Second clap detected");
#endif
        }
      }
      //More than one clap have been detected before
      else if(clap_number > 1)
      {
        //If the timing is right, this is just another clap in a large sequence of claps
        if( (time_since_clap > 150) && (time_since_clap < 600) )
        {
          clap_number++;
#if DEBUG_ENABLED
          Serial.println("Further claps detected");
#endif
        }
      }
    }
    //Save the timestamp of the clap
    last_clap = millis();
  }

  /* No clap detected */
  else
  {
    //If we already counted two claps and no further claps are detected in one second, sequence complete
    if( (clap_number == 2) && (time_since_clap > 1000) )
    {
#if DEBUG_ENABLED
      Serial.println("Order detected!!");
#endif
      clap_number = 0;
      doubleclap_detected = true;     
    }
  }

  current_time = millis() - current_time;

}

void loop() { 

  /* Clap detection state machine */
  clap_detection_sm();
  node_logic();

  //delay(100);
}
