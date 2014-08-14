#ifndef LCD_H_
#define LCD_H_

#define SCK_Pin 		GPIO_Pin_5
#define MISO_Pin 		GPIO_Pin_6
#define MOSI_Pin 		GPIO_Pin_7
#define SCK_MOSI_PORT 	GPIOA

#define CS_Pin		GPIO_Pin_0
#define RST_Pin 	GPIO_Pin_13
#define CS_RST_PORT GPIOC


#define VDD_Pin 		GPIO_Pin_1
#define DC_Pin			GPIO_Pin_8
#define VDD_DC_PORT 	GPIOB

#define SEND_CMD                   0
#define SEND_CHR                   1

#define LCD_X_RES                  84
#define LCD_Y_RES                  48

#define PIXEL_OFF                  0
#define PIXEL_ON                   1
#define PIXEL_XOR                  2

#define FONT_1X                    1
#define FONT_2X                    2

#define LCD_START_LINE_ADDR	(66-2)

#if LCD_START_LINE_ADDR	> 66
#error "Invalid LCD starting line address"
#endif

#define LCD_CACHE_SIZE             ((LCD_X_RES * LCD_Y_RES) / 8)

// LCD memory index
unsigned int LcdMemIdx;

// represent LCD matrix
unsigned char LcdMemory[LCD_CACHE_SIZE];

void init_lcd_gpios();

void Initialize_SPI(void);

void LCDInit(void);

void LCDSend(unsigned char data, unsigned char cd);
void LCDUpdate (void);
void LCDClear(void);
void LCDChrXY (unsigned char x, unsigned char y, unsigned char ch);
void LCDChrXYInverse (unsigned char x, unsigned char y, unsigned char ch);
void LCDContrast(unsigned char contrast);
void LCDStr(unsigned char row, const unsigned char *dataPtr, unsigned char inv);

#endif /* LCD_H_ */
