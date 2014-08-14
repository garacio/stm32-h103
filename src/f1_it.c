/*
 * f1_it.c
 *
 *  Created on: 09 авг. 2014 г.
 *      Author: dkr
 */

#include "stm32f10x_exti.h"
#include "rtc.h"
#include "main.h"

extern __IO uint32_t UserButtonPressed;


void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

// Обработчик прерывания EXTI0
void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if (UserButtonPressed < 0x03)
			UserButtonPressed++;
		else
			UserButtonPressed = 0x00;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void RTC_IRQHandler(void)
{
   if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
   {
	  if (UserButtonPressed == 0x03)
	  {
		  GPIOC->ODR ^= GPIO_Pin_12;
	  }
      RTC_ClearITPendingBit(RTC_IT_SEC);
      RTC_WaitForLastTask();
   }
}
