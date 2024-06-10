// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>
#include <math.h>
#include "ssd1351.h"
#include "font8x8_basic.h"
#include "font12x16.h"
#include "font8x8_topaz.h"
#include "font16x16.h"

// Two buffers, one is being dumped to the OLED via DMA whilst the other is
// being prepared by the main CPU core for the next frame.
uint8_t oled_dma_1[HEIGHT * WIDTH * 2] = {0x00};
uint8_t oled_dma_2[HEIGHT * WIDTH * 2] = {0x00};
// Buffered data to scroll into frame buffer
uint8_t scroll_buffer[HEIGHT * WIDTH * 2] = {0x00};
uint16_t scroll_pos = 0;
uint8_t current_buffer = 1;
uint8_t pos_x = 0;
uint8_t pos_y = 0;

bool show_fps = false;
uint16_t fps = 0;
uint16_t last_fps = 0;
struct repeating_timer timer;

static volatile uint dma_tx;
static dma_channel_config config;

static bool fps_callback(struct repeating_timer *t)
{
    last_fps = fps;
    fps = 0;
}

static void ssd1351_write(const uint8_t cmd, const uint8_t* data, size_t len)
{
    gpio_put(SPI_DC, WRITE_COMMAND);
    spi_write_blocking(SPI_NUM, &cmd, 1);
    if (len)
    {
        gpio_put(SPI_DC, WRITE_DATA);
        spi_write_blocking(SPI_NUM, data, len);
    }
}

static void ssd1351_flush_dma(uint8_t *ptr, size_t len)
{
    // It is possible to call this whilst the DMA is still dumping the previous
    // frame. This blocks until we are ready to start the next frame.
    dma_channel_wait_for_finish_blocking(dma_tx);
    // 1ms pause because otherwise we can flip from data of the previous frame
    // to command of this one too quickly.
    sleep_ms(1);
    ssd1351_write(SSD_CMD_WRITERAM, NULL, 0);

    gpio_put(SPI_DC, WRITE_DATA);

    dma_channel_transfer_from_buffer_now(dma_tx, ptr, len);
    fps++;
}

static uint8_t *get_buffer(buffer_type type)
{
    // Return the pointer to the current buffer being written to.
    if (type == BUFFER_TYPE_SCROLL)
    {
        return scroll_buffer;
    }
    if (current_buffer == 1)
    {
        return oled_dma_1;
    }
    return oled_dma_2;
}

