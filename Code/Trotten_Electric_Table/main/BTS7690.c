/*
 * Filename: c:\Users\antoi\Documents\Github\Trotten-Electric-Table\Code\Trotten_Electric_Table\main\BTS7690.c
 * Path: c:\Users\antoi\Documents\Github\Trotten-Electric-Table\Code\Trotten_Electric_Table\main
 * Created Date: Tuesday, August 26th 2025, 3:22:06 pm
 * Author: Oberson-Antoine
 * 
 * Copyright (c) 2025 Your Company
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "task_common.h"
#include "driver/mcpwm.h"
#include "driver/mcpwm_gen.h"
#include "driver/mcpwm_oper.h"

#include "BTS7690.h"

mcpwm_generator_config_t mcpwm_gen_config_R =
{
    .gen_gpio_num = R_PWM_PIN,
    .flags = {0},
    
};

mcpwm_operator_config_t mcpwm_ope_config_R =
{
    .group_id = 0,
    .intr_priority = 0,
    .flags = {0},
    
};

mcpwm_generator_config_t mcpwm_gen_config_L =
{
    .gen_gpio_num = L_PWM_PIN,
    .flags = {0},
};

mcpwm_operator_config_t mcpwm_ope_config_L =
{
    .group_id = 1,
    .intr_priority = 0,
    .flags = {0},
    
};

mcpwm_gen_handle_t gen_handle_R = NULL;
mcpwm_oper_handle_t ope_handle_R = NULL;

mcpwm_gen_handle_t gen_handle_L = NULL;
mcpwm_oper_handle_t ope_handle_L = NULL;

esp_err_t BTS7960_init(void)
{
    //initializes the operators for the two pwm lines
    ESP_ERROR_CHECK(mcpwm_new_operator(&mcpwm_ope_config_R,&ope_handle_R));
    ESP_ERROR_CHECK(mcpwm_new_operator(&mcpwm_ope_config_L,&ope_handle_L));

    //initializes the generators
    ESP_ERROR_CHECK(mcpwm_new_generator(ope_handle_R, mcpwm_gen_config_R, &gen_handle_R));
    ESP_ERROR_CHECK(mcpwm_new_generator(ope_handle_L, mcpwm_gen_config_L, &gen_handle_L));

    
}