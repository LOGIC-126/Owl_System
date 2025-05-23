#include "bsp_oled.h"

/* Write command */
#define SSD1306_WRITECOMMAND(command) ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data) ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))
/* Absolute value */


/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

typedef struct
{
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;


void OLED_Init(void)
{
    /* A little delay */
    uint32_t p = 2500;
    while (p > 0)
        p--;

    /* Init LCD */
    SSD1306_WRITECOMMAND(0xae);		      // display off
    SSD1306_WRITECOMMAND(0xa6);          // Set Normal Display (default)
    SSD1306_WRITECOMMAND(0xAE);        	// DISPLAYOFF
    SSD1306_WRITECOMMAND(0xD5);        	// SETDISPLAYCLOCKDIV
    SSD1306_WRITECOMMAND(0x80);        	// the suggested ratio 0x80
    SSD1306_WRITECOMMAND(0xA8);        	// SSD1306_SETMULTIPLEX
    SSD1306_WRITECOMMAND(0x1F);
    SSD1306_WRITECOMMAND(0xD3);        	// SETDISPLAYOFFSET
    SSD1306_WRITECOMMAND(0x00);         	// no offset
    SSD1306_WRITECOMMAND(0x40 | 0x0);  	// SETSTARTLINE
    SSD1306_WRITECOMMAND(0x8D);        	// CHARGEPUMP
    SSD1306_WRITECOMMAND(0x14);          // 0x014 enable, 0x010 disable
    SSD1306_WRITECOMMAND(0x20);          // com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5),
    SSD1306_WRITECOMMAND(0x02);          // 0x12 //128x32 OLED: 0x002,  128x32 OLED 0x012
    SSD1306_WRITECOMMAND(0xa1);          // segment remap a0/a1
    SSD1306_WRITECOMMAND(0xc8);          // c0: scan dir normal, c8: reverse
    SSD1306_WRITECOMMAND(0xda);
    SSD1306_WRITECOMMAND(0x02);          // com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5)
    SSD1306_WRITECOMMAND(0x81);
    SSD1306_WRITECOMMAND(0xcf);          // [2] set contrast control
    SSD1306_WRITECOMMAND(0xd9);
    SSD1306_WRITECOMMAND(0xf1);          // [2] pre-charge period 0x022/f1
    SSD1306_WRITECOMMAND(0xdb);
    SSD1306_WRITECOMMAND(0x40);          // vcomh deselect level
    SSD1306_WRITECOMMAND(0x2e);          // Disable scroll
    SSD1306_WRITECOMMAND(0xa4);          // output ram to display
    SSD1306_WRITECOMMAND(0xa6);          // none inverted normal display mode
    SSD1306_WRITECOMMAND(0xaf);          // display on
    
    /* Clear screen */
    SSD1306_Fill(SSD1306_COLOR_BLACK);

    /* Update screen */
    SSD1306_UpdateScreen();

    /* Set default values */
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;

    /* Initialized OK */
    SSD1306.Initialized = 1;

}

void SSD1306_UpdateScreen(void)
{
    uint8_t m;

    for (m = 0; m < 8; m++)
    {
        SSD1306_WRITECOMMAND(0xB0 + m);
        SSD1306_WRITECOMMAND(0x00);
        SSD1306_WRITECOMMAND(0x10);

        /* Write multi data */
        ssd1306_I2C_WriteMulti(SSD1306_I2C_ADDR, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
    }
}

void SSD1306_ToggleInvert(void)
{
    uint16_t i;

    /* Toggle invert */
    SSD1306.Inverted = !SSD1306.Inverted;

    /* Do memory toggle */
    for (i = 0; i < sizeof(SSD1306_Buffer); i++)
    {
        SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
    }
}

void SSD1306_Fill(SSD1306_COLOR_t color)
{
    /* Set memory */
    memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        return; // 出错，超出范围
    }

    /* 检查像素是否倒置 */
    if (SSD1306.Inverted)
    {
        color = (SSD1306_COLOR_t)!color;
    }

    /* 设置颜色 */
    if (color == SSD1306_COLOR_WHITE)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_GotoXY(uint16_t x, uint16_t y)
{
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

char SSD1306_Putc(char ch, FontDef_t *Font, SSD1306_COLOR_t color)
{
    uint32_t i, b, j;

    if (
        SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
        SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight))
    {
        return 0; // 出错，超出范围
    }

    for (i = 0; i < Font->FontHeight; i++)
    {
        b = Font->data[(ch - 32) * Font->FontHeight + i];
        for (j = 0; j < Font->FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)color);
            }
            else
            {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
            }
        }
    }

    /* Increase pointer */
    SSD1306.CurrentX += Font->FontWidth;

    /* Return character written */
    return ch;
}

