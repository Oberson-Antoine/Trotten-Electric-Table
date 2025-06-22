/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-06-19 14:07:17                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-06-20 16:44:57                              *
 * @FilePath              : Trotten_Electric_Table/main/ssd1306.c            *
 ****************************************************************************/

#include <esp_lcd_io_i2c.h>
#include "esp_lcd_panel_ssd1306.h"
#include "esp_lcd_panel_ops.h"
#include <string.h>
#include "task_common.h"

#include "ssd1306.h"
#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "font16x16.h"

#define CHAR_WIDTH 16
#define CHAR_HEIGHT 16

uint8_t ssd1306_framebuffer[WIDTH * HEIGHT / 8] = {0}; // 512 bytes for SSD1306
esp_lcd_panel_handle_t ssd1306_panel_handle;
static QueueHandle_t ssd1306_display_queue = NULL;

/**
 * Bitmap font
 */

 // Contains an 8x8 font map for unicode points U+0000 - U+007F (basic latin)
const uint8_t font8x8_basic[96][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (;)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};

/**
 * Removes the content of the pannel 
 */

 void ssd1306_erase()
 {
    memset(ssd1306_framebuffer, 0, sizeof(ssd1306_framebuffer)); // Clear framebuffer
    //write the empty framebuffer to the screen
    esp_lcd_panel_draw_bitmap(ssd1306_panel_handle, 0, 0, WIDTH, HEIGHT, ssd1306_framebuffer);
 }

// Set a pixel in the framebuffer
void set_pixel(int x, int y, bool on) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    int byte_index = x + (y / 8) * WIDTH;
    if (on) ssd1306_framebuffer[byte_index] |= (1 << (y % 8));
    else    ssd1306_framebuffer[byte_index] &= ~(1 << (y % 8));
}

// Draw a single 5x7 char at (x, y)
void draw_char(int x, int y, char c) 
{
    //int i,j;

    // Convert the character to an index
    c = c & 0x7F;
    if (c < ' ') { //if unknown convert it to a space
        c = 0;
    } 
    
    else 
    { //if known offsets the space index
        // const uint8_t *bitmap = font16x16[c -32]; // Extracting the font "c" row of the character 
        // for (int col = 0; col < CHAR_WIDTH; col++){
        //     for (int row = 0; row < CHAR_HEIGHT; row++){
        //         // Each column is stored in 2 bytes (16 bits)
        //         int byte_index = col * 2 + (row/8); // as row is int, from 0-7 /8 = 0 and 8-15 = 1, col * 2 cause the each column uses 2 bytes
        //         int bit_index = row % 8; // as we loop till 16 and a byte is 8 bit long, the modulo is used like that : 1 % 8 = 1 2%8 = 2 ... 7 % 8 = 7 then 8 % 8 = 0
        //         int pixel_on = (bitmap[byte_index] >> bit_index) & 0x01; //bitmap[byte_index] selects the byte, then the bit shift the bits so that the bit we read is the last one
        //                                                                  // & 0x01 masks out all other bits, leaving only the value (0 or 1) of the pixel.
        //         if (pixel_on) {
        //             int fb_x = x + col; // position x in the framebuffer where the pixel shoud go
        //             int fb_y = y + row; // position y in the framebuffer where the pixel shoud go
        //             uint16_t fb_index = fb_x + (fb_y / 8) * WIDTH; // for the byte that contains the pixel at (fb_x, fb_y). 
        //                                                            //fb_y / 8 selects the page (group of 8 vertical pixels).
        //                                                            // * WIDTH moves to the correct page.
        //                                                            // fb_x selects the column.

        //             ssd1306_framebuffer[fb_index] |= (1 << (fb_y % 8)); // Sets the bit in the framebuffer byte that corresponds to the pixel’s Y position within the page.
        //                                                                 // fb_y % 8 gives the bit position (0–7) within the byte.
        //                                                                 // |= (1 << ...) sets that bit to 1, turning the pixel ON.
        //         }
        //     }
        // }

        const uint8_t *bitmap = font16x16[c -32]; // Extracting the font "c" row of the character 
        for (int row = 0; row < CHAR_HEIGHT; row++){
            for (int col = 0; col < CHAR_WIDTH; col++){
                // Each column is stored in 2 bytes (16 bits)
                int byte_index = row * 2 + (col/8); // as row is int, from 0-7 /8 = 0 and 8-15 = 1, col * 2 cause the each column uses 2 bytes
                int bit_index = 7 - col % 8; // as we loop till 16 and a byte is 8 bit long, the modulo is used like that : 1 % 8 = 1 2%8 = 2 ... 7 % 8 = 7 then 8 % 8 = 0
                int pixel_on = (bitmap[byte_index] >> bit_index) & 0x01; //bitmap[byte_index] selects the byte, then the bit shift the bits so that the bit we read is the last one
                                                                         // & 0x01 masks out all other bits, leaving only the value (0 or 1) of the pixel.
                if (pixel_on) {
                    int fb_x = x + col; // position x in the framebuffer where the pixel shoud go
                    int fb_y = y + row; // position y in the framebuffer where the pixel shoud go
                    uint16_t fb_index = fb_x + (fb_y / 8) * WIDTH; // for the byte that contains the pixel at (fb_x, fb_y). 
                                                                   //fb_y / 8 selects the page (group of 8 vertical pixels).
                                                                   // * WIDTH moves to the correct page.
                                                                   // fb_x selects the column.

                    ssd1306_framebuffer[fb_index] |= (1 << (fb_y % 8)); // Sets the bit in the framebuffer byte that corresponds to the pixel’s Y position within the page.
                                                                        // fb_y % 8 gives the bit position (0–7) within the byte.
                                                                        // |= (1 << ...) sets that bit to 1, turning the pixel ON.
                }
            }
        }
        
    }

    
    // printf("Character: %c ", c);
    // printf("ASCII value: %d\n", c);
    
}

