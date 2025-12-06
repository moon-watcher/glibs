#include "frameloader.h"

void frameloader_init(frameloader *$, void (*update_f)(), uint16_t vrampos)
{
    $->update_f = update_f;
    $->vrampos = vrampos;
}

void frameloader_set(frameloader *$, void *resource, uint16_t frames, uint16_t timer)
{
    $->resource = resource;
    $->countdown = $->timer = timer;
    $->frame = 0;
    $->num_frames = frames;

    $->update_f($);
}

void frameloader_update(frameloader *$)
{
    if ($->countdown == 0)
    {
        ++$->frame;

        if ($->frame >= $->num_frames)
            $->frame = 0;

        $->countdown = $->timer;
        $->update_f($);
    }

    $->countdown--;
}

inline uint16_t frameloader_isLastFrame(frameloader *$)
{
    return ($->countdown == 1);
}

inline uint16_t frameloader_isLastTick(frameloader *$)
{
    return ($->frame == $->num_frames - 1);
}
