/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-19 16:32:19                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-19 18:22:59                              *
 * @FilePath              : Trotten_Electric_Table/main/i2c.c                *
 ****************************************************************************/

#include "i2c.h"


static i2c_master_bus_handle_t i2c_bus_handle = NULL;
static SemaphoreHandle_t i2c_mutex = NULL;


esp_err_t i2c_bus_init(void)
 {
    if (i2c_bus_handle) return ESP_OK; //if the handle already exists exit the init

    i2c_master_bus_config_t i2c_mst_config ={
        .clk_source =   I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false, 
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &i2c_bus_handle));

    i2c_mutex = xSemaphoreCreateMutex(); //  creates a new mutex (mutual exclusion semaphore) for synchronizing access to a shared resource (here, likely the I2C bus).
    if (i2c_mutex == NULL) {
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
 }

 i2c_master_bus_handle_t get_i2c_bus(void) {
    return i2c_bus_handle;
}

SemaphoreHandle_t get_i2c_mutex(void) {
    return i2c_mutex;
}