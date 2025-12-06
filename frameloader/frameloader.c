#include "frameloader.h"

void frameloader_init(frameloader *$, void (*update)(), uint16_t vrampos)
{
    $->update = update;
    $->vrampos = vrampos;
}

void frameloader_set(frameloader *$, void *resource, uint16_t frames, uint16_t timer)
{
    $->resource = resource;
    $->countdown = $->timer = timer;
    $->frame = 0;
    $->num_frames = frames;
}

void frameloader_update(frameloader *$)
{
    if ($->countdown == 0)
    {
        ++$->frame;

        if ($->frame >= $->num_frames)
            $->frame = 0;

        $->countdown = $->timer;
        frameloader_exec($);
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

inline void frameloader_exec(frameloader *$)
{
    $->update($);
}
