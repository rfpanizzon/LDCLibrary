/*
 * LCDLibrary.h
 *
 *  Created on: Nov 1, 2020
 *      Author: rfpan
 */

#ifndef INC_LCDLIBRARY_H_
#define INC_LCDLIBRARY_H_

#define LCD_RS GPIO_PIN_0
#define LCD_EN GPIO_PIN_1
#define LCD_PORT_DATA GPIOA
#define LCD_PORT_CTRL GPIOB
#define LCD_D_BIT_MASK 0xF0

#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"

void LCD_ligar();

void LCD_limpar();

void LCD_CMD(char);

void LCD_char(uint32_t);

void LCD_string(const char *);

void LCD_char_especial(unsigned char, unsigned char *);

void LCD_escreve_char_especial();

#endif /* INC_LCDLIBRARY_H_ */
