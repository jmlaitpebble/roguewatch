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
msg_reportint(int val)
{
	if (!val)
	{
		msg_report("0");
		return;
	}
	if (val < 0)
	{
		msg_report("-");
		val = -val;
	}
	char buf[10];
	int  len = 0;
	while (val)
	{
		buf[len++] = '0' + (val % 10);
		val /= 10;
	}
	for (int i = len; i --> 0; )
	{
		if (msg_length >= MSG_MAXLEN)
			break;
		msg_data[msg_length++] = buf[i];
	}
}


void
msg_draw()
{
    gfx_setcursor(0, 0);
    int			off;
    for (off = 0; off < msg_length; off++)
		gfx_writechar(msg_data[off]);

    for (; off < MSG_MINDRAWLEN; off++)
		gfx_writechar(' ');
}

void
msg_endline()
{
    while (msg_length < MSG_MAXLEN && (msg_length % MSG_WIDTH))
		msg_data[msg_length++] = ' ';
}
