/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-19 16:31:26                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-19 18:36:07                              *
 * @FilePath              : Trotten_Electric_Table/main/i2c.h                *
 ****************************************************************************/



#pragma once

#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "freertos/semphr.h"


#define I2C_PORT        I2C_NUM_0 //-1 for auto select
#define I2C_SDA_GPIO    21
#define I2C_SCL_GPIO    22


/**
 * Initializes the i2c bus settings
 */
esp_err_t i2c_bus_init(void);

i2c_master_bus_handle_t get_i2c_bus(void);
SemaphoreHandle_t get_i2c_mutex(void);