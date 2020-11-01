#ifndef _BME280_LIBRARY__
#define _BME280_LIBRARY__

typedef struct bme280_handler_t bme280_handler_t;

#define BME280_DEVICE_ADDRESS (0x76<<1)
#define BME280_CHIP_ID_REGISTER_ADDRESS 0xD0
#define BME280_CHIP_ID_VALUE 0x60 // to check if everything functions correctly
#define BME280_RESET_REGISTER_ADDRESS 0xE0

bme280_handler_t* bme280_init(I2C_HandleTypeDef *i2c_handler);
bme280_handler_t* bme280_destroy(bme280_handler_t* bme280_handler);

#endif
