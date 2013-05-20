//
// map files
//

#pragma once

#include "mytypes.h"
#include "glbdef.h"

typedef u32	POS;
typedef	u32	DIR;

// Fixed map dimensions, must be power of 2.
#define MAPWIDTHBITS 4
#define MAPHEIGHTBITS 4
#define MAPWIDTH (1 << MAPWIDTHBITS)
#define MAPHEIGHT (1 << MAPHEIGHTBITS)
#define MAPWIDTHMASK (MAPWIDTH-1)
#define MAPHEIGHTMASK (MAPHEIGHT-1)
#define MAPSIZE (1 << (MAPWIDTHBITS+MAPHEIGHTBITS))
#define MAPMASK (MAPSIZE-1)

extern u8 map_tiledata[MAPSIZE];
extern u8 map_flagdata[MAPSIZE];

#define TILE_AVATAR		128

#define TILE_ISMOB(tile) ((tile) & 128)
#define TILE_ISAVATAR(tile) ((tile) == TILE_AVATAR)
#define TILE_MOBSLOT(tile) ((tile) & 127)

#define POSXY(x, y) ( ((x) + ((y) * MAPWIDTH)) & MAPMASK )

#define POS2XY(pos, x, y) \
    x = (pos) & (MAPWIDTHMASK); \
    y = (pos) >> (MAPWIDTHBITS);

// Note absence of masking since we allow negative directions!
#define DIRXY(dx, dy) ((dx) + (dy) * MAPWIDTH)

#define MAP(pos) map_tiledata[pos]
#define MAPXY(x, y) MAP( POSXY(x, y) )

#define MAPFLAG(pos, flag) (map_flagdata[pos] & (flag))
#define CLEARFLAG(pos, flag) (map_flagdata[pos] &= ~(flag))
#define SETFLAG(pos, flag) (map_flagdata[pos] |= (flag))

#define FORALL_POS(pos) \
    for ((pos) = 0; (pos) < MAPSIZE; (pos)++)

// Does not include hitting mobs!
inline bool map_canmove(POS p)
{
    if (glb_tiledefs[MAP(p)].ispassable)
		return true;
    return false;
}

inline bool map_canmoveormob(POS p)
{
    u8		tile = MAP(p);
    if (TILE_ISMOB(tile))
		return true;
    if (glb_tiledefs[tile].ispassable)
		return true;
    return false;
}

bool map_istransparent(POS p);

POS deltaXY(POS p, int dx, int dy);

POS delta(POS p, DIR d);

void map_clear(u8 tile);

void mapflag_clear(u8 flag);

bool map_find(u8 tile, POS *pos);

bool map_findfirst(u8 tile, POS *pos);

void map_buildcave();

void map_build(int depth);

typedef struct 
{
    u8		dist;
    s8		sdx, sdy;
    u8		padding;
} __attribute__((packed)) PosDiff;

PosDiff map_computeDiff(POS a, POS b);

bool map_haslos(POS a, POS b);

void map_buildfov(POS mpos);

void  map_draw(POS mpos);
