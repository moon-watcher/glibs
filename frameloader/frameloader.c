#include "frameloader.h"

void frameloader_init(frameloader *const this, void (*update_f)(frameloader *const), unsigned vrampos)
{
    this->update_f = update_f;
    this->vrampos = vrampos;
    this->resource = 0;
    this->countdown = 0;
    this->timer = 0;
    this->anim = 0;
    this->frame = 0;
    this->pause = 0;
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
    if (!this->resource)
        return FRAMELOADER_ERROR;

    if (this->pause)
        return FRAMELOADER_PAUSED;

    if (this->countdown > 0 && --this->countdown)
        return FRAMELOADER_IDLE;

    this->countdown = this->timer;
    this->update_f(this);

    return FRAMELOADER_OK;
}

void frameloader_pause(frameloader *const this)
{
    this->pause = 1;
}

void frameloader_resume(frameloader *const this)
{
    this->pause = 0;
}
