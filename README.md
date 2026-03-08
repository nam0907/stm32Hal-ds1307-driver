# STM32 DS1307 Driver

Simple **DS1307 RTC driver for STM32** using HAL I2C.

This library allows STM32 microcontrollers to communicate with the DS1307 Real Time Clock module for reading and setting date/time.

---

## Features

* Read current time from DS1307
* Set date and time
* BCD ↔ Decimal conversion
* HAL I2C compatible
* Lightweight and easy to integrate

---

## Hardware

* MCU: STM32
* RTC: DS1307
* Interface: I2C

Typical connection:

| DS1307 | STM32   |
| ------ | ------- |
| VCC    | 5V      |
| GND    | GND     |
| SDA    | I2C SDA |
| SCL    | I2C SCL |

---

## File Structure

```
DS1307.c
DS1307.h
README.md
```

---

## Usage

### Include driver

```c
#include "DS1307.h"
```

### Initialize

```c
DS1307_TypeDef DS1307;

DS1307_INIT(&DS1307, &hi2c1);
```

### Read time

```c

DS1307_READ_RTC(&DS1307);
```

### Set time

```c

uint8_t hour = 12;
uint8_t minute = 30;
uint8_t second = 0;
uint8_t dayOfWeek = 5;
uint8_t day = 30;
uint8_t month = 3;
uint16_t year=2025;


uint8_t DS1307_WRITE_RTC(&DS1307, hour, minute, second, dayOfWeek, day, month, year);
```

---

## Example

```c
DS1307_TypeDef DS1307;

DS1307_GetTime(&DS1307);

printf("hh/mm/ss: %02d:%02d:%02d\n",DS1307.hour,DS1307.minute,DS1307.second);
```

---

## Supported Platforms

* STM32 HAL
* Tested on STM32F1 / STM32F4

---

## License

This project is open-source and free to use.

---

## Author

**Nam**
