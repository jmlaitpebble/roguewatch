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
	
void
status_draw()
{
	const u8 dirsymbol[5] =
	{
		SYMBOL_LEFT,
		SYMBOL_RIGHT,
		SYMBOL_UP,
		SYMBOL_DOWN,
	};
    gfx_setcursor(0, 12);
    gfx_writechar(dirsymbol[glb_roguedir]);
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
}

void
rogue_draw()
{
    POS		apos;

    map_findfirst(TILE_AVATAR, &apos);
    map_buildfov(apos);
    map_draw(apos);

    msg_draw();
    status_draw();
}

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
	
    msg_report(buf);
    msg_endline();
}

void
rogue_tick()
{
    // Locate the avatar
    bool	didmove = false;
    u8		c;

    msg_clear();

	glb_roguedir = rand_choice(4);
	didmove = true;

    POS			apos;
    POS			mpos;

    if (didmove && map_findfirst(TILE_AVATAR, &apos))
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

    if (msg_empty())
    {
		draw_time();
    }
    rogue_draw();
}