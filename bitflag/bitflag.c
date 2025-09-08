#include "bitflag.h"

inline void bitflag_init(unsigned *obs)
{
    *obs = 0;
}

inline void bitflag_add(unsigned *obs, unsigned flag)
{
    *obs |= flag;
}

inline void bitflag_clear(unsigned *obs, unsigned flag)
{
    *obs &= ~flag;
}

inline unsigned bitflag_has(unsigned *obs, unsigned flag)
{
    return (*obs & flag) != 0;
}

inline void bitflag_toggle(unsigned *obs, unsigned flag)
{
    *obs ^= flag;
}

/*
typedef enum {
    BITFLAG_NONE = 0,

    BITFLAG_PLAYER_DIE,
    BITFLAG_PLAYER_HIT,
    BITFLAG_PLAYER_12LAMPS,
    BITFLAG_PLAYER_18LAMPS,
    BITFLAG_PLAYER_26LAMPS,

    BITFLAG_SYSTEM_PAUSE,

    BITFLAG_SCENE_INTRO,
    BITFLAG_SCENE_COMPLETED_01

} OOOOEnum;

// "namespace" simulado
struct OOOO {
    enum {
        NONE            = BITFLAG_NONE,

        PLAYER_DIE      = BITFLAG_PLAYER_DIE,
        PLAYER_HIT      = BITFLAG_PLAYER_HIT,
        PLAYER_12LAMPS  = BITFLAG_PLAYER_12LAMPS,
        PLAYER_18LAMPS  = BITFLAG_PLAYER_18LAMPS,
        PLAYER_26LAMPS  = BITFLAG_PLAYER_26LAMPS,

        SYSTEM_PAUSE    = BITFLAG_SYSTEM_PAUSE,

        SCENE_INTRO     = BITFLAG_SCENE_INTRO,
        SCENE_COMPLETED_01 = BITFLAG_SCENE_COMPLETED_01
    };
};

// Alias para usarlo como tipo
typedef OOOOEnum OOOOFlag;

int main(void)
{
    OOOOFlag e = OOOO.PLAYER_DIE;   // ✅ uso estilo namespace
    printf("OOOO event = %d\n", e);

    if (e == OOOO.PLAYER_DIE) {
        printf("Player died!\n");
    }
    return 0;
}


OOOOFlag flag = OOOO.PLAYER_18LAMPS;

if (flag == OOOO.PLAYER_18LAMPS)
    printf("Show bonus!\n");
*/