/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-08-07 11:54:50                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-08-07 11:54:50                              *
 * @FilePath              : Trotten_Electric_Table/main/INA260.c             *
 ****************************************************************************/

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "esp_err.h"
#include "esp_log.h"

#include "INA260.h"
#include "i2c_p.h"

static i2c_master_bus_handle_t i2c_bus_handle;
static SemaphoreHandle_t i2c_mutex_handle;

static bool init_flag = false;

static const char TAG[] = "INA260";

i2c_master_dev_handle_t ina_dev_handle;
i2c_device_config_t ina_i2c_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = INA_ADDRESS,
    .scl_speed_hz = 100000,
};

esp_err_t ina_write_reg(uint8_t reg, uint8_t value)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t data[3] = {reg, value};
    data[0] = reg;
    data[1] = (value >> 8) & 0xFF; // splitting the uint16 value registry in two
    data[2] = value & 0xFF;

    ret = i2c_master_transmit(ina_dev_handle, data, sizeof(data), pdMS_TO_TICKS(1000));
    ESP_LOGI(TAG, "Transmit returned: %s", esp_err_to_name(ret));

    return ret;
}

void INA260_init(void)
{
    if (init_flag == false)
    {
        i2c_bus_handle = get_i2c_bus();
        i2c_mutex_handle = get_i2c_mutex();
        esp_err_t ret = i2c_master_bus_add_device(i2c_bus_handle, &ina_i2c_cfg, &ina_dev_handle);
        ESP_LOGI(TAG, "MCP23017 registred device: %s", esp_err_to_name(ret));

        if (xSemaphoreTake(i2c_mutex_handle, portMAX_DELAY) == pdTRUE)
        {
            uint16_t config = 0;

            config |= INA_AVG_4;        // number of averages = 1
            config |= INA_VBUSCT_1_1MS; // conversion time = 1.1 ms
            config |= INA_ISHCT_1_1MS;  //  conversion time = 1.1 ms
            config |= INA_MODE_SHUNT_CONTINUOUS;
            ESP_ERROR_CHECK(ina_write_reg(INA260_REG_CONFIG, config));

            xSemaphoreGive(i2c_mutex_handle);
        }

        init_flag = true;
    }
}

/**
 * Reads the INA260 buffer
 * @return uint8 value
 */
uint16_t INA260_read(uint8_t reg)
{
    uint16_t value = 0;
    uint8_t data[2] = {0};
    if (xSemaphoreTake(i2c_mutex_handle, portMAX_DELAY) == pdTRUE)
    {
        // Set register pointer
        if (i2c_master_transmit(ina_dev_handle, &reg, 1, pdMS_TO_TICKS(1000)) == ESP_OK)
        {
            // Read 2 bytes (MSB first)
            if (i2c_master_receive(ina_dev_handle, data, 2, pdMS_TO_TICKS(1000)) == ESP_OK)
            {
                value = ((uint16_t)data[0] << 8) | data[1]; //merges the datas
                printf("Current : %f \n", value *1.25/1000);
            }
            xSemaphoreGive(i2c_mutex_handle);
        }
    }
    return value;
}