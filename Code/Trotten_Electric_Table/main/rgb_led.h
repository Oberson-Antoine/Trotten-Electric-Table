/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-12 14:06:37                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-12 14:11:00                              *
 * @FilePath              : udemy_esp32/main/rgb_led.h                       *
 ****************************************************************************/

#pragma once

//RGB LED GPIOs
#define RGB_LED_RED_GPIO    21
#define RGB_LED_GREEN_GPIO  22
#define RGB_LED_BLUE_GPIO   23

//RGB LED color mix channels
#define RGB_LED_CHANNEL_NUM     3 //car ya RGB (3 couleurs)


//RGB LED configuration
typedef struct rgb_led
{
    int channel;
    int gpio;
    int mode;
    int timer_index;
} ledc_info_t;
//ledc_info_t ledc_ch[RGB_LED_CHANNEL_NUM];

/*
*Color to indicate WiFi application has started
*/
void rgb_led_wifi_app_started(void);

/*
*Color to indicate HTTP application has started
*/
void rgb_led_http_server_started(void);

/*
*Color to indicate that the ESP32 is connected to an accespoint.
*/
void rgb_led_wifi_connected(void);
