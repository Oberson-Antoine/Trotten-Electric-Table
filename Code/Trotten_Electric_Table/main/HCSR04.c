/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-03-01 00:00:00                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-28 21:33:32                              *
 * @FilePath              : Trotten_Electric_Table/main/HCSR04.c             *
 ****************************************************************************/


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "esp_log.h"
#include "HCSR04.h"
#include "ssd1306.h"
#include "task_common.h"


#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10   // HC SR04 start pulse with 10 usec*/
#define PING_TIMEOUT 6000       // echo input high max 6 msec after trigger puls*/
#define ROUNDTRIP_CM 58

// GPIO pins to HC SR04 module


/* Logging tag */
static const char *log_tag = "HCRS04 tag"; /*  Ultrasonic logging tag */
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

/* 
    Private functions
*/

static esp_err_t ultrasonic_measure_raw(uint32_t max_time_us, uint32_t *time_us)
{
    esp_err_t return_value = ESP_OK;
    int64_t   echo_start;
    int64_t   time;

    if (time_us != NULL) {
        portENTER_CRITICAL(&mux);

        // Ping: Low for 4 us, then high 10 us
        gpio_set_level(ESP_HCSR04_TRIGGER_PIN, 0);
        esp_rom_delay_us(TRIGGER_LOW_DELAY);
        gpio_set_level(ESP_HCSR04_TRIGGER_PIN, 1);
        esp_rom_delay_us(TRIGGER_HIGH_DELAY);
        gpio_set_level(ESP_HCSR04_TRIGGER_PIN, 0);

        // Previous ping isn't ended
        if (gpio_get_level(ESP_HCSR04_ECHO_PIN))
            return_value = ESP_ERR_ULTRASONIC_PING;
        

        // Wait for echo
        echo_start = esp_timer_get_time();
        while (!gpio_get_level(ESP_HCSR04_ECHO_PIN) && (return_value == ESP_OK)) {
            time = esp_timer_get_time();
            if ( time - echo_start >= PING_TIMEOUT)
                return_value = ESP_ERR_ULTRASONIC_PING;
        }

        // Echo deteced wait for end of echo
        echo_start = esp_timer_get_time();
        time = echo_start;
        while ((gpio_get_level(ESP_HCSR04_ECHO_PIN)) && (return_value == ESP_OK)) {
            time = esp_timer_get_time();

            if (time - echo_start >= max_time_us) { 
                return_value = ESP_ERR_ULTRASONIC_ECHO_TIMEOUT;
            }
        }    
        portEXIT_CRITICAL(&mux);
        *time_us = time - echo_start;
    }
    else {
        return_value = ESP_ERR_INVALID_ARG;  // Invalid argument
    }
    return return_value;
}

/*
   API Interface functions
*/
esp_err_t UltrasonicInit(void)
{
    gpio_set_direction(ESP_HCSR04_TRIGGER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction( ESP_HCSR04_ECHO_PIN, GPIO_MODE_INPUT);
    gpio_set_level(ESP_HCSR04_TRIGGER_PIN, 0);

    return ESP_OK;
}


esp_err_t UltrasonicMeasure(uint32_t max_distance, uint32_t *distance)
{
    uint32_t    time_us;
    esp_err_t   return_value = ESP_OK;

    if (distance != NULL) {
        return_value = ultrasonic_measure_raw(max_distance * ROUNDTRIP_CM, &time_us);
        *distance = time_us / ROUNDTRIP_CM;
    }
    else {
        return_value = ESP_ERR_INVALID_ARG;
    }
    return return_value;
}


void UltrasonicAssert(esp_err_t error_code)
{
    if (error_code != ESP_OK) {
        ESP_LOGI(log_tag, "Measurement error: %x\n", error_code);
    }
}

/**
 * HC-SR04 sensor task
 */

static void HCSR04_task(void *pvParameter)
{
    UltrasonicInit();
    printf("Starting HC-SR04 task \n\n");
    esp_err_t return_value = ESP_OK;

    // create variable which stores the measured distance
    static uint32_t distance = 0;

    QueueHandle_t SSD1306_queue = ssd1306_get_queue_handle();

    for (;;)
    {
        return_value = UltrasonicMeasure(250, &distance);

        UltrasonicAssert(return_value);
        if (return_value == ESP_OK) {
            printf ("Distance: %ld\n\n", distance);
            ssd1306_queue_message_t msg = {.msgID = HEIGHT_FLOAT ,.x = 0, .y = 0, .height_mes=distance};
            xQueueSend(SSD1306_queue, &msg, portMAX_DELAY);
        }    
    
        // 0,5 second delay before starting new measurement
        
        //strncpy(msg.text, "Hello!", sizeof(msg.text));
        

        vTaskDelay(60 / portTICK_PERIOD_MS);

    }
    
}

void HCSR04_task_start(void)
{
    xTaskCreatePinnedToCore(&HCSR04_task, "HRCSR04_task", HCSR04_TASK_STACK_SIZE, NULL, HCSR04_TASK_PRIORITY, NULL, HCSR04_TASK_CORE_ID);
}