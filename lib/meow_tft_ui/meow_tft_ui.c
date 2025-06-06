#include "meow_tft_ui.h"
#include "meow_tft.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static const MeowFont *currentFont = NULL; ///< 当前使用的字体

/**
 * @brief 获取UTF-8字符的字节数（1–4字节）
 *
 * @param s UTF-8字符串的指针
 * @return UTF-8字符的字节数
 */
static int utf8_char_len(const char *s)
{
    unsigned char c = (unsigned char)s[0];
    if ((c & 0x80) == 0x00)
        return 1; // 0xxx xxxx
    else if ((c & 0xE0) == 0xC0)
        return 2; // 110x xxxx
    else if ((c & 0xF0) == 0xE0)
        return 3; // 1110 xxxx
    else if ((c & 0xF8) == 0xF0)
        return 4; // 1111 0xxx
    return 1;     // fallback
}

/**
 * @brief 设置当前字体
 *
 * @param font 要设置的字体
 */
void Meow_TFT_SetFont(const MeowFont *font)
{
    currentFont = font;
}

/**
 * @brief 在屏幕上绘制一条水平线
 *
 * @param x 起始x坐标
 * @param y 起始y坐标
 * @param length 线段长度
 * @param color 线段颜色
 */
void Meow_TFT_DrawHLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    for (uint16_t i = 0; i < length; i++)
        Meow_TFT_DrawPixel(x + i, y, color);
}

/**
 * @brief 在屏幕上绘制一条垂直线
 *
 * @param x 起始x坐标
 * @param y 起始y坐标
 * @param length 线段长度
 * @param color 线段颜色
 */
void Meow_TFT_DrawVLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    for (uint16_t i = 0; i < length; i++)
        Meow_TFT_DrawPixel(x, y + i, color);
}

/**
 * @brief 在屏幕上绘制矩形框
 *
 * @param x 起始x坐标
 * @param y 起始y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param color 矩形颜色
 */
void Meow_TFT_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    Meow_TFT_DrawHLine(x, y, w, color);
    Meow_TFT_DrawHLine(x, y + h - 1, w, color);
    Meow_TFT_DrawVLine(x, y, h, color);
    Meow_TFT_DrawVLine(x + w - 1, y, h, color);
}

/**
 * @brief 绘制一个UTF-8字符（len字节）
 *
 * @param x 起始x坐标
 * @param y 起始y坐标
 * @param utf8char 要绘制的UTF-8字符
 * @param len 字符的字节数
 * @param color 字符的颜色
 * @param bg 背景颜色
 * @param size 字符的大小（缩放倍数）
 */
void Meow_TFT_DrawCharUTF8(uint16_t x, uint16_t y,
                           const char *utf8char, int len,
                           uint16_t color, uint16_t bg, uint8_t size)
{
    if (!currentFont)
        return;

    // 在 charset 中按UTF-8长度匹配，找到索引
    int charIndex = -1;
    const char *p = currentFont->charset;
    for (int i = 0; i < currentFont->charCount; i++)
    {
        int clen = utf8_char_len(p);
        if (clen == len && strncmp(p, utf8char, len) == 0)
        {
            charIndex = i;
            break;
        }
        p += clen;
    }
    if (charIndex < 0)
        return;

    // 计算位数和字节数
    uint16_t totalBits = currentFont->width * currentFont->height;
    uint16_t totalBytes = (totalBits + 7) / 8;

    const uint8_t *charData = &((*currentFont->data)[charIndex * totalBytes]);

    // 按位绘制
    for (uint16_t bitPos = 0; bitPos < totalBits; bitPos++)
    {
        uint8_t row = bitPos / currentFont->width;
        uint8_t col = bitPos % currentFont->width;
        uint16_t byteIdx = bitPos / 8;
        uint8_t bitOff = 7 - (bitPos % 8);
        uint8_t value = (charData[byteIdx] >> bitOff) & 0x01;
        uint16_t drawColor = value ? color : bg;

        for (uint8_t sx = 0; sx < size; sx++)
        {
            for (uint8_t sy = 0; sy < size; sy++)
            {
                Meow_TFT_DrawPixel(
                    x + col * size + sx,
                    y + row * size + sy,
                    drawColor);
            }
        }
    }
}

/**
 * @brief 绘制字符串（支持UTF-8）
 *
 * @param x 起始x坐标
 * @param y 起始y坐标
 * @param str 要绘制的字符串
 * @param color 字符的颜色
 * @param bg 背景颜色
 * @param size 字符的大小（缩放倍数）
 */
void Meow_TFT_DrawString(uint16_t x, uint16_t y, const char *str,
                         uint16_t color, uint16_t bg, uint8_t size)
{
    while (*str)
    {
        int len = utf8_char_len(str);
        Meow_TFT_DrawCharUTF8(x, y, str, len, color, bg, size);
        x += currentFont->width * size + size;
        str += len;
    }
}

static uint16_t cursor_x = 0; ///< 当前光标x坐标
static uint16_t cursor_y = 0; ///< 当前光标y坐标

/**
 * @brief 设置光标位置
 *
 * @param x 光标的x坐标
 * @param y 光标的y坐标
 */
void Meow_TFT_SetCursor(uint16_t x, uint16_t y)
{
    cursor_x = x;
    cursor_y = y;
}

static uint16_t text_color = 0xFFFF; ///< 默认文字颜色（白色）
static uint16_t bg_color = 0x0000;   ///< 默认背景颜色（黑色）
static uint8_t text_size = 1;        ///< 默认文字大小

/**
 * @brief 设置文本的颜色
 *
 * @param color 文字颜色
 * @param bg 背景颜色
 */
void Meow_TFT_SetTextColor(uint16_t color, uint16_t bg)
{
    text_color = color;
    bg_color = bg;
}

/**
 * @brief 设置文本的大小
 *
 * @param size 文字大小
 */
void Meow_TFT_SetTextSize(uint8_t size)
{
    text_size = size;
}

/**
 * @brief 打印格式化的文本（支持UTF-8）
 *
 * @param format 格式化字符串
 */
void Meow_TFT_Printf(const char *format, ...)
{
    char buffer[128]; ///< 用于存储格式化后的字符串
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Meow_TFT_DrawString(cursor_x, cursor_y, buffer, text_color, bg_color, text_size);
    cursor_y += currentFont->height * text_size + text_size; ///< 更新光标位置
}
