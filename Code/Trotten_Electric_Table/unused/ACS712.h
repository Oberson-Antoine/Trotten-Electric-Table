/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-18 14:30:33                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-19 17:51:23                              *
 * @FilePath              : Trotten_Electric_Table/unused/ACS712.h           *
 ****************************************************************************/

#pragma once

#include <esp_err.h>

#include "hal/adc_types.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"




#define ACS712_GPIO     ADC_CHANNEL_6 //GPIO 34
#define ACS712_R1       10000.
#define ACS712_R2       20000.





/**
 * Function used to measure the current
 * @param current the current variable that will be modified
 */

 esp_err_t ACS712_measure(void);

 /**
  * Function that configures the GPIO for analogic data reading
  * @return ESP_OK
  */

esp_err_t ACS712_GPIO_INIT(void);

/**
 * Starts ACS712 Sensor task
 */
void ACS712_task_start(void);