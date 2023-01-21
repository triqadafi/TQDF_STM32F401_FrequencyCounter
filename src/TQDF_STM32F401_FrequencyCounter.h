#include <Arduino.h>


#ifndef TQDF_STM32F401_FrequencyCounter_h
#define TQDF_STM32F401_FrequencyCounter_h


/**
 * @brief test coba coba tentang brief
 * 
 * dibawahnya ini harusnya comment
 * 
 */
class TQDF_STM32F401_FrequencyCounter
{
  public:
    /**
     * @brief Construct a new TQDF_STM32F401_FrequencyCounter object
     * 
     * @param pin_input_channel PIN Channel Input to measure (square wave 5V tolerant)
     */
    TQDF_STM32F401_FrequencyCounter(uint8_t pin_input_channel);

    /**
     * @brief Read the frequency value
     * 
     */
    unsigned long frequencyRead();

    /**
     * @brief Begin
     * 
     */
    static void begin();

    
    /**
     * @brief Set the calibration value
     * 
     *  + means wider timegate
     *  - means smaller timegate
     */
    void setCalibrationValue(int val);
    
    static void OnTimer1Interrupt();

    static HardwareTimer *TIMER_gate;
    static HardwareTimer *TIMER_counter;
    static bool TIMER_interrupt;
    static volatile unsigned long COUNTER_result;

    static int TRQDF_CPU_MHz;
  private:
    static int TRQDF_Calibration_value;
};
#endif