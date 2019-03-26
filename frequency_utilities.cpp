
#include "frequency_utilities.h"


FrequencyUtilities::FrequencyUtilities()
{
}

void FrequencyUtilities::begin(HardwareSerial* hwPrint)
{
  m_hwPrint = hwPrint;

  max_level_actual = 0;
  max_level_historic = 0;
  max_level_freq_historic = 0;
  iterations_count = 0;

}

void FrequencyUtilities::take_samples()
{
  double val;
  double maxVal = 0;
  double minVal = 1024;
  for (uint16_t i = 0; i < NSAMPLES; i++)
  {
    newTime = micros()-oldTime;
    oldTime = newTime;
    val = (double)analogRead(A0);
    //m_hwPrint->println(i);
    val > maxVal ? maxVal = val : maxVal = maxVal;
    val < minVal ? minVal = val : minVal = minVal;
    //data[i] = val/4 - 128;
    data[i] = val;
    im[i] = 0;
  }

  max_level_actual = maxVal; 

  #ifdef DEBUG_TRACES_TIME
  m_hwPrint->print("Max value: ");
  m_hwPrint->print(maxVal);
  m_hwPrint->print(" Min value: ");
  m_hwPrint->print(minVal);
  m_hwPrint->print(" Diff value: ");
  m_hwPrint->print(maxVal - minVal);
  m_hwPrint->print(" Mas level dBmV: ");
  m_hwPrint->println(max_level_actual);
  #endif
}

void FrequencyUtilities::time_to_freq()
{
   FFT.Windowing(data, NSAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
   FFT.Compute(data, im, NSAMPLES, FFT_FORWARD);
   FFT.ComplexToMagnitude(data, im, NSAMPLES);
}

void FrequencyUtilities::get_freq_info()
{

  max_level_freq_instant = 0;
  dominant_frequency = 99;

  for(uint8_t i=2; i<NSAMPLES/2; i++)
  {    
    if (data[i] > max_level_freq_instant )
    {
      dominant_frequency = i;
      max_level_freq_instant = data[i];
    }
  }


  #ifdef DEBUG_TRACES_FREQ
  m_hwPrint->print("Dominant frequency: ");
  m_hwPrint->println(dominant_frequency);
  m_hwPrint->print(" max_level_freq_instant: ");
  m_hwPrint->println(max_level_freq_instant);
  m_hwPrint->print(" max_level_freq_historic: ");
  m_hwPrint->println(max_level_freq_historic);
  #endif
}


bool FrequencyUtilities::clap_detected()
{
  //Check if sound above threshold detected
  if(max_level_actual > SOUND_THRESHOLD)
  {  
    //Check if the sound max frequency  is in the frequency of the clap
    if( (dominant_frequency > MIN_FREQ) && (dominant_frequency < MAX_FREQ))
    {
      m_hwPrint->println("Clap detected!");
      return true;
    }
  }

  return false;
}

bool FrequencyUtilities::detect_single_clap()
{
  //Sample input signal
  take_samples();
  time_to_freq(); 
  get_freq_info();  

  return clap_detected();
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
