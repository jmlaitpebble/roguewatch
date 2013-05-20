//
// Message functions
//
//
// RAM used: 37 bytes
//

#include "mytypes.h"
#include "msg.h"
#include "gfxengine.h"

u8 msg_data[MSG_MAXLEN];
u8 msg_length;

void
msg_clear()
{
    msg_length = 0;
}

bool
msg_empty()
{
    if (msg_length)
		return false;
    return true;
}

void
msg_report(const char *s)
{
    while (*s && msg_length < MSG_MAXLEN)
    {
		msg_data[msg_length++] = *s++;
    }
}

void
msg_draw()
{
    gfx_setcursor(0, 0);
    int			off;
    for (off = 0; off < msg_length; off++)
		gfx_writechar(msg_data[off]);

    for (; off < MSG_MAXLEN; off++)
		gfx_writechar(' ');
}

void
msg_endline()
{
    while (msg_length % MSG_WIDTH)
		msg_data[msg_length++] = ' ';
}
