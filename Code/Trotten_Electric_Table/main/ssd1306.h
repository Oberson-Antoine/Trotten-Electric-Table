/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-28 18:01:05                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-28 20:25:19                              *
 * @FilePath              : Trotten_Electric_Table/main/ssd1306.h            *
 ****************************************************************************/

/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-19 16:25:38                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-28 17:56:32                              *
 * @FilePath              : Trotten_Electric_Table/main/ssd1306.h            *
 ****************************************************************************/


#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define WIDTH 128 // screen width
#define HEIGHT 32 // screen height


typedef enum ssd1306_server_message
{
    HEIGHT_FLOAT,
    STRING_PRINT,
}ssd1306_message_e;


typedef struct ssd1306_queue_message
{
    ssd1306_message_e msgID;
    int x; // X position for text
    int y; // Y position for text
    char text[32]; // Text to display
    uint8_t height_mes;   
}ssd1306_queue_message_t;

/**
 * Starts the display task
 */
void start_display_task(void);

// void ssd1306_task(void *pv);
void ssd1306_task();

// bool send_to_display(const ssd1306_queue_message_t *msg); 

QueueHandle_t ssd1306_get_queue_handle(void);

