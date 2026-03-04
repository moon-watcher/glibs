#include "frameloader.h"

void frameloader_init(frameloader *$, void (*update)(), uint16_t vrampos)
{
    $->update = update;
    $->vrampos = vrampos;
    $->loop = -1;

    //
    $->external = 0;
}

void frameloader_set(frameloader *$, void *resource, uint16_t frames, uint16_t timer)
{
    $->resource = resource;
    $->countdown = $->timer = timer;
    $->frame = 0;
    $->num_frames = frames;
    $->loop = -1;
}

void frameloader_setResource(frameloader *$, void *resource, uint16_t frames)
{
    $->resource = resource;
    $->countdown = $->timer;
    $->frame = 0;
    $->num_frames = frames;
    $->loop = -1;
}

void frameloader_setTimer(frameloader *$, void *resource, uint16_t timer)
{
    $->countdown = $->timer = timer;
    $->frame = 0;
}

void frameloader_setLoop(frameloader *$, int16_t loop)
{
    $->loop = loop;

    if (loop >= 0)
        ++$->loop;
}

void frameloader_update(frameloader *$)
{
    if ($->loop == 0)
        return;

    if ($->countdown == 0)
    {
        ++$->frame;

        if ($->frame >= $->num_frames)
        {
            $->frame = 0;

            if ($->loop > 0 && --$->loop == 0)
                return;
        }

        $->countdown = $->timer;
        frameloader_exec($);
    }

    $->countdown--;
}

uint16_t frameloader_isLastFrame(frameloader *$)
{
    return ($->countdown == 1);
}

uint16_t frameloader_isLastTick(frameloader *$)
{
    return ($->frame == $->num_frames - 1);
}

int16_t frameloader_getLoop(frameloader *$)
{
    return $->loop;
}

void frameloader_exec(frameloader *$)
{
    $->update($);
}
