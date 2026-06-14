/*
 * Copyright (c) 2026 Adrian Dummermuth
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_INA4235_INA4235_H_
#define ZEPHYR_DRIVERS_SENSOR_INA4235_INA4235_H_

/* Device register addresses */
/* Unique registers */
#define INA4235_REG_CONFIG1             0x20
#define INA4235_REG_CONFIG2             0x21
#define INA4235_REG_FLAGS               0x22
#define INA4235_REG_MANUFACTURER_ID     0x7E
#define INA4235_REG_DEVICE_ID           0x7F

/* Channel registers */
#define INA4235_REG_SHUNT_VOLTAGE       0x00
#define INA4235_REG_BUS_VOLTAGE         0x01
#define INA4235_REG_CURRENT             0x02
#define INA4235_REG_POWER               0x03
#define INA4235_REG_ENERGY              0x04 // 32 bit register!
#define INA4235_REG_CALIBRATION         0x05
#define INA4235_REG_ALERT_LIMIT         0x06
#define INA4235_REG_ALERT_CONFIG        0x07

/* Channel offsets */
#define INA4235_CH1                     0x00
#define INA4235_CH2                     0x08
#define INA4235_CH3                     0x10
#define INA4235_CH4                     0x18

/* Alert configuration masks */
#define INA4235_ALERT_CONFIG_CH1        0x00
#define INA4235_ALERT_CONFIG_CH2        0x08
#define INA4235_ALERT_CONFIG_CH3        0x10
#define INA4235_ALERT_CONFIG_CH4        0x18
#define INA4235_ALERT_CONFIG_SOL        0x01
#define INA4235_ALERT_CONFIG_SUL        0x02
#define INA4235_ALERT_CONFIG_BOL        0x03
#define INA4235_ALERT_CONFIG_BUL        0x04
#define INA4235_ALERT_CONFIG_POL        0x05

/* Device configuration */
#define INA4235_CONFIG1_ALL_CH_ACTIVE   0xF000
#define INA4235_CONFIG1_MASK_CH1_ACTIVE 0x1000
#define INA4235_CONFIG1_MASK_CH2_ACTIVE 0x2000
#define INA4235_CONFIG1_MASK_CH3_ACTIVE 0x4000
#define INA4235_CONFIG1_MASK_CH4_ACTIVE 0x8000
#define INA4235_CONFIG1_AVG_1           0x0000
#define INA4235_CONFIG1_AVG_4           0x0200
#define INA4235_CONFIG1_AVG_16          0x0400
#define INA4235_CONFIG1_AVG_64          0x0600
#define INA4235_CONFIG1_AVG_128         0x0800
#define INA4235_CONFIG1_AVG_256         0x0A00
#define INA4235_CONFIG1_AVG_512         0x0C00
#define INA4235_CONFIG1_AVG_1024        0x0E00
#define INA4235_CONFIG1_VBUSCT_140US    0x0000
#define INA4235_CONFIG1_VBUSCT_204US    0x0040
#define INA4235_CONFIG1_VBUSCT_332US    0x0080
#define INA4235_CONFIG1_VBUSCT_588US    0x00C0
#define INA4235_CONFIG1_VBUSCT_1100US   0x0100
#define INA4235_CONFIG1_VBUSCT_2116US   0x0140
#define INA4235_CONFIG1_VBUSCT_4156US   0x0180
#define INA4235_CONFIG1_VBUSCT_8244US   0x01C0
#define INA4235_CONFIG1_VSHCT_140US     0x0000
#define INA4235_CONFIG1_VSHCT_204US     0x0008
#define INA4235_CONFIG1_VSHCT_332US     0x0010
#define INA4235_CONFIG1_VSHCT_588US     0x0018
#define INA4235_CONFIG1_VSHCT_1100US    0x0020
#define INA4235_CONFIG1_VSHCT_2116US    0x0028
#define INA4235_CONFIG1_VSHCT_4156US    0x0030
#define INA4235_CONFIG1_VSHCT_8244US    0x0038
#define INA4235_CONFIG1_MODE_SHUTDOWN   0x0000
#define INA4235_CONFIG1_MODE_DEFAULT    0x0007

