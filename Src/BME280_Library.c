/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "BME280_Library.h"

struct bme280_handler_t
{
	I2C_HandleTypeDef *i2c_handler;
};

/** Functions ----------------------------------------------------------------*/
uint8_t BME280_getChipID()
{
	return 0;
}

bme280_handler_t* bme280_init(I2C_HandleTypeDef *i2c_handler)
{
	bme280_handler_t *bme280 = malloc(sizeof(bme280_handler_t));

	if(bme280)
	{
		bme280->i2c_handler = i2c_handler;
	}

	return bme280;
}

bme280_handler_t* bme280_destroy(bme280_handler_t* bme280_handler)
{
	if(bme280_handler)
	{
		free(bme280_handler);
	}
}
