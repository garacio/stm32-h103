#include <stdio.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "main.h"
#include "lcd.h"
#include "rtc.h"

__IO uint32_t TimingDelay = 0;
__IO uint32_t UserButtonPressed = 0x00;

LED_STATE LedState = OFF;

int main()
{
  int Inverse=0;

  SysTick_Config(72);

  init_gpios();
  init_exti();
  RTC_Config();

  init_lcd_gpios();

  LCDInit();
  LCDContrast(0x70);
  LCDStr(0, (unsigned char *)"*** OLIMEX ***", !Inverse);
  LCDStr(1, (unsigned char *)"  STM32-H103  ", Inverse);


  while (1)
    {
	   switch(UserButtonPressed)
	   {
		   case 0x00:
			   LCDStr(2, (unsigned char *)"    STAGE 1   ", !Inverse);
			   LCDStr(3, (unsigned char *)"    LED ON    ", Inverse);
				LED_ON();
				break;
		   case 0x01:
			   LCDStr(2, (unsigned char *)"    STAGE 2   ", !Inverse);
			   LCDStr(3, (unsigned char *)"    LED OFF   ", Inverse);
			   LED_OFF();
			   break;
		   case 0x02:
			   LCDStr(2, (unsigned char *)"    STAGE 3   ", !Inverse);
			   LCDStr(3, (unsigned char *)" Blink on CLK ", Inverse);
			   LED_BLINK();
			   break;
		   case 0x03:
			   LCDStr(2, (unsigned char *)"    STAGE 4   ", !Inverse);
			   LCDStr(3, (unsigned char *)" Blink on RTC ", Inverse);
			   break;
		}

    }
}

void uDelay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime*1000;

  while(TimingDelay != 0);
}


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement()
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

void init_gpios()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = LED_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LED_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);

  GPIO_WriteBit(LED_PORT, LED_PIN, Bit_RESET);
  GPIO_WriteBit(BUTTON_PORT, BUTTON_PIN, Bit_RESET);
}

void init_exti()
{

	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

	// Настраиваем прерывания для USER BUTTON

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// Активируем и выставляем прерывание кнопки в наименьший приоритет

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel =  EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void LED_ON()
{
	LedState = ON;
	GPIO_WriteBit(LED_PORT, LED_PIN, Bit_RESET);
}

void LED_OFF()
{
	LedState = OFF;
	GPIO_WriteBit(LED_PORT, LED_PIN, Bit_SET);
}

void LED_BLINK()
{
	GPIOC->ODR ^= GPIO_Pin_12;
	Delay(500);
	GPIOC->ODR ^= GPIO_Pin_12;
	Delay(500);
}