#define INA4235_CONFIG2_RESET           0x8000
#define INA4235_CONFIG2_ACC_RST         0x0F00
#define INA4235_CONFIG2_ACC_RST_CH1     0x0100
#define INA4235_CONFIG2_ACC_RST_CH2     0x0200
#define INA4235_CONFIG2_ACC_RST_CH3     0x0400
#define INA4235_CONFIG2_ACC_RST_CH4     0x0800
#define INA4235_CONFIG2_CNVR_MASK       0x0080
#define INA4235_CONFIG2_ENOF_MASK       0x0040
#define INA4235_CONFIG2_ALERT_LATCH     0x0020
#define INA4235_CONFIG2_ALERT_POL       0x0010
#define INA4235_CONFIG2_RANGE           0x000F
#define INA4235_CONFIG2_RANGE_CH1       0x0001
#define INA4235_CONFIG2_RANGE_CH2       0x0002
#define INA4235_CONFIG2_RANGE_CH3       0x0004
#define INA4235_CONFIG2_RANGE_CH4       0x0008

/* Device flags */
#define INA4235_FLAGS_LIMIT4_ALERT      0x8000
#define INA4235_FLAGS_LIMIT3_ALERT      0x4000
#define INA4235_FLAGS_LIMIT2_ALERT      0x2000
#define INA4235_FLAGS_LIMIT1_ALERT      0x1000
#define INA4235_FLAGS_ENERGYOF          0x0F00
#define INA4235_FLAGS_ENERGYOF_CH4      0x0800
#define INA4235_FLAGS_ENERGYOF_CH3      0x0400
#define INA4235_FLAGS_ENERGYOF_CH2      0x0200
#define INA4235_FLAGS_ENERGYOF_CH1      0x0100
#define INA4345_FLAGS_CVRF              0x0080
#define INA4345_FLAGS_OVF               0x0040

/* Device identification */
#define INA4235_MANUFACTURER_ID         0x5449
#define INA4235_DEVICE_ID_DIE_ID        0x4350
#define INA4235_DEVICE_ID_DIE_ID_MASK   0xFFF0
#define INA4235_DEVICE_ID_REV_ID_MASK   0x000F

static int ina4235_init(const struct device *dev);

struct ina4235_config {
    struct i2c_dt_spec bus;
    uint16_t config1;
    uint16_t config2;
    uint16_t flags;
};


#define INA4235_REG_SHUNT_VOLTAGE       0x00
#define INA4235_REG_BUS_VOLTAGE         0x01
#define INA4235_REG_CURRENT             0x02
#define INA4235_REG_POWER               0x03
#define INA4235_REG_ENERGY              0x04 // 32 bit register!
#define INA4235_REG_CALIBRATION         0x05

struct ina4235_channel {
    int16_t shunt_voltage;
    uint16_t bus_voltage;
    int16_t current;
    uint16_t power;
    uint32_t energy;
    uint16_t calibration;
};




/* Others */
#define INA219_SIGN_BIT(x)	((x) >> 15) & 0x1
#define INA219_V_BUS_MUL	0.004
#define INA219_SI_MUL		0.00001
#define INA219_POWER_MUL	20
#define INA219_WAIT_STARTUP	40
#define INA219_WAIT_MSR_RETRY	100
#define INA219_SCALING_FACTOR	4096000

struct ina219_config {
	struct i2c_dt_spec bus;
	uint16_t current_lsb;
	uint16_t r_shunt;
	uint8_t brng;
	uint8_t pg;
	uint8_t badc;
	uint8_t sadc;
	uint8_t mode;
};

struct ina219_data {
	uint16_t config;
	uint16_t v_bus;
	uint16_t power;
	uint16_t current;
	uint16_t calib;
	uint32_t msr_delay;
};

static int ina219_init(const struct device *dev);

#endif /* ZEPHYR_DRIVERS_SENSOR_INA4235_INA4235_H_ */