void ssd1351_init()
{
    uint8_t param[3];

    // Setup SPI
    spi_init(SPI_NUM, SPI_SPEED);
    spi_set_format(SPI_NUM, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_DATA, GPIO_FUNC_SPI);

    // Setup GPIO
    gpio_init(SPI_DC);
    gpio_set_dir(SPI_DC, GPIO_OUT);
    gpio_put(SPI_DC, 1);

    gpio_init(SPI_RST);
    gpio_set_dir(SPI_RST, GPIO_OUT);

    // Power on reset
    gpio_put(SPI_RST, 1);
    sleep_ms(10);
    gpio_put(SPI_RST, 0);
    sleep_ms(500);
    gpio_put(SPI_RST, 1);
    gpio_put(SPI_DC, 0);

    // Init display

    // Unlock driver
    param[0] = 0x12;
    ssd1351_write(SSD_CMD_COMMANDLOCK, param, 1);

    // Unlock commands
    param[0] = 0xB1;
    ssd1351_write(SSD_CMD_COMMANDLOCK, param, 1);

    ssd1351_write(SSD_CMD_DISPLAYOFF, NULL, 0);

    // Max frequency, no divider. Fastest refresh rate.
    param[0] = 0xF0;
    ssd1351_write(SSD_CMD_CLOCKDIV, param, 1);

    // Effectively number of lines
    param[0] = 0x7F;
    ssd1351_write(SSD_CMD_MUXRATIO, param, 1);

    // Horizontal addressing, unmirrored, C->B->A colours, normal scan, 65K colours
    param[0] = 0x74;
    ssd1351_write(SSD_CMD_SETREMAP, param, 1);

    param[0] = 0x00;
    ssd1351_write(SSD_CMD_STARTLINE, param, 1);

    param[0] = 0x00;
    ssd1351_write(SSD_CMD_DISPLAYOFFSET, param, 1);

    // Disable GPIO
    param[0] = 0x00;
    ssd1351_write(SSD_CMD_SETGPIO, param, 1);

    // 16bit parallel interface
    param[0] = 0x01;
    ssd1351_write(SSD_CMD_FUNCTIONSEL, param, 1);

    ssd1351_write(SSD_CMD_NONINVERT, NULL, 0);

    // R G B contrast
    param[0] = param[1] = param[2] = 0xFF;
    ssd1351_write(SSD_CMD_CONTRASTABC, param, 3);

    // Max master contrast
    param[0] = 0x0F;
    ssd1351_write(SSD_CMD_CONTRASTMASTER, param, 1);

    param[0] = 0xA4;
    param[1] = 0x00;
    param[2] = 0x00;
    ssd1351_write(SSD_CMD_ENHANCE, param, 3);

    ssd1351_write(SSD_CMD_DISPLAYON, NULL, 0);

    sleep_ms(10);

    // Setup DMA transfers
    dma_tx = dma_claim_unused_channel(true);
    config = dma_channel_get_default_config(dma_tx);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    channel_config_set_dreq(&config, spi_get_index(SPI_NUM) ? DREQ_SPI1_TX : DREQ_SPI0_TX);
    dma_channel_configure(dma_tx, &config, &spi_get_hw(SPI_NUM)->dr, oled_dma_1, sizeof(oled_dma_1), false);

    // FPS counter
    add_repeating_timer_ms(1000, fps_callback, NULL, &timer);
}

void ssd1351_load_image_1bpp(buffer_type type, const uint8_t *img_data, const unsigned int img_len)
{
    uint8_t *buffer = get_buffer(type);
    for (int i = 0; i < img_len; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            uint8_t byte;
            if (((img_data[i] >> 7 - j) & 0x1))
            {
                byte = 0xff;
            }
            else
            {
                byte = 0x00;
            }
            buffer[((i*8)+j)*2] = byte;
            buffer[(((i*8)+j)*2)+1] = byte;
        }
    }
}

void ssd1351_load_image(buffer_type type, const uint8_t *img_data, point_t pos, point_t size)
{
    uint8_t *buffer = get_buffer(type);
    uint16_t start_pos = (pos.x * 2) + (pos.y * WIDTH * 2);
    if (size.x == WIDTH)
    {
        memcpy(buffer + start_pos, img_data, WIDTH * size.y * 2);
    }
    else
    {
        for(uint16_t i = 0; i < size.y; i++)
        {
            memcpy(buffer + start_pos + (WIDTH * 2 * i), img_data + (size.x * 2 * i), size.x * 2);
        }
    }
}

void ssd1351_clear()
{
    pos_x = 0;
    uint8_t *buffer = get_buffer(BUFFER_TYPE_DISPLAY);
    memset(buffer, 0, sizeof(oled_dma_1));
}

void ssd1351_fill_colour(buffer_type type, uint16_t colour)
{
    uint8_t *buffer = get_buffer(type);
    for(int i = 0; i < sizeof(oled_dma_1); i+=2)
    {
        memcpy(buffer + i, &colour, 2);
    }
}

inline void ssd1351_draw_pixel(buffer_type type, uint16_t colour, point_t pos)
{
    uint8_t *buffer = get_buffer(type);

    memcpy(buffer + (pos.y*WIDTH*2) + (pos.x*2), &colour, 2);
}

void ssd1351_scroll_unbuffered(uint8_t amount)
{
    amount *= 2; // 2 bytes per px.
    uint8_t copy[amount];
    uint8_t *buffer = get_buffer(BUFFER_TYPE_DISPLAY);
    for (uint8_t i = 0; i < HEIGHT; i++)
    {
        // Take the first two bytes making up each row (2 bytes per pixel),
        // shift everything on that row by two bytes and append the original
        // two bytes to the end. Effectively shifting the pixel from the left
        // side to the right on each row.
        memcpy(copy, buffer + (WIDTH*2*i), amount);
        memmove(buffer + (WIDTH*2*i), buffer + (WIDTH*2*i) + amount, (WIDTH*2)-amount);
        memcpy(buffer + (WIDTH*2*i) + (WIDTH*2) - amount, copy, amount);
    }
}

