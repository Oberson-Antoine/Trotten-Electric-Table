/******************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>         *
 * @CreatedDate           : 2025-06-18 12:34:45                               *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>         *
 * @LastEditDate          : 2025-07-15 11:33:07                               *
 * @FilePath              : Trotten_Electric_Table/main/main.c                *
 *****************************************************************************/

#include "nvs_flash.h"

#include "wifi_app.h"

#include "HCSR04.h"

#include "i2c_p.h"

#include "ssd1306.h"

#include "interrupt.h"

#include "MCP23017_p.h"

#include "INA260.h"

#include "BTS7690.h"


void app_main(void)
{


        //initialize NVS
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
        {
             ESP_ERROR_CHECK(nvs_flash_erase());
             ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);

        // Initialize the i2c Bus
        i2c_bus_init();

        // start wifi
        wifi_app_start();

        
        

        //Start ssd1306_task DECLARE BEFORE ANY COMPONENT THAT WOULD USE THE SCREEN
        start_display_task();

        //start HCSR04 sensor task
        HCSR04_task_start();


        start_interrupt_task();
        
        MCP23017_init();

        //INA260_init();
        
        

}