/* ds1621.c - Driver for Maxim DS1621 temperature sensor */

/*
 * Copyright (c) 2026 Adrian Dummermuth
 * 
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT maxim_ds1621

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/logging/log.h>

#include "ds1621.h"

LOG_MODULE_REGISTER(DS1621, CONFIG_SENSOR_LOG_LEVEL);

static int ds1621_start_conv(const struct device *dev)
{
	const struct ds1621_config *cfg = dev->config;
	uint8_t tx_buf = DS1621_REG_STOP_CONV_T;

	return i2c_write_dt(&cfg->i2c, &tx_buf, sizeof(tx_buf));
}

static int ds1621_write_reg(const struct device *dev, uint8_t reg, uint8_t *val)
{
	const struct ds1621_config *config = dev->config;
	uint8_t tx_buf[2] = {reg, *val};

	return i2c_write_dt(&config->i2c, tx_buf, sizeof(tx_buf));
}

static int ds1621_read_reg(const struct device *dev, uint8_t reg, uint8_t *val)
{
	const struct ds1621_config *cfg = dev->config;

	return i2c_write_read_dt(&cfg->i2c, &reg, sizeof(reg), val, sizeof(val));
}

static int ds1621_read_temp(const struct device *dev, uint16_t *raw_temp)
{
	const struct ds1621_config *cfg = dev->config;
	uint8_t reg_addr = DS1621_REG_TEMP_MSB;
	uint8_t rx_buf[2];
	int rc;

	rc = i2c_write_read_dt(&cfg->i2c, &reg_addr, sizeof(reg_addr), rx_buf, sizeof(rx_buf));

	*raw_temp = ((uint16_t)(rx_buf[0] << DS1621_SHIFT_TEMP_MSB) | (rx_buf[1] >> DS1621_SHIFT_TEMP_LSB));

	return rc;
}

static int ds1621_sample_fetch(const struct device *dev,
				 enum sensor_channel chan)
{
	struct ds1621_data *drv_data = dev->data;

	__ASSERT_NO_MSG(chan == SENSOR_CHAN_ALL || chan == SENSOR_CHAN_AMBIENT_TEMP);

	drv_data->sample = 0U;

	uint8_t reg_config = 0x00;

	if (ds1621_read_reg(dev, DS1621_REG_CONFIG, &reg_config))
	{
		return -EIO;
	}

	if (reg_config & DS1621_CONFIG_DONE)
	{
		return ds1621_read_temp(dev, &drv_data->sample);
	}
	else
	{
		if (reg_config & DS1621_CONFIG_1SHOT) {
			reg_config = reg_config | DS1621_CONFIG_1SHOT;
			if (ds1621_write_reg(dev, DS1621_REG_CONFIG, &reg_config)) {
				return -EIO;
			}
		}

		return ds1621_start_conv(dev);
	}
}

static int ds1621_channel_get(const struct device *dev,
				enum sensor_channel chan,
				struct sensor_value *val)
{
	struct ds1621_data *drv_data = dev->data;

	if (chan != SENSOR_CHAN_AMBIENT_TEMP) {
		return -ENOTSUP;
	}

	/* Temperature resolution is 0.5°C */
	uint16_t raw_temp = drv_data->sample;

	val->val1 = (int8_t)(raw_temp) >> DS1621_SHIFT_TEMP_MSB;
	val->val2 = (raw_temp & DS1621_BITS_RAW_TEMP_LSB) ? 500000 : 0;

	return 0;
}

static DEVICE_API(sensor, ds1621_driver_api) = {
	.sample_fetch = ds1621_sample_fetch,
	.channel_get = ds1621_channel_get,
};

int ds1621_init(const struct device *dev)
{
	const struct ds1621_config *config = dev->config;

	if (!device_is_ready(config->i2c.bus)) {
		LOG_ERR_DEVICE_NOT_READY(config->i2c.bus);
		return -ENODEV;
	}

	return 0;
}

#define DS1621_DEFINE(inst)													\
	static struct ds1621_data ds1621_data_##inst;							\
																			\
	static const struct ds1621_config ds1621_config_##inst = {				\
		.i2c = I2C_DT_SPEC_INST_GET(inst),									\
	};																		\
																			\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, ds1621_init, NULL,					\
			      &ds1621_data_##inst, &ds1621_config_##inst, POST_KERNEL,	\
			      CONFIG_SENSOR_INIT_PRIORITY, &ds1621_driver_api);			\

DT_INST_FOREACH_STATUS_OKAY(DS1621_DEFINE)