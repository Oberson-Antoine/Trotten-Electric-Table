/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-12 13:39:40                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-25 18:12:59                              *
 * @FilePath              : udemy_esp32/main/rgb_led.c                       *
 ****************************************************************************/

/**
 * @author Antoine Oberson <antoine@oberson.org>
 * @file Description
 * @desc Created on 2025-06-08 12:21:41 pm
 * @copyright 
 */
#include <stdbool.h>


#include "esp_err.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "rgb_led.h"





// handle for rgb led_pwm_init
bool g_pwm_init_handle = false;

// Addressable LED configuratrion
led_strip_config_t Led_config;

// RMT backend specific configuration
led_strip_rmt_config_t RMT_config;

/// LED strip handle
led_strip_handle_t led_strip_handle;

// Configure GPIO_NUM_4 as input
gpio_config_t io_conf = {
    .pin_bit_mask = ((1ULL << RGB_LED_ENABLE) | (1ULL << 13)), 
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
};

/*
*Initializes the RGB LED settings per channel, including
*the GPIO for each color, mode and timer configuration
*/
static void rgb_led_pwm_init(void) //static veut dire que la fonction est confinée au fichier rgb_led
{
    // Set the enable pin of the NEOPIXEL tp HIGH
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_set_level(RGB_LED_ENABLE, 1));
    ESP_ERROR_CHECK(gpio_set_level(13, 1)); //set the powerled to on
    
    // Configure the LED strip
    Led_config.strip_gpio_num = RGB_LED_GPIO;
    Led_config.max_leds = 1;
    Led_config.color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB;
    Led_config.led_model = LED_MODEL_WS2812;
    Led_config.flags.invert_out = false;

    // Configure the RMT
    RMT_config.clk_src = RMT_CLK_SRC_DEFAULT;
    RMT_config.resolution_hz = 10 * 1000 * 1000; // RMT counter clock frequency: 10MHz
    RMT_config.mem_block_symbols = 64;
    RMT_config.flags.with_dma = true;

    //Creates the LED object
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&Led_config, &RMT_config, &led_strip_handle));

    g_pwm_init_handle = true;
}

/*
* Sets the RGB color
*/
static void rgb_led_set_color(uint32_t red, uint32_t green, uint32_t blue)
{
    // value should be 0 -255 for 8 bit number
    // ledc_set_duty(ledc_ch[0].mode, ledc_ch[0].channel, red);
    // ledc_update_duty(ledc_ch[0].mode, ledc_ch[0].channel);

    // ledc_set_duty(ledc_ch[1].mode, ledc_ch[1].channel, green);
    // ledc_update_duty(ledc_ch[1].mode, ledc_ch[1].channel);

    // ledc_set_duty(ledc_ch[2].mode, ledc_ch[2].channel, blue);
    // ledc_update_duty(ledc_ch[2].mode, ledc_ch[2].channel);
    ESP_ERROR_CHECK(led_strip_set_pixel(led_strip_handle, 0, red, green, blue));
    ESP_ERROR_CHECK(led_strip_set_pixel_hsv(led_strip_handle, 0,360,255,25));
    ESP_ERROR_CHECK(led_strip_refresh(led_strip_handle)); // refresh to send data
}


void rgb_led_wifi_app_started(void)
{
    if (g_pwm_init_handle == false)
    {
        rgb_led_pwm_init();
    }
    
    rgb_led_set_color(255, 255, 0);
}

void rgb_led_http_server_started(void)
{
    if (g_pwm_init_handle == false)
    {
        rgb_led_pwm_init();
    }
    
    rgb_led_set_color(0, 255, 255);
}

void rgb_led_wifi_connected(void)
{
    if (g_pwm_init_handle == false)
    {
        rgb_led_pwm_init();
    }
    
    rgb_led_set_color(0, 255, 0);
}

