#include <stdio.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "main.h"
#include "font.h"
#include "lcd.h"

unsigned char Dummy;

#define SEND_BYTE_SPI()         \
  {                             \
      while(SPI1->SR & (1<<7)); \
      SPI1->DR = data;          \
      while(SPI1->SR & (1<<7)); \
      Dummy = SPI1->DR;         \
  }



void Initialize_SPI(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitTypeDef SPI_InitStructure;
    // указываем, что используем мы только передачу данных
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    // указываем, что наше устройство - Master
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    // передавать будем по 8 бит (=1 байт)
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    // режим 00
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    // передаём данные старшим битом вперёд (т.е. слева направо)
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    // внесём настройки в SPI
    SPI_Init(SPI1, &SPI_InitStructure);
    // включим  SPI1
    SPI_Cmd(SPI1, ENABLE);
    // SS = 1
    SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

void init_lcd_gpios()
{


  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = SCK_Pin | MOSI_Pin | MISO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SCK_MOSI_PORT, &GPIO_InitStructure);

  GPIO_WriteBit(SCK_MOSI_PORT, SCK_Pin, Bit_SET);
  GPIO_WriteBit(SCK_MOSI_PORT, MOSI_Pin, Bit_SET);
  GPIO_WriteBit(SCK_MOSI_PORT, MISO_Pin, Bit_SET);

  GPIO_InitStructure.GPIO_Pin = RST_Pin | CS_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CS_RST_PORT, &GPIO_InitStructure);

  GPIO_WriteBit(CS_RST_PORT, RST_Pin, Bit_SET);
  GPIO_WriteBit(CS_RST_PORT, CS_Pin, Bit_SET);

  GPIO_InitStructure.GPIO_Pin = VDD_Pin | DC_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(VDD_DC_PORT, &GPIO_InitStructure);

  GPIO_WriteBit(VDD_DC_PORT, VDD_Pin, Bit_SET);
  GPIO_WriteBit(VDD_DC_PORT, DC_Pin, Bit_SET);

}

void LCDInit(void)
{
	init_lcd_gpios();

    // Initialie SPI Interface
    Initialize_SPI();

    // Toggle reset pin
    GPIO_WriteBit(CS_RST_PORT, RST_Pin, Bit_RESET);
    uDelay(100);
    GPIO_WriteBit(CS_RST_PORT, RST_Pin, Bit_SET);
    uDelay(100);

    // Send sequence of command
    LCDSend(0x21, SEND_CMD); // LCD Extended Commands.
    LCDSend(0xC8, SEND_CMD); // Set LCD Vop (Contrast). 0xC8
    LCDSend(0x04 | !!(LCD_START_LINE_ADDR & (1u << 6)), SEND_CMD); // Set Temp S6 for start line
    LCDSend(0x40 | (LCD_START_LINE_ADDR & ((1u << 6) - 1)), SEND_CMD); // Set Temp S[5:0] for start line
    //LCDSend( 0x13, SEND_CMD );  // LCD bias mode 1:48.
    LCDSend(0x12, SEND_CMD); // LCD bias mode 1:68.
    LCDSend(0x20, SEND_CMD); // LCD Standard Commands, Horizontal addressing mode.
    //LCDSend( 0x22, SEND_CMD );  // LCD Standard Commands, Vertical addressing mode.
    LCDSend(0x08, SEND_CMD); // LCD blank
    LCDSend(0x0C, SEND_CMD); // LCD in normal mode.

    // Clear and Update
    LCDClear();
    LCDUpdate();
}

void LCDReset(void)
{
	GPIO_WriteBit(CS_RST_PORT, RST_Pin, Bit_RESET);
}

void LCDUpdate(void)
{
    int x, y;

    for (y = 0; y < 48 / 8; y++)
    {
        LCDSend(0x80, SEND_CMD);
        LCDSend(0x40 | y, SEND_CMD);
        for (x = 0; x < 84; x++)
        {
            LCDSend(LcdMemory[y * 84 + x], SEND_CHR);
        }
    }
}

void LCDSend(unsigned char data, unsigned char cd)
{
	GPIO_WriteBit(CS_RST_PORT, CS_Pin, Bit_RESET);

    if (cd == SEND_CHR)
    {
    	GPIO_WriteBit(VDD_DC_PORT, DC_Pin, Bit_SET);
    }
    else
    {
    	GPIO_WriteBit(VDD_DC_PORT, DC_Pin, Bit_RESET);
    }

    // send data over SPI
    SEND_BYTE_SPI();

    GPIO_WriteBit(CS_RST_PORT, CS_Pin, Bit_SET);
}


void LCDClear(void)
{
    int i;

    // loop all cache array
    for (i = 0; i < LCD_CACHE_SIZE; i++)
    {
        LcdMemory[i] = 0x00;
    }
}

void LCDChrXY(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned int index = 0;
    unsigned int i = 0;

    // check for out off range
    if (x > LCD_X_RES) return;
    if (y > LCD_Y_RES) return;

    index = (unsigned int) x * 6 + (unsigned int) y * 84;

    for (i = 0; i < 6; i++)
    {
        if (i==5)
          LcdMemory[index++] = 0x00;
        else
          LcdMemory[index++] = FontLookup[ch - 32][i];
    }

}

void LCDChrXYInverse(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned int index = 0;
    unsigned int i = 0;

    // check for out off range
    if (x > LCD_X_RES) return;
    if (y > LCD_Y_RES) return;

    index = (unsigned int) x * 6 + (unsigned int) y * 84;

    for (i = 0; i < 6; i++)
    {
       if (i==5)
          LcdMemory[index++] = 0xFF;
       else
          LcdMemory[index++] = ~(FontLookup[ch - 32][i]);
    }

}

void LCDContrast(unsigned char contrast)
{

    //  LCD Extended Commands.
    LCDSend(0x21, SEND_CMD);

    // Set LCD Vop (Contrast).
    LCDSend(0x80 | contrast, SEND_CMD);

    //  LCD Standard Commands, horizontal addressing mode.
    LCDSend(0x20, SEND_CMD);
}

void LCDStr(unsigned char row, const unsigned char *dataPtr, unsigned char inv)
{

    // variable for X coordinate
    unsigned char x = 0;

    // loop to the and of string
    while (*dataPtr)
    {
        if (inv)
        {
            LCDChrXYInverse(x, row, *dataPtr);
        }
        else
        {
            LCDChrXY(x, row, *dataPtr);
        }
        x++;
        dataPtr++;
    }

    LCDUpdate();
}
