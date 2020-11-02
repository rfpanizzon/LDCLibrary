/*
 * LCDLibrary.c
 *
 *  Created on: Nov 1, 2020
 *      Author: rfpan
 */

#include "LCDLibrary.h"

// CARACTERES ESPECIAIS
unsigned char a_acento_agudo[8] = {0x02, 0x04, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x00}; // á
unsigned char e_acento_agudo[8] = {0x02, 0x04, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x00}; // é
unsigned char a_acento_tiu[8] = {0x0D, 0x12, 0x06, 0x01, 0x0F, 0x11, 0x0F, 0x00}; 	// ã
unsigned char o_acento_tiu[8] = {0x0D, 0x12, 0x00, 0x0E, 0x11, 0x11, 0x0E, 0x00}; 	// õ
unsigned char cedilha[8] = {0x00, 0x0F, 0x10, 0x10, 0x0F, 0x02, 0x0E, 0x00}; 		// ç

/**
 * @brief Inicia o display
 *
 *
 * @retval None
 */
void LCD_ligar()
{
    HAL_Delay(15); // delay para ligar
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK, 0);

    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & 0x20, 1);
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (~0x20), 0);

    // delay
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 1); //E = 1
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 0); //E = 0

    LCD_CMD(0x28);
    LCD_CMD(0x0c);
    LCD_CMD(0x06);
    LCD_CMD(0x01);
}

/**
 * @brief Limpa o display
 *
 *
 * @retval None
 */
void LCD_limpar()
{
	LCD_CMD(0x01); //limpa display
}

/**
 * @brief Envia um comando para o display
 *
 * @param CMD O comando para ser executado
 * @retval None
 */
void LCD_CMD(char CMD)
{
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_RS, 0);

    // envia dados para porta
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & CMD, 1);
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (~CMD), 0);

    // delay
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 1); //E = 1
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 0); //E = 0

    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (CMD << 4), 1);
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (~(CMD << 4)), 0);

    // delay
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 1); //E = 1
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 0); //E = 0
}

/**
 * @brief Envia dados para o display
 *
 * @param dado o dado a ser enviado
 * @retval None
 */
void LCD_char(uint32_t dado)
{
    if ((dado >> 8) == 0xC3)
    {
        switch (dado & 0xFF)
        {
            case 0xA1: dado = 0; break;
            case 0xA9: dado = 1; break;
            case 0xA7: dado = 2; break;
            case 0xA3: dado = 3; break;
            case 0xB5: dado = 4; break;
            default: break;
        }
    }

    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_RS, 1);

    // envia os dados
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (dado), 1);
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (~dado), 0);

    // delay
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 1); //E = 1
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 0); //E = 0

    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (dado << 4), 1);
    HAL_GPIO_WritePin(LCD_PORT_DATA, LCD_D_BIT_MASK & (~(dado << 4)), 0);

    // delay
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 1); //E = 1
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_PORT_CTRL, LCD_EN, 0); //E = 0
}

/**
 * @brief Envia uma string
 * @param msg Uma string
 * @retval None
 */
void LCD_string(const char *msg)
{
    while ((*msg) != 0)
    {
        if (*msg == 0xC3)
        {
            switch (*(++msg))
            {
                case 0xA1: LCD_char(0); break;
                case 0xA9: LCD_char(1); break;
                case 0xA7: LCD_char(2); break;
                case 0xA3: LCD_char(3); break;
                case 0xB5: LCD_char(4); break;
                default: continue;
            }
        }
        else
        {
        	LCD_char(*msg);
        }
        msg++;
    }
}

/**
 * @brief Envia um dos char especial
 *
 * @param pos posicao do char
 * @param msg o char especial
 * @retval None
 */
void LCD_char_especial(unsigned char pos, unsigned char *msg)
{
    unsigned char i;
    if (pos < 8)
    {
    	LCD_CMD(0x40 + (pos * 8));
        for (i = 0; i < 8; i++)
        {
        	LCD_char(msg[i]);
        }
    }
}

/**
 * @brief Carrega os char especial
 *
 * @retval None
 */
void LCD_escreve_char_especial()
{
	LCD_char_especial(0, a_acento_agudo);
	LCD_char_especial(1, e_acento_agudo);
	LCD_char_especial(2, cedilha);
	LCD_char_especial(3, a_acento_tiu);
	LCD_char_especial(4, o_acento_tiu);
}
