#ifndef _BME280_LIBRARY__
#define _BME280_LIBRARY__

#define DEFAULT_TIMEOUT 				100
#define BME280_RW_READ 					1
#define BME280_RW_WRITE 				0
#define BME280_DEVICE_ADDRESS 			(0x76<<1)
#define BME280_CHIP_ID_VALUE 			0x60 // to check if everything functions correctly
#define BME280_RESET_REGISTER_VALUE 	0xB6


// Table 21 Register 0xF3 “status”
#define BME280_STATUS_MEASURING_RUNNING					1
#define BME280_STATUS_MEASURING_TRANSFERRED 			0
#define BME280_STATUS_IM_UPDATE_COPYING_IN_PROGRESS		1
#define BME280_STATUS_IM_UPDATE_COPYING_DONE			0

// Table 25  Register Settings Mode
#define BME280_OPERATION_MODE_SLEEP		0
#define BME280_OPERATION_MODE_FORCED_1	1
#define BME280_OPERATION_MODE_FORCED_2	2
#define BME280_OPERATION_MODE_NORMAL	3

// Oversampling definitions
#define BME280_OVERSAMPLING_1 	1
#define BME280_OVERSAMPLING_2 	2
#define BME280_OVERSAMPLING_4 	3
#define BME280_OVERSAMPLING_8 	4
#define BME280_OVERSAMPLING_16 	5

// Table 27 t_sb definitions - t standby
#define BME280_STANDBY_0_5		0
#define BME280_STANDBY_62_5		1
#define BME280_STANDBY_125		2
#define BME280_STANDBY_250		3
#define BME280_STANDBY_500		4
#define BME280_STANDBY_1000		5
#define BME280_STANDBY_10		6
#define BME280_STANDBY_20		7

// Table 28 Filter Settings
#define BME280_FILTER_OFF				0
#define BME280_FILTER_COEFFICIENT_2		1
#define BME280_FILTER_COEFFICIENT_4		2
#define BME280_FILTER_COEFFICIENT_8		3
#define BME280_FILTER_COEFFICIENT_16	4


#define BME280_REGISTER_ADDRESS_CHIP_ID 	0xD0
#define BME280_REGISTER_ADDRESS_RESET 		0xE0
#define BME280_REGISTER_ADDRESS_CTRL_HUM 	0xF2
#define BME280_REGISTER_ADDRESS_STATUS 		0xF3
#define BME280_REGISTER_ADDRESS_CTRL_MEAS 	0xF4
#define BME280_REGISTER_ADDRESS_CONFIG		0xF5
#define BME280_REGISTER_ADDRESS_PRESS_MSB	0xF7
#define BME280_REGISTER_ADDRESS_PRESS_LSB	0xF8
#define BME280_REGISTER_ADDRESS_PRESS_XLSB	0xF9
#define BME280_REGISTER_ADDRESS_TEMP_MSB	0xFA
#define BME280_REGISTER_ADDRESS_TEMP_LSB	0xFB
#define BME280_REGISTER_ADDRESS_TEMP_XLSB	0xFC
#define BME280_REGISTER_ADDRESS_HUM_MSB		0xFD
#define BME280_REGISTER_ADDRESS_HUM_LSB		0xFE



enum BME280_STATUS
{
	BME280_OK,
	BME280_FAIL
};

typedef struct bme280_register_frame
{
	uint8_t registerAddress;
	uint8_t registerData;
}bme280_register_frame;

typedef struct bme280_register_ctrl_hum
{
	uint8_t registerAddress;
	uint8_t reserved: 	5; // do not touch this register
	uint8_t osrs_h:		3;
}bme280_register_ctrl_hum;

typedef struct bme280_register_status
{
	uint8_t reserved_from_bit_7_to_4: 	4;
	uint8_t measuring: 					1;
	uint8_t reserved_from_bit_2_to_1:	2;
	uint8_t im_update:					1;
}bme280_register_status;

