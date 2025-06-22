/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-14 18:41:34                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-14 19:00:32                              *
 * @FilePath              : udemy_esp32/main/http_server.h                   *
 ****************************************************************************/

#pragma once
#include "freertos/FreeRTOS.h"

#define OTA_UPDATE_PENDING      0
#define OTA_UPDATE_SUCCESSFUL   1
#define OTA_UPDATE_FAILED      -1

/**
 * messages for the HTTP monitor
 */
typedef enum http_server_message
{
    HTTP_MSG_WIFI_CONNECT_INIT = 0,
    HTTP_MSG_WIFI_CONNECT_SUCCESS,
    HTTP_MSG_WIFI_CONNECT_FAIL,
    HTTP_MSG_OTA_UPDATE_SUCCESSFUL,
    HTTP_MSG_OTA_UPDATE_FAILED,
} http_server_message_e;

/**
 *  Structure for the message queue
 */
typedef struct http_server_queue_message
{
    http_server_message_e msgID;
} http_server_queue_message_t;

/**
 * Sends a message to the queue
 * @param msgID from the http_server_message_e enum
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE
 * @note Expand the parameter list based on your requirements e.q. how you've expanded the http_server_queue_message_t
 */
BaseType_t http_server_monitor_send_message(http_server_message_e msgID);

/**
 * Starts the http server
 */
void http_server_start(void);

/**
 * Stops the http server
 */
void http_server_stop(void);

/**
 *  Timer callback function which calls esp_restart upon successful update
 */
void http_server_fw_update_reset_callback(void *arg);