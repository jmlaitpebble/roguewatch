//
// gfxengine
//
// RAM use: 16 bytes

#include "gfxengine.h"

int 	gfx_cursorx = 0;
int 	gfx_cursory = 0;
u8	gfx_attr = ATTR_WHITE;

extern char glbMapText[(GFX_TEXTWIDTH+1) * GFX_TEXTHEIGHT];

void
gfx_reset()
{
    gfx_cursorx = gfx_cursory = 0;
    gfx_attr = ATTR_WHITE;
}

void
gfx_putcharCD(int x, int y, u8 c, u8 attr)
{
	x += 2;
    // Clip.
    if (x < 0 || x > (GFX_TEXTWIDTH-1))
		return;
    if (y < 0 || y > (GFX_TEXTHEIGHT-1))
		return;
	
	glbMapText[x + y * (GFX_TEXTWIDTH+1)] = c;
}

void
gfx_putchar(int x, int y, u8 c)
{
    gfx_putcharCD(x, y, c, gfx_attr);
}

void
gfx_writecharCD(u8 c, u8 attr)
{
    if (c == '\n')
	{
		gfx_cursorx = 0;
		gfx_cursory++;
		if (gfx_cursory >= GFX_TEXTHEIGHT)
			gfx_cursory = 0;
		return;
	}
    if (c == '\b')
	{
		gfx_cursorx--;
		if (gfx_cursorx < 0)
		{
			gfx_cursorx = GFX_TEXTWIDTH-1;
			gfx_cursory--;
			if (gfx_cursory < 0)
				gfx_cursory = 0;
		}
		gfx_putcharCD(gfx_cursorx, gfx_cursory, ' ', attr);
		return;
	}
    gfx_putcharCD(gfx_cursorx, gfx_cursory, c, attr);
    gfx_cursorx++;
    if (gfx_cursorx >= GFX_TEXTWIDTH)
	{
		gfx_cursorx = 0;
		gfx_cursory++;
		if (gfx_cursory >= GFX_TEXTHEIGHT)
			gfx_cursory = 0;
	}
}

void
gfx_writechar(u8 c)
{
    gfx_writecharCD(c, gfx_attr);
}

void
gfx_writestringCD(const char *s, u8 attr)
{
    while (*s)
    {
		gfx_writecharCD(*s++, attr);
    }
}

void
gfx_writestring(const char *s)
{
    while (*s)
    {
		gfx_writechar(*s++);
    }
}

void
gfx_writeint(int val)
{
	if (val < 0)
	{
		gfx_writechar('-');
		val = -val;
	}
	if (!val)
		gfx_writechar('0');
	char buf[10];
	int  len = 0;
	while (val)
	{
		buf[len++] = '0' + (val % 10);
		val /= 10;
	}
	for (int i = len; i --> 0; )
	{
		gfx_writechar(buf[i]);
	}
}