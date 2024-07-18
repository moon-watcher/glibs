#include "joyreader.h"

void joyreader_init(joyreader *const joy)
{
    joy->changed = joy->active = 0;
}

void joyreader_update(joyreader *const joy, unsigned active)
{
    joy->changed = active ^ joy->active;
    joy->active = active;
}

void joyreader_press(joyreader *const joy, unsigned value)
{
    joy->active = value;
}