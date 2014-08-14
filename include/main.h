/*
 * main.h
 *
 *  Created on: 08 авг. 2014 г.
 *      Author: dkr
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f10x.h"

#define LED_PORT	GPIOC
#define LED_PIN		GPIO_Pin_12

#define BUTTON_PORT	GPIOA
#define BUTTON_PIN	GPIO_Pin_0

typedef enum
{
	ON,
	OFF
}LED_STATE;

#define PIN_LOW(PORT, PIN)   PORT = PORT & (~(1<<PIN));
#define PIN_HIGH(PORT, PIN)  PORT = PORT | (1<<PIN);

void Pin_Output (GPIO_TypeDef* GPIOx, int Pin);

void LED_ON();
void LED_OFF();
void LED_BLINK();

void init_exti();
void init_gpios();

void TimingDelay_Decrement();
void Delay(__IO uint32_t nTime);

#endif /* MAIN_H_ */
