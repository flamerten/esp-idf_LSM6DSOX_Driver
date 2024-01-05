/**
  ******************************************************************************
  * @file    lsm6dsox_driver.h
  * @author  Samuel Yow
  * @brief   This file contains all the functions prototypes for the
  *          lsm6dsox_driver.c driver for ESP-iDF, based on ST's lsm6dsox_reg.c 
  *          PID driver https://github.com/STMicroelectronics/lsm6dsox-pid.
  ******************************************************************************
*/
#ifndef LSM6DSOX_DRV
#define LSM6DSOX_DRV

#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"

#include "esp_log.h"
#include "driver/i2c.h"

#include "lsm6dsox_reg.h"

#define I2C_MASTER_TIMEOUT_MS 1000

typedef struct
{
    i2c_port_t i2c_num;
    uint8_t LSM_SA;
    uint8_t LSM_8bit_addr;

    lsm6dsox_odr_xl_t Acc_Data_Rate;
    lsm6dsox_fs_xl_t Acc_Scale;

    lsm6dsox_odr_g_t Gyr_Data_Rate;
    lsm6dsox_fs_g_t Gyr_Scale;

}LSM_DriverConfig;


//Compulsory Funcs
int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len);

//Driver
int lsm_init(LSM_DriverConfig *sensor_config);

//Error Handling
int32_t handle_esp_err(char *TAG,esp_err_t err);


#endif //LSM6DSOX_DRV