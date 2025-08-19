#include "frameloader.h"

void frameloader_init(frameloader *const this, int (*update_f)(frameloader *const), unsigned vrampos)
{
    this->update_f = update_f;
    this->vrampos = vrampos;
    this->resource = 0;
    this->countdown = 0;
    this->timer = 0;
    this->anim = 0;
    this->frame = 0;
}

void frameloader_set(frameloader *const this, const void *resource, int timer, int anim)
{
    frameloader_init(this, this->update_f, this->vrampos);

    this->resource = resource;
    this->timer = timer;
    this->anim = anim;

    frameloader_update(this);
}

unsigned frameloader_update(frameloader *const this)
{
    if (this->countdown == 0)
    {
        this->countdown = this->timer;
        unsigned total_frames = this->update_f(this);

        if (++this->frame == total_frames)
            this->frame = 0;
    }

    return this->countdown--;
}