typedef struct bme280_register_ctrl_meas
{
	uint8_t registerAddress;
	uint8_t osrs_t:	3;
	uint8_t osrs_p: 3;
	uint8_t mode:	2;
}bme280_register_ctrl_meas;

typedef struct bme280_register_config
{
	uint8_t registerAddress;
	uint8_t t_sb: 			3;
	uint8_t filter: 		3;
	uint8_t reserved_bit_1: 1;
	uint8_t spi3w_en: 		1;
}bme280_register_config;

typedef struct bme280_register_press_msb
{
	uint8_t press_msb;
}bme280_register_press_msb;

typedef struct bme280_register_press_lsb
{
	uint8_t press_lsb;
}bme280_register_press_lsb;

typedef struct bme280_register_press_xlsb
{
	uint8_t press_xlsb: 				4;
	uint8_t reserved_from_bit_3_to_0: 	4;
}bme280_register_press_xlsb;

typedef struct bme280_register_temp_msb
{
	uint8_t temp_msb;
}bme280_register_temp_msb;

typedef struct bme280_register_temp_lsb
{
	uint8_t temp_lsb;
}bme280_register_temp_lsb;

typedef struct bme280_register_temp_xlsb
{
	uint8_t temp_xlsb: 				  	4;
	uint8_t reserved_from_bit_3_to_0: 	4;
}bme280_register_temp_xlsb;

typedef struct bme280_register_hum_msb
{
	uint8_t hum_msb;
}bme280_register_hum_msb;

typedef struct bme280_register_hum_lsb
{
	uint8_t hum_lsb;
}bme280_register_hum_lsb;

typedef struct bme280_control_registers
{
	bme280_register_ctrl_hum	ctrlHum;
	bme280_register_ctrl_meas 	ctrlMeas;
	bme280_register_config 		config;
}bme280_control_registers;

typedef struct bme280_registers_pressure
{
	uint8_t registerAddress;
	bme280_register_press_msb 	pressMsb;
	bme280_register_press_lsb 	pressLsb;
	bme280_register_press_xlsb 	pressXlsb;
}bme280_registers_pressure;

typedef struct
{
	bme280_register_temp_msb 	tempMsb;
	bme280_register_temp_lsb	tempLsb;
	bme280_register_temp_xlsb	tempXlsb;
}bme280_registers_temperature;

typedef struct //bme280_registers_humidity
{
	uint8_t registerAddress;
	bme280_register_hum_msb 	humMsb;
	bme280_register_hum_lsb		humLsb;
}bme280_registers_humidity;

typedef struct //bme280_registers_compensation_parameters_temperature
{
	uint8_t registerAddress;

	uint8_t dig_T1_lsb;
	uint8_t dig_T1_msb;

	uint8_t dig_T2_lsb;
	uint8_t dig_T2_msb;

	uint8_t dig_T3_lsb;
	uint8_t dig_T3_msb;
}bme280_registers_compensation_parameters_temperature;


typedef enum BME280_STATUS BME280_STATUS;
typedef struct bme280_handler_t bme280_handler_t;
typedef int32_t BME280_S32_t;

bme280_handler_t* bme280_init(I2C_HandleTypeDef *i2c_handler);
void bme280_destroy(bme280_handler_t* bme280_handler);

BME280_STATUS BME280_getChipID(bme280_handler_t* bme280_handler, uint8_t* chipID, uint32_t timeout);
BME280_STATUS BME280_resetChip(bme280_handler_t* bme280_handler, uint32_t timeout);

BME280_STATUS BME280_getTemperatureCompensationParameters(bme280_handler_t* bme280_handler, bme280_registers_compensation_parameters_temperature* compTermp);
BME280_STATUS BME280_readADCTemperature(bme280_handler_t* bme280_handler, BME280_S32_t *adc_T, uint32_t timeout);
BME280_S32_t BME280_convertTempIntoReadable(BME280_S32_t adc_T);

#endif
