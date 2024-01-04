#ifndef LSM6DSOX_DRV
#define LSM6DSOX_DRV

#include <stdint.h>

#include "esp_log.h"
#include "driver/i2c.h"

#include "lsm6dsox_reg.h"

#define I2C_MASTER_TIMEOUT_MS 1000



//Compulsory Funcs
int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len);

//Driver
int lsm_init(i2c_port_t i2c_port_number);

//Error Handling
int32_t handle_esp_err(char *TAG,esp_err_t err);


#endif //LSM6DSOX_DRV