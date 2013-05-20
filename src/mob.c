//
// This contains all the functions to deal with monsters, or mobs.
//

#include "mytypes.h"
#include "glbdef.h"
#include "map.h"
#include "mob.h"
#include "msg.h"
#include "rand.h"

u8	glb_gold = 0;
u8	glb_depth = 1;
u8	glb_roguedir = 0;

struct MOB_INFO mob_info[MOB_MAXIMUM];

bool
action_walk(POS mpos, DIR d)
{
    POS		goalpos = delta(mpos, d);
    u8		goal = MAP(goalpos);

    if (TILE_ISAVATAR(MAP(mpos)))
    {
	if (goal == TILE_GOLD)
	{
	    msg_report("Gold!");
	    msg_endline();
	    MAP(goalpos) = TILE_FLOOR;
	    glb_gold++;
	}
	else if (goal == TILE_DOWNSTAIRS)
	{
	    msg_report("You descend.");
	    // Generate a new map!
	    glb_depth++;
	    map_build(glb_depth);
	    return false;
	}
	else if (!glb_tiledefs[goal].ispassable)
	{
	    msg_report("Ow!");
	    u32 roguedirlut = 0x00010302;
	    glb_roguedir = (roguedirlut >> (glb_roguedir*8)) & 3;
	    return false;
	}
    }

    u8		osym = MAP(mpos);
    u8		sym = TILE_MOBSLOT(osym);

    MAP(mpos) = mob_info[sym].floor;
    mob_info[sym].floor = MAP(goalpos);
    MAP(goalpos) = osym;

    return true;
}

const char *
mob_getname(u8 tile)
{
    return glb_mobdefs[mob_info[TILE_MOBSLOT(tile)].defn].name;
}

bool
action_melee(POS mpos, DIR d)
{
    POS		goalpos = delta(mpos, d);
    u8		target = MAP(goalpos);
    u8		me = MAP(mpos);
    bool	hit = false;
    const char	*verb;

    if (!TILE_ISMOB(target))
	return false;
    target = TILE_MOBSLOT(target);
    me = TILE_MOBSLOT(me);

    // Determine if we hit
    if (rand_choice(3))
	hit = true;

    if (hit)
    {
	bool		kills = false;
	int		nhp = mob_info[target].hp;

	if (mob_info[me].defn == MOB_ELF &&
	    mob_info[target].defn == MOB_AVATAR &&
	    glb_gold)
	{
	    verb = " robs ";
	    glb_gold--;
	}
	else
	{
	    nhp--;
	    if (nhp <= 0)
	    {
		kills = true;
	    }

	    if (kills)
	    {
		verb = " kill ";
		MAP(goalpos) = mob_info[target].floor;
		mob_info[target].hp = 0;
	    }
	    else
	    {
		verb = " hit ";
		mob_info[target].hp = nhp;
	    }
	}
    }
    else
    {
	// Just some spam.
	verb = " miss ";
    }
    msg_report(mob_getname(me));
    msg_report(verb);
    msg_report(mob_getname(target));
    msg_endline();
    return true;
}

bool
action_bump(POS mpos, DIR d)
{
    POS		goalpos = delta(mpos, d);
    u8		goal = MAP(goalpos);

    if (!d)
	return true;

    if (TILE_ISMOB(goal))
    {
	return action_melee(mpos, d);
    }
    return action_walk(mpos, d);
}

void
ai_randomwalk(POS mpos)
{
    // Determine adjacent legal square.
    int		dx, dy;
    rand_dir(&dx, &dy);
    DIR		d = DIRXY(dx, dy);
    POS		goalpos = delta(mpos, d);
    if (map_canmove(goalpos))
    {
	// Walk that way!
	action_bump(mpos, d);
    }
}

void
ai_charge(POS mpos, POS apos)
{
    if (!MAPFLAG(mpos, MAPFLAG_FOV))
    {
	return ai_randomwalk(mpos);
    }

    PosDiff	diff;

    diff = map_computeDiff(mpos, apos);

    int		slot = TILE_MOBSLOT(MAP(mpos));
    u8		defn = mob_info[slot].defn;

    if (diff.dist > glb_mobdefs[defn].sight)
    {
	return ai_randomwalk(mpos);
    }

    int		sdx, sdy;
    sdx = diff.sdx;
    sdy = diff.sdy;

    if (sdx && sdy)
    {
	if (rand_choice(2))
	    sdy = 0;
	else
	    sdx = 0;
    }

    DIR		d = DIRXY(sdx, sdy);
    POS		goalpos = delta(mpos, d);
    if (map_canmoveormob(goalpos))
    {
	// Walk that way!
	action_bump(mpos, d);
    }
    else
    {
	return ai_randomwalk(mpos);
    }
}

void
mob_resetAll()
{
    mob_info[0].floor = MAP(0);
    mob_info[0].defn = MOB_AVATAR;
    MAP(0) = TILE_AVATAR;
    for (int mobslot = 1; mobslot < MOB_MAXIMUM; mobslot++)
	mob_info[mobslot].hp = 0;
}

bool 
mob_spawn(int depth)
{
    int		slot;

    // Find a free slot
    for (slot = 1; slot < MOB_MAXIMUM; slot++)
	if (mob_info[slot].hp == 0)
	    break;
    if (slot >= MOB_MAXIMUM)
	return false;

    // determine a type...
    int		mtype;
    while (1)
    {
	mtype = rand_choice(NUM_MOBS);
	if (glb_mobdefs[mtype].depth <= depth)
	    break;
    }

    // Find a square
    POS			mpos;
    if (!map_find(TILE_FLOOR, &mpos))
	return false;

    // Create the mob!
    mob_info[slot].floor = MAP(mpos);
    mob_info[slot].hp = glb_mobdefs[mtype].max_hp;
    mob_info[slot].defn = mtype;
    MAP(mpos) = slot + TILE_AVATAR;

    return true;
}
