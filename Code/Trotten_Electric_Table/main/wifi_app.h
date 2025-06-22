/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-08 14:54:20                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-16 11:39:43                              *
 * @FilePath              : udemy_esp32/main/wifi_app.h                      *
 ****************************************************************************/


#pragma once

#include "esp_netif.h"
#include "freertos/FreeRTOS.h"

// wifi application settings
#define WIFI_AP_SSID                "ESP32_AP"      //AP name
#define WIFI_AP_PASSWORD            "password"      //AP password
#define WIFI_AP_CHANNEL             1               //AP channel
#define WIFI_AP_SSID_HIDDEN         0               //Wifi visibility (0 = visible)
#define WIFI_AP_MAX_CONNECTIONS     5               //Number of simultaneous connections
#define WIFI_AP_BEACON_INTERVAL     100             //AP beaucon : 100ms as recommended
#define WIFI_AP_IP                  "192.168.0.1"   //AP default IP
#define WIFI_AP_GATEWAY             "192.168.0.1"   //AP default gateway (should be the same as IP)
#define WIFI_AP_NETMASK             "255.255.255.0" //AP netmask
#define WIFI_AP_BANDWIDTH           WIFI_BW_HT20    //AP banswidth 20MHz (40MHz is the other option)
#define WIFI_STA_POWER_SAVE         WIFI_PS_NONE    //Power save is not used
#define MAX_SSID_LENGTH             32              //IEEE standard max
#define MAX_PASSWORD_LENGTH         64              //IEEE standard max
#define MAX_CONNECTION_RETRIES      5               //Retry number on disconnect

//Netif (network interface) object for the Station and Access Point
extern esp_netif_t* esp_netif_sta; //extern = accessible partout
extern esp_netif_t* esp_netif_ap;

/*
*Message IDs for the WIFI application task
*@note Expend this based on your application requirments
*/

typedef enum wifi_app_message
{
    WIFI_APP_MSG_START_HTTP_SERVER = 0,
    WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
    WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
} wifi_app_message_e;  

/*
* Structure for the message queue
* @note Expand this based on application need e.g. add another type and parameter as required
*/
typedef struct wifi_app_queue_message
{
    wifi_app_message_e msgID;
}wifi_app_queue_message_t;

/*
* Sends a message to the queue
* @param msgID message ID from the wifi_app_message_e enum.
* @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE
* @note Expend the parameter list based on your requirements e.g. how you'ce expended the wifi_app_queue_message_t.
*/
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

/*
* Starts the Wifi RTOS task
*/
void wifi_app_start();
