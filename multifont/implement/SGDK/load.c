#include <genesis.h>

void SGDK_load(unsigned int vrampos, const unsigned long *tiles, unsigned int pos, char chr, unsigned int size)
{
    DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(vrampos << 5)), tiles + ((pos + chr) << 3), size << 4, 2);
}
