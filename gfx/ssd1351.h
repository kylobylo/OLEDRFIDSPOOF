// SPDX-License-Identifier: GPL-2.0

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

#define SPI_NUM   spi0
#define SPI_DC    5
#define SPI_RST   4
#define SPI_SCK   6
#define SPI_DATA  7
#define SPI_SPEED 20000000 // 20MHz, about the limit in tests
#define HEIGHT    128
#define WIDTH     128

typedef enum scroll_direction
{
    SCROLL_LEFT,
    SCROLL_RIGHT,
    SCROLL_UP,
    SCROLL_DOWN
} scroll_direction;

typedef enum buffer_type
{
    BUFFER_TYPE_DISPLAY,
    BUFFER_TYPE_SCROLL
} buffer_type;

typedef enum write_type
{
    WRITE_COMMAND = 0,
    WRITE_DATA = 1
} write_type;

typedef enum ssd_commands
{
    SSD_CMD_SETCOLUMN     = 0x15,
    SSD_CMD_WRITERAM      = 0x5C,
    SSD_CMD_READRAM       = 0x5D,
    SSD_CMD_SETROW        = 0x75,
    SSD_CMD_SCROLL_SETUP  = 0x96,
    SSD_CMD_SCROLL_START  = 0x9E,
    SSD_CMD_SCROLL_STOP   = 0x9F,
    SSD_CMD_SETREMAP      = 0xA0,
    SSD_CMD_STARTLINE     = 0xA1,
    SSD_CMD_DISPLAYOFFSET = 0xA2,
    SSD_CMD_PIXELSOFF     = 0xA4, // All off
    SSD_CMD_PIXELSON      = 0xA5, // All on
    SSD_CMD_NONINVERT     = 0xA6,
    SSD_CMD_INVERT        = 0xA7,
    SSD_CMD_FUNCTIONSEL   = 0xAB,
    SSD_CMD_DISPLAYOFF    = 0xAE,
    SSD_CMD_DISPLAYON     = 0xAF,
    SSD_CMD_PRECHARGE     = 0xB1,
    SSD_CMD_ENHANCE       = 0xB2,
    SSD_CMD_CLOCKDIV      = 0xB3,
    SSD_CMD_SETVSL        = 0xB4,
    SSD_CMD_SETGPIO       = 0xB5,
    SSD_CMD_PRECHARGE2    = 0xB6,
    SSD_CMD_GAMMA_TABLE   = 0xB8,
    SSD_CMD_RESET_GAMMA   = 0xB9,
    SSD_CMD_PRECHARGEV    = 0xBB,
    SSD_CMD_VCOMH         = 0xBE,
    SSD_CMD_CONTRASTABC   = 0xC1,
    SSD_CMD_CONTRASTMASTER= 0xC7,
    SSD_CMD_MUXRATIO      = 0xCA,
    SSD_CMD_COMMANDLOCK   = 0xFD
} ssd_commands;

typedef enum font {
    FONT_IBM,
    FONT_LARGE,
    FONT_LARGE2,
    FONT_TOPAZ
} font;

typedef struct point_t
{
    uint8_t x;
    uint8_t y;
} point_t;

void ssd1351_init();
void ssd1351_clear();
void ssd1351_refresh();
void ssd1351_write_text(buffer_type type, const char *text, point_t pos, uint16_t colour, font font_type, bool transparent, bool invert);
void ssd1351_scroll_unbuffered(uint8_t amount);
uint16_t ssd1351_make_colour(uint8_t red, uint8_t green, uint8_t blue);
void ssd1351_set_contrast(uint8_t value);

void ssd1351_setup_scroll_buffered();
void ssd1351_scroll_buffered(uint8_t amount, scroll_direction direction);
void ssd1351_load_image(buffer_type type, const uint8_t *img_data, point_t pos, point_t size);
void ssd1351_load_image_1bpp(buffer_type type, const uint8_t *img_data, const unsigned int img_len);
void ssd1351_fill_colour(buffer_type type, uint16_t colour);
void ssd1351_draw_pixel(buffer_type type, uint16_t colour, point_t pos);
void ssd1351_draw_line(buffer_type type, uint16_t colour, point_t start, point_t end);
void ssd1351_draw_rectangle(buffer_type type, uint16_t colour, point_t start, point_t end, bool filled);
void ssd1351_draw_circle(buffer_type type, uint16_t colour, point_t pos, uint8_t r, bool filled);
void ssd1351_draw_triangle(buffer_type type, uint16_t colour, point_t p1, point_t p2, point_t p3, bool filled);
uint8_t* ssd1351_resize_image(const uint8_t *image, point_t old_size, point_t new_size);
void ssd1351_fps_counter(bool enable);
