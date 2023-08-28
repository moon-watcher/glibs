#include "joyreader.h"

void joyreader_init(joyreader *const joy, unsigned int port, unsigned int (*reader_f)(unsigned int))
{
    joy->port = port;
    joy->reader_f = reader_f;
    joy->changed = 0;
    joy->active = 0;
}

void joyreader_update(joyreader *const joy)
{
    unsigned int const active = joy->reader_f(joy->port);

    joy->changed = active ^ joy->active;
    joy->active = active;
}

void joyreader_reset(joyreader *const joy)
{
    joy->changed = 0;
    joy->active = 0;
}

void joyreader_press(joyreader *const joy, unsigned int value)
{
    joy->active = value;
}