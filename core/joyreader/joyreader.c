#include "joyreader.h"

void joyreader_init(joyreader *const joy, unsigned port, unsigned (*reader_f)(unsigned))
{
    joy->port = port;
    joy->reader_f = reader_f;
    joy->changed = 0;
    joy->active = 0;
}

void joyreader_update(joyreader *const joy)
{
    unsigned const active = joy->reader_f(joy->port);

    joy->changed = active ^ joy->active;
    joy->active = active;
}

void joyreader_reset(joyreader *const joy)
{
    joy->changed = 0;
    joy->active = 0;
}

void joyreader_press(joyreader *const joy, unsigned value)
{
    joy->active = value;
}