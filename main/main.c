#include <stdio.h>
#include <stdint.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "driver/i2c.h"
#include "lsm6dsox_driver.h"

static const char *TAG = "main.c";

#define I2C_MASTER_SCL_IO           GPIO_NUM_22                         /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           GPIO_NUM_21                        /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */

//Fun prototypes
static esp_err_t i2c_master_init(void);

void app_main(void)
{
    ESP_LOGI(TAG,"Hello World");
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG,"I2C Initialised successfully");

    lsm_init(I2C_MASTER_NUM);
}

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}
