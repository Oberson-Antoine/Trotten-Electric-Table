/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-07-14 17:53:29                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-07-14 22:08:52                              *
 * @FilePath              : Trotten_Electric_Table/main/interrupt.h          *
 ****************************************************************************/



#pragma once

#include <esp_err.h>

#define MCP23_INT_PIN 9

typedef enum int_message_ids
{
    MCP23017,
    INA260,
}int_message_e;

typedef struct int_queue_message
{
    int_message_e msgID;
}int_queue_message_t;

/**
 * @brief Init ranging module
 *
 * @param dev Pointer to the device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t InterruptInit(void);

/**
 * Starts the interrupt task
 */
void start_interrupt_task(void);


void int_main_task();