/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-12 14:06:37                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-25 17:26:01                              *
 * @FilePath              : udemy_esp32/main/rgb_led.h                       *
 ****************************************************************************/

#pragma once

//RGB LED GPIOs
#define RGB_LED_GPIO    33
#define RGB_LED_ENABLE  21

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
