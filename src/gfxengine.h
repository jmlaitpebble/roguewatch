//
// gfxengine
//
// RAM use: 16 bytes

#pragma once

#include "mytypes.h"
#include "glbdef.h"

extern int 	gfx_cursorx;
extern int 	gfx_cursory;
extern u8	gfx_attr;

#define GFX_TEXTWIDTH	18
#define GFX_TEXTHEIGHT	14

// These are special symbols in our character set.  The SYMBOLSTRING
// version is "".
#define SYMBOL_HEART		'H'
#define SYMBOLSTRING_HEART	"H"
#define SYMBOL_MAGIC		'M'
#define SYMBOLSTRING_MAGIC	"M"
#define SYMBOL_LEFT		'<'
#define SYMBOLSTRING_LEFT	"<"
#define SYMBOL_RIGHT		'>'
#define SYMBOLSTRING_RIGHT	">"
#define SYMBOL_UP		'^'
#define SYMBOLSTRING_UP		"^"
#define SYMBOL_DOWN		'v'
#define SYMBOLSTRING_DOWN	"v"
#define SYMBOL_NEXT		';'
#define SYMBOLSTRING_NEXT	";"
#define SYMBOL_DLEVEL		'D'
#define SYMBOLSTRING_DLEVEL	"D"
#define SYMBOL_AC		'A'
#define SYMBOLSTRING_AC		"A"
#define SYMBOL_TRIDUDE		'Y'
#define SYMBOLSTRING_TRIDUDE	"Y"
#define SYMBOL_CURSOR		'#'
#define SYMBOLSTRING_CURSOR	"#"
#define SYMBOL_UNIQUE		'!'
#define SYMBOLSTRING_UNIQUE	"!"

void
gfx_reset();

void gfx_putcharCD(int x, int y, u8 c, u8 attr);
void gfx_putchar(int x, int y, u8 c);
void gfx_writecharCD(u8 c, u8 attr);
void gfx_writechar(u8 c);
void gfx_writestringCD(const char *s, u8 attr);
void gfx_writestring(const char *s);
void gfx_writeint(int value);

inline void
gfx_setcursor(int x, int y)
{
    gfx_cursorx = x;
    gfx_cursory = y;
}

inline void
gfx_setattr(u8 attr)
{
    gfx_attr = attr;
}
