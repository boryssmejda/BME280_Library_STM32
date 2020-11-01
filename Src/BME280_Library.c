/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "BME280_Library.h"

#include <stdlib.h>
#include <assert.h>

struct bme280_handler_t
{
	I2C_HandleTypeDef *i2c_handler;
};

/** Functions ----------------------------------------------------------------*/
bme280_handler_t* bme280_init(I2C_HandleTypeDef *i2c_handler)
{
	bme280_handler_t *bme280 = malloc(sizeof(bme280_handler_t));

	if(bme280)
	{
		bme280->i2c_handler = i2c_handler;
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
