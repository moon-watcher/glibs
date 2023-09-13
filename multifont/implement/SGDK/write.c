#include <genesis.h>

void SGDK_write(int plan, int pal, int prio, unsigned int vrampos, unsigned int x, unsigned int y, unsigned char width, unsigned char height)
{
    VDP_fillTileMapRectInc(plan, TILE_ATTR_FULL(pal, prio, 0, 0, vrampos), x, y, width, height);
}