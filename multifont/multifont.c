#include "multifont.h"

void multifont_init(multifont *const mf, const Image *image, unsigned int plan, unsigned int pal, unsigned int (*vrampos_f)(unsigned int))
{
    mf->image = image;
    mf->pal = pal;
    mf->plan = plan;
    mf->prio = 1;
    mf->vrampos_f = vrampos_f;
}