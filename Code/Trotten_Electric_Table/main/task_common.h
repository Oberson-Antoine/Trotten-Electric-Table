/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-14 18:41:20                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-28 21:20:12                              *
 * @FilePath              : Trotten_Electric_Table/main/task_common.h        *
 ****************************************************************************/


#pragma once

//Wifi application task
#define WIFI_APP_TASK_STACK_SIZE            4096
#define WIFI_APP_TASK_PRIORITY              5
#define WIFI_APP_TASK_CORE_ID               0

//Http server task
#define HTTP_SERVER_TASK_STACK_SIZE               8192
#define HTTP_SERVER_TASK_PRIORITY           4       //1 priority level under the wifi task
#define HTTP_SERVER_TASK_CORE_ID            0

//http server monitor task
#define HTTP_SERVER_MONITOR_STACK_SIZE      4096
#define HTTP_SERVER_MONITOR_PRIORITY        3
#define HTTP_SERVER_MONITOR_CORE_ID         0

// DHT22 sensor task
// #define DHT22_TASK_STACK_SIZE               4096
// #define DHT22_TASK_PRIORITY                 5
// #define DHT22_TASK_CORE_ID                  1

//HC-SR04 TASK

#define HCSR04_TASK_STACK_SIZE               3072 
#define HCSR04_TASK_PRIORITY                 5
#define HCSR04_TASK_CORE_ID                  1


//ACS712 TASK

#define ACS712_TASK_STACK_SIZE               2048
#define ACS712_TASK_PRIORITY                 6
#define ACS712_TASK_CORE_ID                  1

//SSD1306 TASK
#define SSD1306_TASK_STACK_SIZE               4096
#define SSD1306_TASK_PRIORITY                 4
#define SSD1306_TASK_CORE_ID                  1

//SSD1306 M