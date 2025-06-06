#include "meow_tft.h"

// 私有函数
static void Meow_TFT_SendCommand(uint8_t cmd)
{
    MEOW_TFT_DC_LOW();
    MEOW_TFT_CS_LOW();
    HAL_SPI_Transmit(&MEOW_TFT_SPI_HANDLE, &cmd, 1, HAL_MAX_DELAY);
    MEOW_TFT_CS_HIGH();
}

static void Meow_TFT_SendData(uint8_t *buff, size_t buff_size)
{
    MEOW_TFT_DC_HIGH();
    MEOW_TFT_CS_LOW();
    HAL_SPI_Transmit(&MEOW_TFT_SPI_HANDLE, buff, buff_size, HAL_MAX_DELAY);
    MEOW_TFT_CS_HIGH();
}

static void Meow_TFT_SendColor(uint16_t color, uint32_t size)
{
    uint8_t data[2] = {color >> 8, color & 0xFF};
    MEOW_TFT_DC_HIGH();
    MEOW_TFT_CS_LOW();
    for (uint32_t i = 0; i < size; i++)
    {
        HAL_SPI_Transmit(&MEOW_TFT_SPI_HANDLE, data, 2, HAL_MAX_DELAY);
    }
    MEOW_TFT_CS_HIGH();
}

static void Meow_TFT_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];

    Meow_TFT_SendCommand(0x2A);
    data[0] = x0 >> 8;
    data[1] = x0 & 0xFF;
    data[2] = x1 >> 8;
    data[3] = x1 & 0xFF;
    Meow_TFT_SendData(data, 4);

    Meow_TFT_SendCommand(0x2B);
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    Meow_TFT_SendData(data, 4);

    Meow_TFT_SendCommand(0x2C);
}

// 初始化
void Meow_TFT_Init(void)
{
    MEOW_TFT_RST_LOW();
    HAL_Delay(50);
    MEOW_TFT_RST_HIGH();
    HAL_Delay(120);

    MEOW_TFT_BL_ON();

    Meow_TFT_SendCommand(0x36);
    uint8_t data1 = 0x00;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0x3A);
    data1 = 0x05;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xB2);
    uint8_t porch[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
    Meow_TFT_SendData(porch, sizeof(porch));

    Meow_TFT_SendCommand(0xB7);
    data1 = 0x35;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xBB);
    data1 = 0x19;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xC0);
    data1 = 0x2C;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xC2);
    data1 = 0x01;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xC3);
    data1 = 0x12;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xC4);
    data1 = 0x20;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xC6);
    data1 = 0x0F;
    Meow_TFT_SendData(&data1, 1);

    Meow_TFT_SendCommand(0xD0);
    uint8_t pwr[] = {0xA4, 0xA1};
    Meow_TFT_SendData(pwr, sizeof(pwr));

    Meow_TFT_SendCommand(0xE0);
    uint8_t gamma_pos[] = {0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19};
    Meow_TFT_SendData(gamma_pos, sizeof(gamma_pos));

    Meow_TFT_SendCommand(0xE1);
    uint8_t gamma_neg[] = {0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44, 0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19};
    Meow_TFT_SendData(gamma_neg, sizeof(gamma_neg));

    Meow_TFT_SendCommand(0x21); // Inversion ON
    Meow_TFT_SendCommand(0x11); // Sleep Out
    HAL_Delay(120);
    Meow_TFT_SendCommand(0x29); // Display ON
}

// 清屏
void Meow_TFT_FillScreen(uint16_t color)
{
    Meow_TFT_SetAddressWindow(0, 0, MEOW_TFT_WIDTH - 1, MEOW_TFT_HEIGHT - 1);
    Meow_TFT_SendColor(color, MEOW_TFT_WIDTH * MEOW_TFT_HEIGHT);
}

// 画点
void Meow_TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= MEOW_TFT_WIDTH || y >= MEOW_TFT_HEIGHT)
        return;
    Meow_TFT_SetAddressWindow(x, y, x, y);
    uint8_t data[] = {color >> 8, color & 0xFF};
    Meow_TFT_SendData(data, 2);
}
