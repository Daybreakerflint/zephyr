/*
 * Copyright (c) 2024 Gergo Vari <work@gergovari.com>
 * Copyright (c) 2026 Adrian Dummermuth
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_DS1621_DS1621_H_
#define ZEPHYR_DRIVERS_SENSOR_DS1621_DS1621_H_

#include <zephyr/sys/util.h>
#include <zephyr/drivers/i2c.h>

/* Temperature registers */
#define DS1621_REG_TEMP_MSB             0xAA
#define DS1621_REG_START_CONV_T         0xEE
#define DS1621_REG_STOP_CONV_T          0x22

/* Higher resolution temperature reading can be obtained by implementing this readout: */
/* (DS1621_CMD_READ_TEMP (16bit) >> 8) - 0.25 +  (DS1621_CMD_READ_SLOPE - DS1621_CMD_READ_CTR) / DS1621_CMD_READ_SLOPE */
#define DS1621_REG_CONFIG               0xAC
#define DS1621_REG_READ_CTR             0xA8
#define DS1621_REG_READ_SLOPE           0xA9
#define DS1621_REG_WRITE_TH             0xA1
#define DS1621_REG_WRITE_TL             0xA2

/* Temperature masks and shifts */
#define DS1621_BITS_RAW_TEMP_LSB        BIT(0) /* fractional portion */
#define DS1621_SHIFT_TEMP_LSB           7
#define DS1621_SHIFT_TEMP_MSB           1

/* Config bitmasks*/
#define DS1621_CONFIG_DONE              BIT(7)
#define DS1621_CONFIG_THF               BIT(6)
#define DS1621_CONFIG_TLF               BIT(5)
#define DS1621_CONFIG_NVB               BIT(4)
#define DS1621_CONFIG_POL               BIT(1)
#define DS1621_CONFIG_1SHOT             BIT(0)

struct ds1621_data {
	uint16_t sample;
};

struct ds1621_config {
	struct i2c_dt_spec i2c;
};

#endif /* ZEPHYR_DRIVERS_SENSOR_DS1621_DS1621_H_ */
