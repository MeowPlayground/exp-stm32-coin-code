#ifndef __MEOW_TFT_H
#define __MEOW_TFT_H

#include "stm32f1xx_hal.h"
#include "spi.h"

// ========== SPI 句柄（根据你具体使用的 SPI 实例进行定义）==========
#define MEOW_TFT_SPI_HANDLE hspi2

// ========== 控制引脚 ==========
#define MEOW_TFT_CS_LOW() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define MEOW_TFT_CS_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

#define MEOW_TFT_RST_LOW() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define MEOW_TFT_RST_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)

#define MEOW_TFT_DC_LOW() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)
#define MEOW_TFT_DC_HIGH() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)

#define MEOW_TFT_BL_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define MEOW_TFT_BL_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)

// ========== 显示参数 ==========
#define MEOW_TFT_WIDTH 240
#define MEOW_TFT_HEIGHT 320

// ========== API ==========
void Meow_TFT_Init(void);
void Meow_TFT_FillScreen(uint16_t color);
void Meow_TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

#endif
