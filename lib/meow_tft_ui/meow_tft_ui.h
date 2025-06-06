#ifndef __MEOW_TFT_UI_H
#define __MEOW_TFT_UI_H

#include <stdint.h>

// 字符集定义
#define MAX_CHARSET_SIZE 256

typedef struct
{
    const uint8_t (*data)[]; // 字符数据
    uint8_t width;           // 字符宽度
    uint8_t height;          // 字符高度
    const char *charset;     // 字符集（字符映射表）
    uint16_t charCount;      // 字符数量
} MeowFont;

typedef enum
{
    MEOW_LANG_EN,
    MEOW_LANG_ZH
} MeowLang;

void Meow_TFT_DrawHLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void Meow_TFT_DrawVLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void Meow_TFT_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void Meow_TFT_DrawCharUTF8(uint16_t x, uint16_t y, const char *utf8char, int len, uint16_t color, uint16_t bg, uint8_t size);
void Meow_TFT_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);
void Meow_TFT_SetFont(const MeowFont *font);
void Meow_TFT_SetCursor(uint16_t x, uint16_t y);
void Meow_TFT_SetTextColor(uint16_t color, uint16_t bg);
void Meow_TFT_SetTextSize(uint8_t size);
void Meow_TFT_Printf(const char *format, ...);

#endif
