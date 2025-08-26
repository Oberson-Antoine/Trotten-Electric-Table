/*
 * Filename: c:\Users\antoi\Documents\Github\Trotten-Electric-Table\Code\Trotten_Electric_Table\main\BTS7690.H
 * Path: c:\Users\antoi\Documents\Github\Trotten-Electric-Table\Code\Trotten_Electric_Table\main
 * Created Date: Tuesday, August 26th 2025, 3:22:17 pm
 * Author: Oberson-Antoine
 * 
 * Copyright (c) 2025 Your Company
 */

 #pragma once

 #include <esp_err.h>

 #define R_EN_PIN       11
 #define R_PWM_PIN      12
 #define L_EN_PIN       6
 #define L_PWM_PIN      10


 /**
 * @brief Init motor driver
 *
 * @return `ESP_OK` on success
 */
esp_err_t BTS7960_init(void);


/**
 * Starts DHT22 Sensor task
 */
void BTS7960_task_start(void);