// Setup the buffer for an image to scroll into frame
void ssd1351_setup_scroll_buffered()
{
    scroll_pos = 0;
    memset(scroll_buffer, '\0', sizeof(scroll_buffer));
}

// Scroll from scroll buffer into frame
void ssd1351_scroll_buffered(uint8_t amount, scroll_direction direction)
{
    uint8_t *buffer = get_buffer(BUFFER_TYPE_DISPLAY);
    if (direction == SCROLL_LEFT)
    {
        amount *= 2; // 2 bytes per px.
        for (uint8_t i = 0; i < HEIGHT; i++)
        {
            memmove(buffer + (WIDTH*2*i), buffer + (WIDTH*2*i) + amount, (WIDTH*2)-amount);
            memcpy(buffer + (WIDTH*2*i) - amount, scroll_buffer + scroll_pos + (WIDTH*2*i), amount);
        }
    }
    else if (direction == SCROLL_RIGHT)
    {
        amount *= 2; // 2 bytes per px.
        for (uint8_t i = 0; i < HEIGHT; i++)
        {
            memmove(buffer + (WIDTH*2*i) + amount, buffer + (WIDTH*2*i), (WIDTH*2)-amount);
            memcpy(buffer + (WIDTH*2*i), scroll_buffer + (WIDTH*2) - scroll_pos + (WIDTH*2*i) - amount, amount);
        }
    }
    else if (direction == SCROLL_UP)
    {
        memmove(buffer, buffer + (WIDTH*2*amount), WIDTH*2*(HEIGHT-amount));
        memcpy(buffer + (WIDTH*2*(HEIGHT-amount)), scroll_buffer + (WIDTH*2*scroll_pos), WIDTH*2*amount);
    }
    else // SCROLL_DOWN
    {
        memmove(buffer + (WIDTH*2*amount), buffer, WIDTH*2*(HEIGHT-amount));
        memcpy(buffer, scroll_buffer + (WIDTH*2*(HEIGHT-scroll_pos-amount)), WIDTH*2*amount);
    }

    scroll_pos += amount;
}

static void ssd1351_put_char(buffer_type type, char c, uint16_t colour, font font_type, bool transparent, bool invert)
{
    uint8_t font_width, font_height;
    const unsigned char *font_ptr;
    if (font_type == FONT_LARGE)
    {
        font_width = 12;
        font_height = 16;
        font_ptr = (unsigned char*)font_12x16;
    }
    else if (font_type == FONT_LARGE2)
    {
        font_width = 15;
        font_height = 16;
        font_ptr = (unsigned char*)font_16x16;
        c -= 32;
    }
    else if (font_type == FONT_IBM)
    {
        font_width = 8;
        font_height = 8;
        font_ptr = (unsigned char*)font_8x8;
    }
    else
    {
        font_width = 8;
        font_height = 8;
        font_ptr = (unsigned char*)font_topaz;
        if (c < 32)
        {
            return;
        }
        c -= 32;
    }
    uint8_t font_width_bytes = (font_width + 8 - 1) / 8;
    uint8_t character[font_height * font_width_bytes];
    uint8_t *buffer = get_buffer(type);
    // Position already too far
    if ((pos_x + font_width) > WIDTH)
        return;

    memcpy(character, &font_ptr[(uint16_t)c * font_height * font_width_bytes], font_height * font_width_bytes);

    for (uint8_t i = 0; i < (font_height * font_width_bytes); i += font_width_bytes)
    {
        uint16_t pos_y_off = (WIDTH * (pos_y + (i / font_width_bytes)) * 2);
        // Create a 2 byte RGB565 pixel for every dot in the font.
        for (uint8_t j = 0; j < font_width; j++)
        {
            if ((character[i + (j / 8)] >> j - ((j / 8) * 8)) & 1)
            {
                if (invert)
                {
                    buffer[(pos_x * 2) + pos_y_off + (j*2)] ^= 0xff;
                    buffer[(pos_x * 2) + pos_y_off + (j*2)+1] ^= 0xff;
                }
                else
                {
                    memcpy(buffer + (pos_x * 2) + pos_y_off + (j*2), &colour, 2);
                }
            }
            else if (!transparent)
            {
                buffer[(pos_x * 2) + pos_y_off + (j*2)] = 0x0;
                buffer[(pos_x * 2) + pos_y_off + (j*2)+1] = 0x0;
            }

        }
    }

    pos_x += font_width;
}

