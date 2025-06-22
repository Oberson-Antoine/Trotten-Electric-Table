/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-18 14:36:09                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-19 17:51:41                              *
 * @FilePath              : Trotten_Electric_Table/unused/ACS712.c           *
 ****************************************************************************/


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "task_common.h"
#include <math.h>

#include "ACS712.h"

static const char TAG[] = "ACS712";

int ADC_raw = 0;
float current = 0;

adc_oneshot_unit_handle_t adc1_handle;

adc_cali_handle_t adc1_cali_handle = NULL;



esp_err_t ACS712_GPIO_INIT(void)
{
    // 1. Create and configure ADC1 oneshot driver
    
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    esp_err_t err = adc_oneshot_new_unit(&init_config, &adc1_handle);
    ESP_LOGI(TAG, "adc_oneshot_new_unit returned: %d", err);

    // 2. Configure the ADC1 channel (e.g., GPIO32 = ADC_CHANNEL_4)
    adc_oneshot_chan_cfg_t channel_config = {
        .atten = ADC_ATTEN_DB_12,      // 0-3.6V input range
        .bitwidth = ADC_BITWIDTH_12,   // 12-bit resolution
    };
    adc_oneshot_config_channel(adc1_handle, ACS712_GPIO, &channel_config);

    //calibration of the ADC
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };

    esp_err_t cali_ret = adc_cali_create_scheme_line_fitting(&cali_config, &adc1_cali_handle);
    if (cali_ret != ESP_OK) {
        printf("ADC calibration not supported on this chip or efuse not programmed.\n");
        adc1_cali_handle = NULL;
    }

    adc_oneshot_read(adc1_handle, ACS712_GPIO, &ADC_raw);




    return ESP_OK;
}

esp_err_t ACS712_measure(void)
{
    //esp_err_t err = adc2_get_raw(ADC_WIDTH_BIT_12,ACS712_GPIO, &ADC_raw);
    adc_oneshot_read(adc1_handle, ACS712_GPIO, &ADC_raw);
    int voltage = 0;
    adc_cali_raw_to_voltage(adc1_cali_handle, ADC_raw, &voltage);
    printf("ADC raw: %d, Calibrated voltage: %d mV\n", ADC_raw, voltage);

    //float adc_voltage = (ADC_raw- adc_zero) * (3.6 / 4096.0); 
    float current_voltage = (((float)voltage) * (ACS712_R1+ACS712_R2)/ACS712_R2); //when nothing no load should output 2500 mV
    float current_voltage_offset = current_voltage -2500.f;
    current = fabsf(current_voltage_offset/ 66);
            printf("ADC_raw : %lf \n\n", current_voltage); 
    
    return ESP_OK;
}

/**
 * ACS712 sensor task
 */

static void HCSR04_task(void *pvParameter)
{
    ACS712_GPIO_INIT();
    ESP_LOGI(TAG,"ACS712 task starting");
    for (;;)
    {
        ACS712_measure();
        printf("Current : %lf \n\n", current);
        // 0,5 second delay before starting new measurement
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}


void ACS712_task_start()
{
    xTaskCreatePinnedToCore(&HCSR04_task,"HCSR04_task", ACS712_TASK_STACK_SIZE, NULL, ACS712_TASK_PRIORITY, NULL, ACS712_TASK_CORE_ID);
}