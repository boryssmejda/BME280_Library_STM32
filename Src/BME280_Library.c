/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "BME280_Library.h"

#include <stdlib.h>
#include <assert.h>


BME280_S32_t t_fine;
uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;

struct bme280_handler_t
{
	I2C_HandleTypeDef *i2c_handler;
	bme280_control_registers ctrlRegs;
};

/** Functions ----------------------------------------------------------------*/
bme280_handler_t* bme280_init(I2C_HandleTypeDef *i2c_handler, bme280_control_registers ctrlRegs)
{
	bme280_handler_t *bme280 = malloc(sizeof(bme280_handler_t));

	if(bme280 == NULL)
	{
		return NULL;
	}

	bme280->i2c_handler = i2c_handler;
	bme280->ctrlRegs = ctrlRegs;

	uint8_t receivedID = 0;
	if(BME280_getChipID(bme280, &receivedID, DEFAULT_TIMEOUT) == BME280_FAIL)
	{
		bme280_destroy(bme280);
		return NULL;
	}

	if(BME280_CHIP_ID_VALUE != receivedID)
	{
		bme280_destroy(bme280);
		return NULL;
	}

	if(BME280_resetChip(bme280, DEFAULT_TIMEOUT) == BME280_FAIL)
	{
		bme280_destroy(bme280);
		return NULL;
	}

	if(BME280_setControlRegisters(bme280, ctrlRegs) == BME280_FAIL)
	{
		bme280_destroy(bme280);
		return NULL;
	}

	return bme280;
}

void bme280_destroy(bme280_handler_t* bme280_handler)
{
	if(bme280_handler)
	{
		free(bme280_handler);
	}
}

BME280_STATUS BME280_setControlRegisters(bme280_handler_t* bme280_handler, bme280_control_registers controlReg)
{
	assert(bme280_handler);

	HAL_StatusTypeDef retVal;
	retVal = HAL_I2C_Master_Transmit(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, (uint8_t*)&controlReg, sizeof(controlReg), DEFAULT_TIMEOUT);

	if(retVal != HAL_OK)
	{
		return BME280_FAIL;
	}

	return BME280_OK;
}


BME280_STATUS BME280_getChipID(bme280_handler_t* bme280_handler, uint8_t* chipID, uint32_t timeout)
{
	assert(bme280_handler);
	assert(chipID);
	assert(timeout > 0);

	HAL_StatusTypeDef retVal;
	uint8_t chipIDRegisterAddress = BME280_REGISTER_ADDRESS_CHIP_ID;
	retVal = HAL_I2C_Master_Transmit(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, &chipIDRegisterAddress, sizeof(chipIDRegisterAddress), timeout);
	if(retVal != HAL_OK)
	{
		return BME280_FAIL;
	}

	retVal = HAL_I2C_Master_Receive(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, chipID, 1, timeout);
	if(retVal == HAL_OK)
	{
		return BME280_OK;
	}
	else
	{
		return BME280_FAIL;
	}
}

BME280_STATUS BME280_resetChip(bme280_handler_t* bme280_handler, uint32_t timeout)
{
	assert(bme280_handler);
	assert(timeout > 0);

	bme280_register_frame dataFrame;
	dataFrame.registerAddress = BME280_REGISTER_ADDRESS_RESET;
	dataFrame.registerData = BME280_RESET_REGISTER_VALUE;

	HAL_StatusTypeDef retVal;
	retVal = HAL_I2C_Master_Transmit(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, (uint8_t*)&dataFrame, sizeof(dataFrame), timeout);
	if(retVal == HAL_OK)
	{
		return BME280_OK;
	}
	else
	{
		return BME280_FAIL;
	}
}

BME280_STATUS BME280_getTemperatureCompensationParameters(bme280_handler_t* bme280_handler, bme280_registers_compensation_parameters_temperature* compTemp)
{
	uint8_t values[6];
	uint8_t compTempAddress = 0x88;

	HAL_StatusTypeDef retVal;
	retVal = HAL_I2C_Master_Transmit(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, &compTempAddress, sizeof(compTempAddress), 1000);
	if(retVal != HAL_OK)
	{
		return BME280_FAIL;
	}

	retVal = HAL_I2C_Master_Receive(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, values, sizeof(values), 1000);
	if(retVal != HAL_OK)
	{
		return BME280_FAIL;
	}

	dig_T1 = 0;
	dig_T1 |= ((uint16_t)values[1]) << 8;
	dig_T1 |= (uint16_t)values[0];

	dig_T2 = 0;
	dig_T2 |= ((uint16_t)values[3]) << 8;
	dig_T2 |= (uint16_t)values[2];

	dig_T3 = 0;
	dig_T3 |= ((uint16_t)values[5]) << 8;
	dig_T3 |= (uint16_t)values[4];

	compTemp->dig_T1 = dig_T1;
	compTemp->dig_T2 = dig_T2;
	compTemp->dig_T3 = dig_T3;

	return BME280_OK;
}

BME280_STATUS BME280_readADCTemperature(bme280_handler_t* bme280_handler, BME280_S32_t *adc_T, uint32_t timeout)
{
	assert(bme280_handler);
	assert(adc_T);

	uint8_t values[8];
	uint8_t tempRegisterDeviceAddress = BME280_REGISTER_ADDRESS_PRESS_MSB;


	HAL_StatusTypeDef retVal;
	retVal = HAL_I2C_Master_Transmit(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, &tempRegisterDeviceAddress, sizeof(tempRegisterDeviceAddress), timeout);

	if(retVal != HAL_OK)
	{
		return BME280_FAIL;
	}

	retVal = HAL_I2C_Master_Receive(bme280_handler->i2c_handler, BME280_DEVICE_ADDRESS, values, sizeof(values), timeout);

	if(retVal != HAL_OK)
	{
		return BME280_FAIL;
	}
	else
	{
		*adc_T = 0;
		*adc_T |= ((uint32_t)values[0]) << 12;
		*adc_T |= ((uint32_t)values[1]) << 4;
		*adc_T |= (uint32_t)values[2] >> 4;

		uint32_t adc_pressure = 0;
		adc_pressure |= ((uint32_t)values[3]) << 12;
		adc_pressure |= ((uint32_t)values[4]) << 4;
		adc_pressure |= ((uint32_t)values[5]) >> 4;

		uint32_t adc_humidity = 0;
		adc_humidity |= ((uint32_t)values[6]) << 8;
		adc_humidity |= (uint32_t)values[7];
		return BME280_OK;
	}
}

BME280_S32_t BME280_convertTempIntoReadable(BME280_S32_t adc_T)
{
	BME280_S32_t var1, var2, T;

	//var1 = (adc_T >> 3) - ((BME280_S32_t)dig_T1 << 1);
	//var1 = (var1 * ((BME280_S32_t)dig_T2)) >> 11;

	//var2 = (adc_T >> 4) - ((BME280_S32_t)dig_T1);
	//var2 = ((var2 * var2) >> 12) * (((BME280_S32_t)dig_T3) >> 14);

	//t_fine = var1 + var2;
	//T = (t_fine * 5 + 128) >> 8;

	var1 = (int32_t)((adc_T / 8) - ((int32_t)dig_T1 * 2));
	var1 = (var1 * ((int32_t)dig_T2)) / 2048;
	var2 = (int32_t)((adc_T / 16) - ((int32_t)dig_T1));
	var2 = (((var2 * var2) / 4096) * ((int32_t)dig_T3)) / 16384;
	t_fine = var1 + var2;

	T = (t_fine * 5 + 128) / 256;

	return T;
}
