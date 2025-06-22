/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-19 16:25:38                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-21 13:43:09                              *
 * @FilePath              : Trotten_Electric_Table/main/ssd1306.h            *
 ****************************************************************************/



#pragma once


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
    
}ssd1306_queue_message_t;

/**
 * Starts the display task
 */
void start_display_task(void);

// void ssd1306_task(void *pv);
void ssd1306_task();

bool send_to_display(const ssd1306_queue_message_t *msg); 
