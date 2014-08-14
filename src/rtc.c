/*
 * rtc.c
 *
 *  Created on: 09 авг. 2014 г.
 *      Author: dkr
 */

#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

char ch=0;

unsigned char bars[5] = {1, 2, 3, 4, 5};
/*                      Structure to save time parameters                       */
typedef struct
{
  unsigned char  hour;     //Hour
  unsigned char  min;      //minutes
  unsigned char  sec;      //seconds
} RTC_Time;

void RTC_Config(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;

   /* Configure one bit for preemption priority */
//   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

   /* Enable the RTC Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (10);
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = (10);
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

   PWR_BackupAccessCmd(ENABLE);
   RCC_LSEConfig(RCC_LSE_ON);
   while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
   RCC_RTCCLKCmd(ENABLE);

   RTC_WaitForSynchro();
   RTC_WaitForLastTask();
   RTC_ITConfig(RTC_IT_SEC, ENABLE);
   RTC_WaitForLastTask();
   RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
   RTC_WaitForLastTask();
}