void ssd1351_write_text(buffer_type type, const char *text, point_t pos, uint16_t colour, font font_type, bool transparent, bool invert)
{
    pos_x = pos.x;
    pos_y = pos.y;
    while (*text)
    {
        ssd1351_put_char(type, *text++, colour, font_type, transparent, invert);
    }
}

void ssd1351_write_text_centered(buffer_type type, const char *text, uint8_t y, uint16_t colour, font font_type, bool transparent, bool invert)
{

    pos_y = y;

    uint8_t font_width, font_height;

    if (font_type == FONT_LARGE)
    {
        font_width = 12;
        font_height = 16;

    }
    else if (font_type == FONT_LARGE2)
    {
        font_width = 15;
        font_height = 16;

    }
    else if (font_type == FONT_IBM)
    {
        font_width = 8;
        font_height = 8;

    }
    else
    {
        font_width = 8;
        font_height = 8;
    }

    short size;

    size = strlen(text);
    pos_x = 127 - ((size + 1) * font_width);

    while (*text)
    {
        ssd1351_put_char(type, *text++, colour, font_type, transparent, invert);
    }
}

void ssd1351_fps_counter(bool enable)
{
    show_fps = enable;
}

void ssd1351_refresh()
{
    char fps_text[5];
    if (show_fps)
    {
        sprintf(fps_text, "%d", last_fps);
        ssd1351_write_text(BUFFER_TYPE_DISPLAY, fps_text, (point_t){0, 0}, 65535, FONT_IBM, false, false);
    }
    // Flush one buffer to the DMA, copy it to the other buffer and set that
    // one as the current one to write modify.
    if (current_buffer == 1)
    {
        ssd1351_flush_dma(oled_dma_1, sizeof(oled_dma_1));
        memcpy(oled_dma_2, oled_dma_1, sizeof(oled_dma_1));
        current_buffer = 2;
    }
    else
    {
        ssd1351_flush_dma(oled_dma_2, sizeof(oled_dma_2));
        memcpy(oled_dma_1, oled_dma_2, sizeof(oled_dma_2));
        current_buffer = 1;
    }
}

// Convert 24bit RGB to 16bit RGB565 (note this will need byte swapping when
// sending to the display)
inline uint16_t ssd1351_make_colour(uint8_t red, uint8_t green, uint8_t blue)
{
    uint16_t ret;
    ret = (red & 0xf8) << 8;
    ret |= (green & 0xfc) << 3;
    ret |= (blue & 0xf8) >> 3;

    ret = __builtin_bswap16(ret);

    return ret;
}

void ssd1351_set_contrast(uint8_t value)
{
    uint8_t param[3];
    dma_channel_wait_for_finish_blocking(dma_tx);
    sleep_ms(1);
    param[0] = param[1] = param[2] = value;
    ssd1351_write(SSD_CMD_CONTRASTABC, param, 3);
}

