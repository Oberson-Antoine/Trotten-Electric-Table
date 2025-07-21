/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-07-14 17:46:43                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-07-16 13:42:21                              *
 * @FilePath              : Trotten_Electric_Table/main/MCP23017_p.C         *
 ****************************************************************************/

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "esp_err.h"
#include "esp_log.h"

#include "MCP23017_p.h"
#include "i2c_p.h"

i2c_master_bus_handle_t i2c_bus_handle;
SemaphoreHandle_t i2c_mutex_handle;

static const char TAG[] = "MCP23017";

i2c_master_dev_handle_t mcp_dev_handle;
i2c_device_config_t mcp_i2c_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = MCP23017_ADDRESS,
    .scl_speed_hz = 100000,
};

bool init_flag = false;

esp_err_t mcp23017_write_reg(uint8_t reg, uint8_t value)
{
    esp_err_t ret = ESP_FAIL;
    uint8_t data[2] = {reg, value};
    // i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // i2c_master_start(cmd);
    // i2c_master_write_byte(cmd, (MCP23017_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    // i2c_master_write(cmd, data, 2, true);
    // i2c_master_stop(cmd);
    // i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    // i2c_cmd_link_delete(cmd);

    ret = i2c_master_transmit(mcp_dev_handle, data, sizeof(data), pdMS_TO_TICKS(1000));
    ESP_LOGI(TAG, "Transmit returned: %s", esp_err_to_name(ret));

    return ret;
}

// initializes the MCP23017 and configures it
void MCP23017_init(void)
{
    if (init_flag == false)
    {
        i2c_bus_handle = get_i2c_bus();
        i2c_mutex_handle = get_i2c_mutex();
        esp_err_t ret = i2c_master_bus_add_device(i2c_bus_handle, &mcp_i2c_cfg, &mcp_dev_handle);
        ESP_LOGI(TAG, "MCP23017 registred device: %s", esp_err_to_name(ret));
        if (xSemaphoreTake(i2c_mutex_handle, portMAX_DELAY) == pdTRUE)
        {

            uint8_t pin_mask = (1 << 6) | (1 << 7);
            // Set all GPB pins as input
            ESP_ERROR_CHECK(mcp23017_write_reg(MCP23017_REG_IODIRB, 0xff));
            // Set all GPB pins low
            // ESP_ERROR_CHECK(mcp23017_write_reg(MCP23017_REG_GPIOB, 0x00));

            ESP_ERROR_CHECK(mcp23017_write_reg(MCP23017_REG_GPPUB, 0xff));

            ESP_ERROR_CHECK(mcp23017_write_reg(MCP23017_REG_GPINTENB, pin_mask));

            ESP_ERROR_CHECK(mcp23017_write_reg(MCP23017_REG_INTCONB, 0x00));

            // mcp23017_write_reg(0x5, 0xff);


            uint8_t reg = MCP23017_REG_GPIOB;
            uint8_t dummy;
            i2c_master_transmit(mcp_dev_handle, &reg, 1, pdMS_TO_TICKS(1000));
            i2c_master_receive(mcp_dev_handle, &dummy, 1, pdMS_TO_TICKS(1000));
            printf("valeur lue : %d \n", dummy);

            xSemaphoreGive(i2c_mutex_handle);
        }
    }
}

void MCP23017_task()
{

}

void start_MCP23017_task()
{
    
}