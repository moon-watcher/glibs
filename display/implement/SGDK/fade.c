#include <genesis.h>

void FADE_FUNCTION(unsigned int frames, unsigned int *colors)
{
    SYS_doVBlankProcess();
    SYS_disableInts();

    if (frames)
        PAL_fadeTo(0, 63, colors, frames, 0);
    else
        PAL_setColors(0, colors, 64, DMA);

    SYS_enableInts();
}