
#if !defined CONFIG_H
#define CONFIG_H

//IR sender configuration
#define LED_PIN 2
#define SEND_REPETITION_PERIOD 200//Delay between send repetitions
#define SEND_REPETITIONS 2//Amount of send repetitions for a single command

//FFT configuration
#define NSAMPLES 64

//Clap configuration
#define SOUND_THRESHOLD 530 //Needs to be calibrated
#define MAX_FREQ (5*NSAMPLES)/6
#define MIN_FREQ NSAMPLES/10

//---------- Debugging ------------------
#define DEBUG_TRACES_TIME 0
#define DEBUG_TRACES_FREQ 0
#define DEBUG_TRACES_GENERAL 0

#endif
