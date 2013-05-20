//
// map files
//

#include "mytypes.h"
#include "glbdef.h"
#include "gfxengine.h"
#include "map.h"
#include "mob.h"
#include "rand.h"

u8 map_tiledata[MAPSIZE];
u8 map_flagdata[MAPSIZE];

bool map_istransparent(POS p)
{
    if (TILE_ISMOB(MAP(p)) ||
		glb_tiledefs[MAP(p)].istransparent)
		return true;
    return false;
}

POS
deltaXY(POS p, int dx, int dy)
{
    p += dx + dy * MAPWIDTH;
    p &= MAPMASK;
    return p;
}

POS
delta(POS p, DIR d)
{
    p += d;
    p &= MAPMASK;
    return p;
}

void
map_clear(u8 tile)
{
    POS		p;
    FORALL_POS(p)
		MAP(p) = tile;
}

void
mapflag_clear(u8 flag)
{
    flag = ~flag;
    POS		p;
    FORALL_POS(p)
		map_flagdata[p] &= flag;
}

bool
map_find(u8 tile, POS *pos)
{
    int		nfound = 0;
    POS		p;

    FORALL_POS(p)
    {
		if (MAP(p) == tile)
		{
			nfound++;
			if (!rand_choice(nfound))
			{
				*pos = p;
			}
		}
    }
    return nfound != 0;
}

bool
map_findfirst(u8 tile, POS *pos)
{
    POS		p;

    FORALL_POS(p)
    {
		if (MAP(p) == tile)
		{
			*pos = p;
			return true;
		}
    }
    return false;
}

void
map_buildcave()
{
    map_clear(TILE_WALL);
    mapflag_clear(MAPFLAG_FOV | MAPFLAG_MAPPED);

    // Seed location
    MAP(0) = TILE_FLOOR;
    const int numdrunk = 20;
    const int drunklife = 20;

    for (int drunk = 0; drunk < numdrunk; drunk++)
    {
		POS		pos;
		int		dx, dy;
	
		map_find(TILE_FLOOR, &pos);
		for (int life = 0; life < drunklife; life++)
		{
			rand_dir(&dx, &dy);
			pos = deltaXY(pos, dx, dy);
			MAP(pos) = TILE_FLOOR;
		}
    }
}

void
map_build(int depth)
{
    map_buildcave();

    mob_resetAll();

    POS		mpos;

    // Make downstairs
    map_find(TILE_FLOOR, &mpos);
    MAP(mpos) = TILE_DOWNSTAIRS;

    for (int i = 0; i < 2+depth; i++)
    {
		mob_spawn(depth);
    }

    // Create gold
    for (int ngold = 0; ngold < 5; ngold++)
    {
		map_find(TILE_FLOOR, &mpos);
		MAP(mpos) = TILE_GOLD;
	}
}

PosDiff
map_computeDiff(POS a, POS b)
{
    // Bah, these have to be s8 or I blow my stack :<
    s8		ax, ay, bx, by;
    s8		dx, dy, sdx, sdy, dist;
    s8		adx, ady;
    PosDiff	result;

    POS2XY(b, bx, by);
    POS2XY(a, ax, ay);

    dx = bx - ax;
    dx &= MAPWIDTHMASK;
    sdx = dx ? 1 : 0;
    adx = dx;
    if (dx > MAPWIDTH/2)
    {
		dx = dx - MAPWIDTH;
		sdx = -1;
		adx = -dx;
    }
    dy = by - ay;
    dy &= MAPHEIGHTMASK;
    sdy = dy ? 1 : 0;
    ady = dy;
    if (dy > MAPHEIGHT/2)
	{
		dy = dy - MAPHEIGHT;
		sdy = -1;
		ady = -dy;
    }

    if (ady > adx)
		dist = ady;
    else
		dist = adx;

    result.sdx = sdx;
    result.sdy = sdy;
    result.dist = dist;
    result.padding = 0;

    return result;
}

bool
map_haslos(POS a, POS b)
{
    if (a == b)
		return true;

    // Center relative to a.
    while (1)
	{
		PosDiff		diff;

		diff = map_computeDiff(a, b);

		if (diff.dist <= 1)
			return true;

		if (diff.dist > 5)
			return false;

		a = deltaXY(a, diff.sdx, diff.sdy);
		if (!map_istransparent(a))
			return false;
    }

    return false;
}

void
map_buildfov(POS mpos)
{
    mapflag_clear(MAPFLAG_FOV);

    POS		p;
    FORALL_POS(p)
    {
		if (map_haslos(mpos, p) || map_haslos(p, mpos))
		{
			SETFLAG(p, MAPFLAG_FOV | MAPFLAG_MAPPED);
		}
	}
}

void 
map_draw(POS mpos)
{
    int		mx, my;
    
    POS2XY(mpos, mx, my);

    for (int y = 0; y < 11; y++)
    {
		for (int x = 0; x < 15; x++)
		{
			POS		mappos;
			u8		tile;
			u8			sym = ' ';
			u8			attr = ATTR_WHITE;
	
			mappos = POSXY(mx+x-7, my+y-5);
			tile = MAP(mappos);
	
			if (MAPFLAG(mappos, MAPFLAG_MAPPED))
			{
				u8		infov = MAPFLAG(mappos, MAPFLAG_FOV);
		
				if (TILE_ISMOB(tile))
				{
					int		defn;
					defn = mob_info[TILE_MOBSLOT(tile)].defn;
					attr = glb_mobdefs[defn].attr;
					if (infov)
						sym = glb_mobdefs[defn].symbol;
					else
						sym = glb_tiledefs[mob_info[TILE_MOBSLOT(tile)].floor].symbol;
				}
				else
				{
					attr = glb_tiledefs[tile].attr;
					sym = glb_tiledefs[tile].symbol;
				}
				if (!infov)
				{
					attr = ATTR_OUTOFFOV;
				}
			}
			gfx_putcharCD(x, y+1, sym, attr);
		}
    }
}
