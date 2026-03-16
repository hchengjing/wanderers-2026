//enumeration: user-defined data type that defines a set of named constants
#ifndef ENITIO_enums_h
#define ENITIO_enums_h

typedef enum {
    INVICTA = 0,
    DYNARI = 1,
    EPHILIA = 2,
    AKRONA = 3,
    SOLARIS = 4,
} CLAN_id;  //diff clan

typedef enum {
    do_nothing = 0,
    attack = 1,
    collect = 2,
    heal = 3,
    revive = 4,
    poison = 5,
    deposit = 6,
    transfer = 7,
    depo_kda = 8,
    bomb = 9,
} action_id;  //diff actions

typedef enum {
    waiting = 0,
    use = 1,
    active = 2,
} Bomb_State;  //state of bomb

typedef enum {
    idle = 0,
    button = 1,
    right = 2,
    up = 3,
    left = 4,
    down = 5,
} joystick_pos;  //diff joystick directions/actions

typedef enum {
    nothing = 0,
    blue = 1, //2x points from shards
    yellow = 2, //invincible for 15sec
    red = 3, //poison attack
    purple = 4, //2x damage
    green = 5, //charge to max EN
    orange = 6, //auto revive
    transferstone = 7, //transfer
    mergestone = 8,
} power_up;  //diff infinity stone abilities

typedef enum {
    freshman = 0,
    groupLeader = 1,
    sigma = 2,
} role;  //diff player roles, sigma for tech comm

#endif
