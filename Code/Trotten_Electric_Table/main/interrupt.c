/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-07-14 17:48:51                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-07-16 13:09:27                              *
 * @FilePath              : Trotten_Electric_Table/main/interrupt.c          *
 ****************************************************************************/

#include "esp_intr_alloc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "task_common.h"

#include "interrupt.h"
#include "rgb_led.h"
#include "i2c_p.h"

static bool init_flag = false;

static QueueHandle_t int_message_queue = NULL;

BaseType_t xHigherPriorityTaskWoken = pdFALSE;

static const char TAG[] = "Interrupt";

void IRAM_ATTR MCP23_HANDLER(void *arg)
{   
    int_queue_message_t msg = { .msgID = MCP23017 };
    xQueueSendFromISR(int_message_queue, &msg, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }
}

// Initializes and set the gpios for interrupts
esp_err_t InterruptInit(void)
{
    if (init_flag == false)
    {
        gpio_config_t io_conf = {
            .intr_type = GPIO_INTR_NEGEDGE, // or GPIO_INTR_NEGEDGE, or GPIO_INTR_ANYEDGE
            .mode = GPIO_MODE_INPUT,
            .pin_bit_mask = (1ULL << MCP23_INT_PIN),
            .pull_up_en = 1,
            .pull_down_en = 0,
        };
        gpio_config(&io_conf);

        gpio_install_isr_service(0);
        gpio_isr_handler_add(MCP23_INT_PIN, MCP23_HANDLER, (void *)MCP23_INT_PIN);
    }
    init_flag = true;
    // gpio_set_direction(MCP23_INT_PIN, GPIO_MODE_INPUT);
    // gpio_pulldown_en(MCP23_INT_PIN);
    // gpio_pullup_dis(MCP23_INT_PIN);
    // gpio_set_intr_type(MCP23_INT_PIN, GPIO_INTR_POSEDGE);

    return ESP_OK;
}

void int_task()
{
    InterruptInit();

    int_queue_message_t msg;
    for (;;)
    {
        if (xQueueReceive(int_message_queue, &msg, portMAX_DELAY) == pdTRUE)
        {
            switch (msg.msgID)
            {
            case MCP23017:
                ESP_LOGI(TAG, "INTERRUPT DETECTED");
                break;

            default:
                break;
            }
        }
    }
}

void start_interrupt_task(void)
{
    int_message_queue = xQueueCreate(5, sizeof(int_queue_message_t));
    xTaskCreatePinnedToCore(int_task, "int_task", INT_TASK_STACK_SIZE, NULL, INT_TASK_PRIORITY, NULL, INT_TASK_CORE_ID);
}