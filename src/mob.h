//
// This contains all the functions to deal with monsters, or mobs.
//

#pragma once

extern u8	glb_gold;
extern u8	glb_depth;
extern u8	glb_roguedir;

#include "map.h"

// Forward declarations
#define		MOB_MAXIMUM	32

struct MOB_INFO
{
    u8		hp;		// Dynamic data.  Zero means dead
    u8		defn;		// Our MOB_NAME
    u8		floor;		// What we are standing on
};

extern struct MOB_INFO mob_info[MOB_MAXIMUM];

bool action_walk(POS mpos, DIR d);

const char *mob_getname(u8 tile);

bool action_melee(POS mpos, DIR d);
bool action_bump(POS mpos, DIR d);

void ai_randomwalk(POS mpos);
void ai_charge(POS mpos, POS apos);

void mob_resetAll();
bool mob_spawn(int depth);
