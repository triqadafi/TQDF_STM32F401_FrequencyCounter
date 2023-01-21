#include "TQDF_STM32F401_FrequencyCounter.h"

int TQDF_STM32F401_FrequencyCounter::TRQDF_CPU_MHz;
int TQDF_STM32F401_FrequencyCounter::TRQDF_Calibration_value;
bool TQDF_STM32F401_FrequencyCounter::TIMER_interrupt;
volatile unsigned long TQDF_STM32F401_FrequencyCounter::COUNTER_result;
HardwareTimer *TQDF_STM32F401_FrequencyCounter::TIMER_gate;
HardwareTimer *TQDF_STM32F401_FrequencyCounter::TIMER_counter;

TQDF_STM32F401_FrequencyCounter::TQDF_STM32F401_FrequencyCounter(uint8_t pin_input_channel){
  // TRQDF_gate = gate;
  // TRQDF_counter = counter;
  TIMER_gate = new HardwareTimer(TIM5);
  TIMER_counter = new HardwareTimer(TIM2);
  TRQDF_CPU_MHz = 84;
  TRQDF_Calibration_value = 0;
}

void TQDF_STM32F401_FrequencyCounter::begin(){
    TIMER_gate->setPrescaleFactor(1); 
    TIMER_gate->setOverflow((TRQDF_CPU_MHz*1000000-1) + TRQDF_Calibration_value);
    TIMER_gate->setInterruptPriority(0,0);
    TIMER_gate->attachInterrupt(OnTimer1Interrupt);

    TIMER_gate->refresh(); // Make register changes take effect
    TIMER_gate->resume(); // Start

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_ETR
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
    sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
    sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
    sClockSourceConfig.ClockFilter = 0;
  
    if (HAL_TIM_ConfigClockSource(TIMER_counter->getHandle(), &sClockSourceConfig) != HAL_OK){
      Error_Handler();
    }
    
    TIMER_counter->setOverflow(4294967295);
    TIMER_counter->refresh(); // Start
    TIMER_counter->resume(); // Start
}

void TQDF_STM32F401_FrequencyCounter::setCalibrationValue(int val){
  TRQDF_Calibration_value = val;
}

void TQDF_STM32F401_FrequencyCounter::OnTimer1Interrupt(){
  COUNTER_result = TIM2->CNT;
  TIM2->CNT = 0;
}

unsigned long TQDF_STM32F401_FrequencyCounter::frequencyRead(){
  return COUNTER_result;
}

