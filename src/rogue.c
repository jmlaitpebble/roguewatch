///
/// Game like functions
///
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "mytypes.h"
#include "gfxengine.h"
#include "mob.h"
#include "map.h"
#include "msg.h"
#include "rand.h"
	
inline void draw_time()
{
	PblTm	time;
	
	get_time(&time);
	
    char		buf[12];
	
	buf[0] = time.tm_hour / 10 + '0';
	buf[1] = time.tm_hour % 10 + '0';
	buf[2] = ':';
	buf[3] = time.tm_min / 10 + '0';
	buf[4] = time.tm_min % 10 + '0';
	buf[5] = ':';
	buf[6] = time.tm_sec / 10 + '0';
	buf[7] = time.tm_sec % 10 + '0';
	buf[8] = 0;
	
	msg_report("    ");
    msg_report(buf);
    msg_endline();
}

void
status_draw()
{
    gfx_setcursor(0, 12);
    gfx_writestring("  ");
	switch (glb_roguedir)
	{
		case 0:
			gfx_writestring("<-");
			break;
		case 1:
			gfx_writestring("->");
			break;
		case 2:
			gfx_writestring("/\\");
			break;
		case 3:
			gfx_writestring("\\/");
			break;
		case 4:
			gfx_writestring("..");
			break;
	}
    gfx_writechar(' ');
    gfx_setattr(ATTR_RED);
    gfx_writechar(SYMBOL_HEART);
    gfx_setattr(ATTR_WHITE);
	gfx_writeint(mob_info[0].hp);
	gfx_writechar(' ');
    gfx_setattr(ATTR_GOLD);
    gfx_writechar('$');
    gfx_setattr(ATTR_WHITE);
	gfx_writeint(glb_gold);
	gfx_writechar(' ');

	gfx_writechar(SYMBOL_DLEVEL);
	gfx_writeint(glb_depth);
	gfx_writechar(' ');
}

void
rogue_reset()
{
    mob_info[0].hp = 10;
    glb_gold = 0;
    glb_depth = 1;
	map_build(glb_depth);
}

void
rogue_draw()
{
    POS		apos;

    map_findfirst(TILE_AVATAR, &apos);
    map_buildfov(apos);
    map_draw(apos);

    if (msg_empty())
    {
		draw_time();
    }
	
    msg_draw();
    status_draw();
}

void
rogue_tick(int dirkey)
{
    // Locate the avatar
    bool	didmove = false;
    u8		c;
    POS			apos;
    POS			mpos;
	
	static const int rotatebackward[4] = 
	{
		3,
		2,
		0,
		1
	};
	static const int rotateforward[4] = 
	{
		2,
		3,
		1,
		0
	};

    msg_clear();
	
    if (!map_findfirst(TILE_AVATAR, &apos))
	{
		// Dead, rebuild the map!
		rogue_reset();
		rogue_draw();
		return;
	}
	
	if (dirkey < 0)
		glb_roguedir = rotatebackward[glb_roguedir];
	else if (dirkey > 0)
		glb_roguedir = rotateforward[glb_roguedir];
	else
		didmove = true;
	glb_roguedir &= 3;

    if (didmove)
	{
		const s16		dirmap[5] = 
				{ DIRXY(-1, 0),
				  DIRXY(1, 0),
				  DIRXY(0, -1),
				  DIRXY(0, 1),
				  0
				};
		didmove = action_bump(apos, dirmap[glb_roguedir]);
	}

    if (didmove)
	{
		for (int mobslot = 1; mobslot < MOB_MAXIMUM; mobslot++)
		{
			if (mob_info[mobslot].hp &&
				map_findfirst(mobslot + TILE_AVATAR, &mpos))
			{
				ai_charge(mpos, apos);
			}
		}
	}

    rogue_draw();
}