#include "user.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "meow_tft.h"
#include "meow_tft_ui.h"
#include "meow_font.h"
#include "meow_pcf8563.h"

void user_loop(void)
{
    // Your user code here

    Meow_PCF8563_DateTime dt;
    Meow_PCF8563_ReadTime(&dt); // 读取时间
    char time_str[20];
    Meow_PCF8563_FormatDateTime(&dt, time_str, sizeof(time_str)); // 格式化时间字符串

    // Meow_TFT_SetCursor(0, 65);                 // 设置光标位置
    // Meow_TFT_SetTextColor(0x0000, 0xFFFF);     // 设置文字颜色和背景颜色
    // Meow_TFT_SetFont(&meow_font_cn_20_normal); // 设置字体
    // Meow_TFT_Printf("当前时间");               // 打印当前日期
    Meow_TFT_SetTextColor(0x2222, 0xFFFF); // 设置文字颜色和背景颜色
    Meow_TFT_SetCursor(34, 65);            // 设置光标位置
    Meow_TFT_SetFont(&meow_font_num_14);   // 设置字体
    Meow_TFT_Printf("%s", time_str);       // 打印当前时间

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Toggle the LED on PC13    HAL_Delay(1000); // Delay for 1 second
    HAL_Delay(1000);
}

void user_setup(void)
{
    Meow_TFT_Init();             // 初始化屏幕
    Meow_TFT_FillScreen(0xFFFF); // 黑色背景
    Meow_PCF8563_Init(&hi2c1);   // 初始化 RTC

    Meow_TFT_SetFont(&meow_font_cn_24);     // 设置字体
    Meow_TFT_SetCursor(45, 30);             // 设置光标位置
    Meow_TFT_SetTextColor(0x0000, 0xFFFF);  // 设置文字颜色和背景颜色
    Meow_TFT_Printf("钱币存储系统");        // 打印字符串
    Meow_TFT_DrawHLine(0, 60, 240, 0x0000); // 绘制横线
}