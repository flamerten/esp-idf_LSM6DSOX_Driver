/**
 * @file               lsm6dsox_driver.c
 * @author             Samuel Yow (flamerten@gmail.com)
 * @brief              LSM6DSOX_driver driver file for ESP-iDF, based on ST's lsm6dsox_reg.c 
 *                     PID driver https://github.com/STMicroelectronics/lsm6dsox-pid.
 */

#include "lsm6dsox_driver.h"


int lsm_init(LSM_DriverConfig *sensor_config)
{   
    char* TAG = "lsm_init";

    if(sensor_config->LSM_SA){
        sensor_config->LSM_8bit_addr = (LSM6DSOX_I2C_ADD_H >> 1);
    }
    else{
        sensor_config->LSM_8bit_addr = (LSM6DSOX_I2C_ADD_L >> 1); 
    }

    stmdev_ctx_t dev_ctx;
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.handle = sensor_config;

    uint8_t whoamI;
    
    lsm6dsox_device_id_get(&dev_ctx,&whoamI);

    if(whoamI != LSM6DSOX_ID){
        ESP_LOGE(TAG,"Unable to detect sensor is %x should be %x",whoamI,LSM6DSOX_ID);
        while(1);
    }
    else{
        ESP_LOGI(TAG,"Sensor detected");
    }

    int ret = 0;
    ret = lsm6dsox_xl_data_rate_set(&dev_ctx,sensor_config->Acc_Data_Rate);
    ret = lsm6dsox_xl_full_scale_set(&dev_ctx,sensor_config->Acc_Scale);

    ret = lsm6dsox_gy_data_rate_set(&dev_ctx,sensor_config->Gyr_Data_Rate);
    ret = lsm6dsox_gy_full_scale_set(&dev_ctx,sensor_config->Gyr_Scale);
    
    return ret;

} 



/**
 * @brief    Write generic device register for ESP-IDF based on PID
 * 
 * @param handle       LSM_DriverConfig
 * @param Reg          register to write
 * @param Bufp         pointer to data to write in register reg
 * @param len          number of consecutive register to write
 * @return int32_t     0 if ok, else 1
 */
int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len)
{   
    LSM_DriverConfig *sensor_config = (LSM_DriverConfig *)handle;
    uint8_t sensor_addr = sensor_config->LSM_8bit_addr;
    i2c_port_t i2c_port_number = sensor_config->i2c_num;

    //Create a new buffer, message that includes register to write to
    uint8_t msg[len + 1];
    msg[0] = Reg;
    memcpy(&msg[1],&Bufp[0],len * sizeof(Reg));

    esp_err_t err = i2c_master_write_to_device(
        i2c_port_number,
        sensor_addr,
        msg,
        len + 1,
        I2C_MASTER_TIMEOUT_MS/portTICK_RATE_MS    
    );

    if(err == ESP_OK){
        return 0;
    }
    else{
        return handle_esp_err("platform_write",err);
    }
}


/**
 * @brief    Read generic device register for ESP-IDF based on PID 
 * 
 * @param handle       LSM_DriverConfig 
 * @param Reg          register to read
 * @param Bufp         pointer to buffer that store the data read
 * @param len          number of consecutive register to read
 * @return int32_t     0 if okay, else 1
 */
int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{   
    LSM_DriverConfig *sensor_config = (LSM_DriverConfig *)handle;
    uint8_t sensor_addr = sensor_config->LSM_8bit_addr;
    i2c_port_t i2c_port_number = sensor_config->i2c_num;


    esp_err_t err = i2c_master_write_read_device(
        i2c_port_number,
        sensor_addr, 
        &Reg,
        1,
        Bufp,
        len,
        I2C_MASTER_TIMEOUT_MS/portTICK_RATE_MS    
    );

    if(err == ESP_OK){
        return 0;
    }
    else{
        return handle_esp_err("platform_read",err);
    }
}

/**
 * @brief    Handle i2c errors in ESP-iDF
 * 
 * @param TAG          tag for esp_log
 * @param err          esp_err_t
 * @return int32_t     0 if okay, else 1
 */
int32_t handle_esp_err(char *TAG,esp_err_t err)
{
    if(err == ESP_OK){
        return 0;
    }
    else if(err == ESP_ERR_INVALID_ARG){
        ESP_LOGE(TAG,"Parameter Error");
    }
    else if(err == ESP_ERR_INVALID_ARG){
        ESP_LOGE(TAG,"Sending command error, slave hasn’t ACK the transfer.");
    }
    else if(err == ESP_ERR_INVALID_STATE){
        ESP_LOGE(TAG,"I2C driver not installed or not in master mode");
    }
    else if(err == ESP_ERR_TIMEOUT){
        ESP_LOGE(TAG,"Operation timeout because the bus is busy");
    }
    else{
        ESP_LOGE(TAG,"Unkown Error %d",err);
    }
    return 1;
}