char SSD1306_Puts(char *str, FontDef_t *Font, SSD1306_COLOR_t color)
{
    /* Write characters */
    while (*str)
    {
        /* Write character by character */
        if (SSD1306_Putc(*str, Font, color) != *str)
        {
            /* Return error */
            return *str;
        }

        /* Increase string pointer */
        str++;
    }

    /* Everything OK, zero should be returned */
    return *str;
}

void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c)
{
    int16_t dx, dy, sx, sy, err, e2, i, tmp;

    /* Check for overflow */
    if (x0 >= SSD1306_WIDTH)
    {
        x0 = SSD1306_WIDTH - 1;
    }
    if (x1 >= SSD1306_WIDTH)
    {
        x1 = SSD1306_WIDTH - 1;
    }
    if (y0 >= SSD1306_HEIGHT)
    {
        y0 = SSD1306_HEIGHT - 1;
    }
    if (y1 >= SSD1306_HEIGHT)
    {
        y1 = SSD1306_HEIGHT - 1;
    }

    dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
    dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = ((dx > dy) ? dx : -dy) / 2;

    if (dx == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Vertical line */
        for (i = y0; i <= y1; i++)
        {
            SSD1306_DrawPixel(x0, i, c);
        }

        /* Return from function */
        return;
    }

    if (dy == 0)
    {
        if (y1 < y0)
        {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0)
        {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Horizontal line */
        for (i = x0; i <= x1; i++)
        {
            SSD1306_DrawPixel(i, y0, c);
        }

        /* Return from function */
        return;
    }

    while (1)
    {
        SSD1306_DrawPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c)
{
    /* Check input parameters */
    if (
        x >= SSD1306_WIDTH ||
        y >= SSD1306_HEIGHT)
    {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH)
    {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT)
    {
        h = SSD1306_HEIGHT - y;
    }

    /* Draw 4 lines */
    SSD1306_DrawLine(x, y, x + w, y, c);         /* Top line */
    SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
    SSD1306_DrawLine(x, y, x, y + h, c);         /* Left line */
    SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c)
{
    uint8_t i;

    /* Check input parameters */
    if (
        x >= SSD1306_WIDTH ||
        y >= SSD1306_HEIGHT)
    {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH)
    {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT)
    {
        h = SSD1306_HEIGHT - y;
    }

    /* Draw lines */
    for (i = 0; i <= h; i++)
    {
        /* Draw lines */
        SSD1306_DrawLine(x, y + i, x + w, y + i, c);
    }
}



void SSD1306_ON(void)
{
    SSD1306_WRITECOMMAND(0x8D);
    SSD1306_WRITECOMMAND(0x14);
    SSD1306_WRITECOMMAND(0xAF);
}
void SSD1306_OFF(void)
{
    SSD1306_WRITECOMMAND(0x8D);
    SSD1306_WRITECOMMAND(0x10);
    SSD1306_WRITECOMMAND(0xAE);
}



void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count)
{
    OLED_i2cWrite(address, reg, count, data);
}

void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data)
{
    OLED_i2cWrite(address, reg, 1, &data);
}


/* OLED清除屏幕 */
void OLED_Clear(void)
{
    SSD1306_Fill(SSD1306_COLOR_BLACK);
}

/* 刷新OLED屏幕 */
void OLED_Refresh(void)
{
    SSD1306_UpdateScreen();
}

/* 写入字符 */
void OLED_Draw_String(char *data, uint8_t x, uint8_t y, bool clear, bool refresh)
{
    if (clear) OLED_Clear();
    SSD1306_GotoXY(x, y);
    SSD1306_Puts(data, &Font_7x10, SSD1306_COLOR_WHITE);
    if (refresh) OLED_Refresh();
}

/* 写入一行字符 */
void OLED_Draw_Line(char *data, uint8_t line, bool clear, bool refresh)
{
	
		if (line > 0 && line <= 3)
		{
				OLED_Draw_String(data, 0, 10 * (line - 1), clear, refresh);
		}
		
}