void ssd1351_draw_line(buffer_type type, uint16_t colour, point_t start, point_t end)
{
    bool steep = false;
    if (abs(start.x - end.x) < abs(start.y - end.y))
    {
        uint8_t tmp;
        tmp = start.y;
        start.y = start.x;
        start.x = tmp;

        tmp = end.y;
        end.y = end.x;
        end.x = tmp;
        steep = true;
    }
    if (start.x > end.x)
    {
        uint8_t tmp = end.x;
        end.x = start.x;
        start.x = tmp;

        tmp = end.y;
        end.y = start.y;
        start.y = tmp;
    }

    int dx = end.x - start.x;
    int dy = end.y - start.y;
    float derror = fabs(dy / (float)dx);
    float error = 0;
    point_t pos = {0, start.y};

    for (pos.x = start.x; pos.x <= end.x; pos.x++)
    {
        if (steep)
        {
            point_t tmp = {pos.y, pos.x};
            ssd1351_draw_pixel(type, colour, tmp);
        }
        else
        {
            ssd1351_draw_pixel(type, colour, pos);
        }
        error += derror;
        if (error > 0.5)
        {
            pos.y += (end.y > start.y ? 1 : -1);
            error -= 1;
        }
    }
}

void ssd1351_draw_rectangle(buffer_type type, uint16_t colour, point_t start, point_t end, bool filled)
{
    if (end.x < start.x)
    {
        uint8_t tmp = end.x;
        end.x = start.x;
        start.x = tmp;
    }

    if (end.y < start.y)
    {
        uint8_t tmp = end.y;
        end.y = start.y;
        start.y = tmp;
    }

    ssd1351_draw_line(type, colour, (point_t){start.x, start.y}, (point_t){end.x, start.y});
    ssd1351_draw_line(type, colour, (point_t){start.x, end.y}, (point_t){end.x, end.y});
    ssd1351_draw_line(type, colour, (point_t){start.x, start.y}, (point_t){start.x, end.y});
    ssd1351_draw_line(type, colour, (point_t){end.x, start.y}, (point_t){end.x, end.y});
    if (filled)
    {
        for (uint8_t x = start.x + 1; x < end.x; x++)
        {
            ssd1351_draw_line(type, colour, (point_t){x, start.y}, (point_t){x, end.y});
        }
    }
}

// Based on http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
static void ssd1351_circle_points(buffer_type type, uint16_t colour, bool filled, point_t c, point_t off)
{
    if (filled)
    {
        ssd1351_draw_line(type, colour, (point_t){c.x + off.x, c.y + off.y}, (point_t){c.x - off.x, c.y + off.y});
        ssd1351_draw_line(type, colour, (point_t){c.x + off.x, c.y - off.y}, (point_t){c.x - off.x, c.y - off.y});
        ssd1351_draw_line(type, colour, (point_t){c.x + off.y, c.y + off.x}, (point_t){c.x - off.y, c.y + off.x});
        ssd1351_draw_line(type, colour, (point_t){c.x + off.y, c.y - off.x}, (point_t){c.x - off.y, c.y - off.x});
        return;
    }

    if (off.x == 0)
    {
        ssd1351_draw_pixel(type, colour, (point_t){c.x, c.y + off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x, c.y - off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x + off.y, c.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x - off.y, c.y});
    }
    else if (off.x == off.y)
    {
        ssd1351_draw_pixel(type, colour, (point_t){c.x + off.x, c.y + off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x - off.x, c.y + off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x + off.x, c.y - off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x - off.x, c.y - off.y});
    }
    else if (off.x < off.y)
    {
        ssd1351_draw_pixel(type, colour, (point_t){c.x + off.x, c.y + off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x - off.x, c.y + off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x + off.x, c.y - off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x - off.x, c.y - off.y});
        ssd1351_draw_pixel(type, colour, (point_t){c.x + off.y, c.y + off.x});
        ssd1351_draw_pixel(type, colour, (point_t){c.x - off.y, c.y + off.x});
        ssd1351_draw_pixel(type, colour, (point_t){c.x + off.y, c.y - off.x});
        ssd1351_draw_pixel(type, colour, (point_t){c.x - off.y, c.y - off.x});
    }
}

