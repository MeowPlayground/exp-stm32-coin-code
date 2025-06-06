#include "meow_pcf8563.h"
#include <stdio.h>

#define PCF8563_ADDR (0xA2 >> 1)

#define REG_CTRL1 0x00
#define REG_SECONDS 0x02
#define REG_MINUTES 0x03
#define REG_HOURS 0x04
#define REG_DAYS 0x05
#define REG_WEEKDAYS 0x06
#define REG_MONTHS 0x07
#define REG_YEARS 0x08

static I2C_HandleTypeDef *meow_i2c = NULL;

static uint8_t BCD_To_BIN(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

static uint8_t BIN_To_BCD(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

bool Meow_PCF8563_Init(I2C_HandleTypeDef *hi2c)
{
    meow_i2c = hi2c;
    uint8_t data[2] = {REG_CTRL1, 0x00};
    return HAL_I2C_Master_Transmit(meow_i2c, PCF8563_ADDR << 1, data, 2, 100) == HAL_OK;
}

bool Meow_PCF8563_ReadTime(Meow_PCF8563_DateTime *dt)
{
    if (!meow_i2c || !dt)
        return false;

    uint8_t reg = REG_SECONDS;
    uint8_t buf[7];

    if (HAL_I2C_Master_Transmit(meow_i2c, PCF8563_ADDR << 1, &reg, 1, 100) != HAL_OK)
        return false;
    if (HAL_I2C_Master_Receive(meow_i2c, PCF8563_ADDR << 1, buf, 7, 100) != HAL_OK)
        return false;

    dt->second = BCD_To_BIN(buf[0] & 0x7F);
    dt->minute = BCD_To_BIN(buf[1] & 0x7F);
    dt->hour = BCD_To_BIN(buf[2] & 0x3F);
    dt->day = BCD_To_BIN(buf[3] & 0x3F);
    dt->week = BCD_To_BIN(buf[4] & 0x07);
    dt->month = BCD_To_BIN(buf[5] & 0x1F);
    dt->year = BCD_To_BIN(buf[6]);

    return true;
}

bool Meow_PCF8563_SetTime(const Meow_PCF8563_DateTime *dt)
{
    if (!meow_i2c || !dt)
        return false;

    uint8_t buf[8];
    buf[0] = REG_SECONDS;
    buf[1] = BIN_To_BCD(dt->second & 0x7F);
    buf[2] = BIN_To_BCD(dt->minute & 0x7F);
    buf[3] = BIN_To_BCD(dt->hour & 0x3F);
    buf[4] = BIN_To_BCD(dt->day & 0x3F);
    buf[5] = BIN_To_BCD(dt->week & 0x07);
    buf[6] = BIN_To_BCD(dt->month & 0x1F);
    buf[7] = BIN_To_BCD(dt->year);

    return HAL_I2C_Master_Transmit(meow_i2c, PCF8563_ADDR << 1, buf, 8, 100) == HAL_OK;
}

bool Meow_PCF8563_SetDateOnly(uint8_t year, uint8_t month, uint8_t day)
{
    if (!meow_i2c)
        return false;

    Meow_PCF8563_DateTime current;
    if (!Meow_PCF8563_ReadTime(&current))
        return false;

    current.year = year;
    current.month = month;
    current.day = day;

    return Meow_PCF8563_SetTime(&current);
}

bool Meow_PCF8563_UpdateDate(const Meow_PCF8563_DateTime *new_date_part)
{
    if (!meow_i2c || !new_date_part)
        return false;

    Meow_PCF8563_DateTime current;
    if (!Meow_PCF8563_ReadTime(&current))
        return false;

    // Update year, month, and day from new_date_part
    if (new_date_part->year != 0)
        current.year = new_date_part->year;
    if (new_date_part->month != 0)
        current.month = new_date_part->month;
    if (new_date_part->day != 0)
        current.day = new_date_part->day;

    return Meow_PCF8563_SetTime(&current);
}

void Meow_PCF8563_FormatDateTime(const Meow_PCF8563_DateTime *dt, char *buf, int bufsize)
{
    snprintf(buf, bufsize, "20%02d-%02d-%02d %02d:%02d:%02d",
             dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second);
}

void Meow_PCF8563_FormatDate(const Meow_PCF8563_DateTime *dt, char *buf, int bufsize)
{
    snprintf(buf, bufsize, "20%02d-%02d-%02d", dt->year, dt->month, dt->day);
}

void Meow_PCF8563_FormatTime(const Meow_PCF8563_DateTime *dt, char *buf, int bufsize)
{
    snprintf(buf, bufsize, "%02d:%02d:%02d", dt->hour, dt->minute, dt->second);
}