void draw_char_scale2(int x, int y, char c) 
{
    c = c & 0x7F;
    if (c < ' ') {
        c = 0;
    } 
    else 
    {
        const uint8_t *bitmap = font16x16[c - 32];
        for (int row = 0; row < CHAR_HEIGHT; row++) {
            for (int col = 0; col < CHAR_WIDTH; col++) {
                int byte_index = row * 2 + (col / 8);
                int bit_index = 7 - (col % 8);
                int pixel_on = (bitmap[byte_index] >> bit_index) & 0x01;
                if (pixel_on) {
                    int fb_x = x + col;
                    // Vertical scaling: draw each row twice
                    for (int dy = 0; dy < 2; dy++) {
                        int fb_y = y + row * 2 + dy;
                        uint16_t fb_index = fb_x + (fb_y / 8) * WIDTH;
                        ssd1306_framebuffer[fb_index] |= (1 << (fb_y % 8));
                    }
                }
            }
        }
    }
}

// Draw string left to right
void draw_string(int x, int y, const char* text) {
    memset(ssd1306_framebuffer, 0, sizeof(ssd1306_framebuffer)); // Clear framebuffer
    while (*text) {
        draw_char_scale2(x, y, *text++);
        x += 12; // 16 pixels + 1 spacing
    }
}


void ssd1306_task()
{
    i2c_master_bus_handle_t i2c_bus_handle = get_i2c_bus(); // get the handle from the init
    SemaphoreHandle_t i2c_bus_mutex_handle = get_i2c_mutex(); // the mutex from the init^




    esp_lcd_panel_io_handle_t ssd1306_io_handle;
    esp_lcd_panel_io_i2c_config_t ssd1306_io_config = {
        .dev_addr = 0x3C,
        .control_phase_bytes = 1,
        .dc_bit_offset = 6,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .flags = {
            .dc_low_on_data = 0,  // default
        },
        .scl_speed_hz = 100000,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_handle, &ssd1306_io_config, &ssd1306_io_handle));



    esp_lcd_panel_ssd1306_config_t ssd1306_screen_vendor_config = {
        .height = 32, //my screen is the small version
    };

        esp_lcd_panel_dev_config_t ssd1306_screen_config = {
        .bits_per_pixel = 1, // SSD1306 is monochrome
        .reset_gpio_num = -1, // No reset pin? Use -1
        .color_space = ESP_LCD_COLOR_SPACE_MONOCHROME,
        .vendor_config = &ssd1306_screen_vendor_config,
    };


    
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(ssd1306_io_handle, &ssd1306_screen_config, &ssd1306_panel_handle));
    esp_lcd_panel_reset(ssd1306_panel_handle);
    esp_lcd_panel_init(ssd1306_panel_handle);
    esp_lcd_panel_mirror(ssd1306_panel_handle, 1, 1);



    // 5. Clear the screen
    esp_lcd_panel_disp_on_off(ssd1306_panel_handle, true);
    char buffer[7];
    sprintf(buffer,"%d cm",232);
    draw_string(0, 0, buffer);
    esp_lcd_panel_draw_bitmap(ssd1306_panel_handle, 0, 0, WIDTH, HEIGHT, ssd1306_framebuffer);
    
}


void start_display_task(void) {
    display_queue = xQueueCreate(5, sizeof(display_msg_t));
    xTaskCreatePinnedToCore(ssd1306_task, "ssd1306 Task", SSD1306_TASK_STACK_SIZE, NULL, SSD1306_TASK_PRIORITY, NULL, SSD1306_TASK_CORE_ID);
}
