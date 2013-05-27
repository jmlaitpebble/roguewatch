// Automagically generated by enummaker.exe.
// DO NOT EDIT THIS FILE (Yes, I mean you!)
#ifndef __glbdef_h__
#define __glbdef_h__


// Definitions for MOB
enum MOB_NAMES
{
    MOB_AVATAR,
    MOB_IMP,
    MOB_ANT,
    MOB_ORC,
    MOB_RAT,
    MOB_BEE,
    MOB_ELF,
    MOB_WIZ,
    NUM_MOBS
};

// Macros for MOB
#define FOREACH_MOB(x) \
    for ((x) = (MOB_NAMES) 0; \
         (x) < NUM_MOBS; \
         (x) = (MOB_NAMES) ((int)(x)+1))

struct MOB_DEF
{
    u8 symbol;
    u8 attr;
    u8 melee_damage;
    u8 max_hp;
    u8 depth;
    u8 sight;
    char name[4];
} PACKED;

extern const struct MOB_DEF glb_mobdefs[];

// Definitions for TILE
enum TILE_NAMES
{
    TILE_WALL,
    TILE_FLOOR,
    TILE_DOWNSTAIRS,
    TILE_GOLD,
	TILE_POTION,
    NUM_TILES
};

// Macros for TILE
#define FOREACH_TILE(x) \
    for ((x) = (TILE_NAMES) 0; \
         (x) < NUM_TILES; \
         (x) = (TILE_NAMES) ((int)(x)+1))

struct TILE_DEF
{
    u8 symbol;
    u8 attr;
    bool ispassable : 1;
    bool istransparent : 1;
} PACKED;

extern const struct TILE_DEF glb_tiledefs[];

// Definitions for MAPFLAG
enum MAPFLAG_NAMES
{
    MAPFLAG_NONE = 0,
    MAPFLAG_FOV = 128,
    MAPFLAG_MAPPED = 64,
    MAPFLAG_MOB = 32,
    NUM_MAPFLAGS
};

// Macros for MAPFLAG
#define FOREACH_MAPFLAG(x) \
    for ((x) = (MAPFLAG_NAMES) 0; \
         (x) < NUM_MAPFLAGS; \
         (x) = (MAPFLAG_NAMES) ((int)(x)+1))

// Definitions for ATTR
enum ATTR_NAMES
{
    ATTR_GOLD,
    ATTR_YELLOW,
    ATTR_NORMAL,
    ATTR_LIGHTBLACK,
    ATTR_OUTOFFOV,
    ATTR_WHITE,
    ATTR_BROWN,
    ATTR_RED,
    ATTR_DKRED,
    ATTR_GREEN,
    ATTR_PURPLE,
    NUM_ATTRS
};

// Macros for ATTR
#define FOREACH_ATTR(x) \
    for ((x) = (ATTR_NAMES) 0; \
         (x) < NUM_ATTRS; \
         (x) = (ATTR_NAMES) ((int)(x)+1))

struct ATTR_DEF
{
    u8 fg_r;
    u8 fg_g;
    u8 fg_b;
} PACKED;

extern const struct ATTR_DEF glb_attrdefs[];

// Definitions for ACTION
enum ACTION_NAMES
{
    ACTION_NONE,
    ACTION_RESTART,
    ACTION_BUMP,
    ACTION_WAIT,
    NUM_ACTIONS
};

// Macros for ACTION
#define FOREACH_ACTION(x) \
    for ((x) = (ACTION_NAMES) 0; \
         (x) < NUM_ACTIONS; \
         (x) = (ACTION_NAMES) ((int)(x)+1))
#endif
