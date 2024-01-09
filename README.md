# LSM6DSOX Driver for ESP-IDF

ST LSM6DSOX driver developed using ESP-IDF V4.4 for ESP-IDF based on the [LSM6DSOX Platform Indepdenent Driver](https://github.com/STMicroelectronics/lsm6dsox-pid) provided by ST Engineering. Develompment only has basic features and is still in progress.

## Instructions for use
1. Clone the repository and copy the components folder into project folder.
2. Create a `LSM_DriverConfig` struct and configure it. Then pass it as a pointer to various functions. For example

```C
LSM_DriverConfig LSM_config = {
    .i2c_port_num = 0,
    .LSM_SA = 0,

    .Acc_Data_Rate = LSM6DSOX_XL_ODR_26Hz,
    .Gyr_Data_Rate = LSM6DSOX_GY_ODR_26Hz,
    .Acc_Scale = LSM6DSOX_8g,
    .Gyr_Scale = LSM6DSOX_1000dps
};

int ret = lsm_init(&LSM_config);
```

3. Note that main is simply for testing and development. A example folder may be added in the future.

## Current Features
- `lsm_init(LSM_DriverConfig *sensor_cfg)`: Sensor (accel and gyro) initialisation with scale and data rate 
- `lsm_data_ready(LSM_DriverConfig *sensor_cfg)`, `lsm_update_raw(LSM_DriverConfig *sensor_cfg)`, `lsm_convert_raw(LSM_DriverConfig *sensor_cfg)` Checking if gyro and accel data is ready, before pulling data, and converting it from full scale `int16_t` to `mg` and `mdps`

