#ifndef __MEOW_PCF8563_H
#define __MEOW_PCF8563_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t year;   // 0-99
    uint8_t month;  // 1-12
    uint8_t day;    // 1-31
    uint8_t hour;   // 0-23
    uint8_t minute; // 0-59
    uint8_t second; // 0-59
    uint8_t week;   // 0 = Sunday, 6 = Saturday
} Meow_PCF8563_DateTime;

bool Meow_PCF8563_Init(I2C_HandleTypeDef *hi2c);
bool Meow_PCF8563_SetTime(const Meow_PCF8563_DateTime *dt);
bool Meow_PCF8563_SetDateOnly(uint8_t year, uint8_t month, uint8_t day);
bool Meow_PCF8563_ReadTime(Meow_PCF8563_DateTime *dt);

void Meow_PCF8563_FormatDateTime(const Meow_PCF8563_DateTime *dt, char *buf, int bufsize);
void Meow_PCF8563_FormatDate(const Meow_PCF8563_DateTime *dt, char *buf, int bufsize);
void Meow_PCF8563_FormatTime(const Meow_PCF8563_DateTime *dt, char *buf, int bufsize);
bool Meow_PCF8563_UpdateDate(const Meow_PCF8563_DateTime *new_date_part);

#endif // __MEOW_PCF8563_H
