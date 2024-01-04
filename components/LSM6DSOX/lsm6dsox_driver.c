#include "lsm6dsox_driver.h"


int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len)
{   
    i2c_port_t i2c_port_no = 0;
    esp_err_t err = i2c_master_write_to_device(
        0,
        Reg,
        Bufp,
        sizeof(Bufp),
        I2C_MASTER_TIMEOUT_MS/portTICK_RATE_MS    
    );

    if(err == ESP_OK){
        return 0;
    }
    else{
        return handle_esp_err("platform_write",err);
    }
}

int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{   
    esp_err_t err = i2c_master_read_from_device(
        0,
        Reg,
        Bufp,
        sizeof(Bufp),
        I2C_MASTER_TIMEOUT_MS/portTICK_RATE_MS    
    );

    if(err == ESP_OK){
        return 0;
    }
    else{
        return handle_esp_err("platform_read",err);
    }
}

int lsm_init(i2c_port_t i2c_port_number)
{   
    char* TAG = "lsm_init";

    stmdev_ctx_t dev_ctx;
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.handle = &i2c_port_number;

    uint8_t whoamI;
    
    lsm6dsox_device_id_get(&dev_ctx,&whoamI);

    if(whoamI != LSM6DSOX_ID){
        ESP_LOGE(TAG,"Init error 0x%x",whoamI);
        while(1);
    }
    else{
        ESP_LOGI(TAG,"Sensor detected");
    }

    return 0;

}

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
    return 1;
}