void ssd1351_draw_circle(buffer_type type, uint16_t colour, point_t pos, uint8_t r, bool filled)
{
    int p = (5 - r * 4) / 4;
    uint8_t yoff = r;
    uint8_t xoff = 0;
    while (xoff < yoff)
    {
        xoff++;
        if (p < 0)
        {
            p += 2*xoff+1;
        }
        else
        {
            yoff--;
            p += 2*(xoff-yoff)+1;
        }
        ssd1351_circle_points(type, colour, filled, pos, (point_t){xoff, yoff});
    }
    if (filled)
    {
        ssd1351_draw_line(type, colour, (point_t){pos.x + r, pos.y}, (point_t){pos.x - r, pos.y});
    }
}

// Based on http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
static void ssd1351_fill_bottom_flat_triangle(buffer_type type, uint16_t colour, point_t p1, point_t p2, point_t p3)
{
    float invslope1 = ((float)p2.x - (float)p1.x) / ((float)p2.y - (float)p1.y);
    float invslope2 = ((float)p3.x - (float)p1.x) / ((float)p3.y - (float)p1.y);

    float curx1 = p1.x;
    float curx2 = p1.x;

    for (int scanline = p1.y; scanline <= p2.y; scanline++)
    {
        ssd1351_draw_line(type, colour, (point_t){(uint8_t)curx1, scanline}, (point_t){(uint8_t)curx2, scanline});
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

static void ssd1351_fill_top_flat_triangle(buffer_type type, uint16_t colour, point_t p1, point_t p2, point_t p3)
{
    float invslope1 = ((float)p3.x - (float)p1.x) / ((float)p3.y - (float)p1.y);
    float invslope2 = ((float)p3.x - (float)p2.x) / ((float)p3.y - (float)p2.y);

    float curx1 = p3.x;
    float curx2 = p3.x;

    for (int scanline = p3.y; scanline > p1.y; scanline--)
    {
        ssd1351_draw_line(type, colour, (point_t){(uint8_t)curx1, scanline}, (point_t){(uint8_t)curx2, scanline});
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

static void swap_coords(point_t *p1, point_t *p2)
{
    point_t tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void ssd1351_draw_triangle(buffer_type type, uint16_t colour, point_t p1, point_t p2, point_t p3, bool filled)
{
    if (!filled)
    {
        ssd1351_draw_line(type, colour, p1, p2);
        ssd1351_draw_line(type, colour, p1, p3);
        ssd1351_draw_line(type, colour, p2, p3);
        return;
    }

    // Sort so p1.y is the lowest
    if (p1.y > p2.y)
    {
        swap_coords(&p2, &p1);
    }
    if (p2.y > p3.y)
    {
        swap_coords(&p2, &p3);
    }
    if (p1.y > p2.y)
    {
        swap_coords(&p2, &p1);
    }

    if (p2.y == p3.y)
    {
        ssd1351_fill_bottom_flat_triangle(type, colour, p1, p2, p3);
    }
    else if (p1.y == p2.y)
    {
        ssd1351_fill_top_flat_triangle(type, colour, p1, p2, p3);
    }
    else
    {
        point_t p4;
        p4.x = (uint8_t)(p1.x + ((float)(p2.y - p1.y) / (float)(p3.y - p1.y)) * (p3.x - p1.x));
        p4.y = p2.y;
        ssd1351_fill_bottom_flat_triangle(type, colour, p1, p2, p4);
        ssd1351_fill_top_flat_triangle(type, colour, p2, p4, p3);
    }
}

uint8_t* ssd1351_resize_image(const uint8_t *image, point_t old_size, point_t new_size)
{
    float x_ratio, y_ratio;

    x_ratio = (float)(old_size.x) / (float)(new_size.x);
    y_ratio = (float)(old_size.y) / (float)(new_size.y);

    for(uint8_t i = 0; i < new_size.y; i++)
    {
        uint16_t old_y = (uint16_t)((float)i * y_ratio) * old_size.x;
        uint16_t new_y = i * new_size.x;
        for (uint8_t j = 0; j < new_size.x; j++)
        {
            uint16_t old_offset = (uint16_t)((old_y + (j * x_ratio))) * 2;
            uint16_t new_offset = (new_y + j) * 2;
            memcpy(scroll_buffer + new_offset, image + old_offset, 2);
        }
    }

    return scroll_buffer;
}
