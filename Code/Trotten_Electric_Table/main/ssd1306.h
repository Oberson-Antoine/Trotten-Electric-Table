/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-28 18:01:05                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-07-06 18:13:42                              *
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
#define SSD1306_address 0x3C


typedef enum ssd1306_server_message
{
    HEIGHT_FLOAT,
    STRING_PRINT,
    CONTRAST_SET,
}ssd1306_message_e;


typedef struct ssd1306_queue_message
{
    ssd1306_message_e msgID;
    int x; // X position for text
    int y; // Y position for text
    char text[32]; // Text to display
    uint8_t height_mes;
    uint8_t contrast;   
}ssd1306_queue_message_t;

/**
 * Starts the display task
 */
void start_display_task(void);

// void ssd1306_task(void *pv);
void ssd1306_task();

// bool send_to_display(const ssd1306_queue_message_t *msg); 

QueueHandle_t ssd1306_get_queue_handle(void);

// // Used to change contrast of the screen
// void ssd1306_contrast_set(int8_t contrast);

