#include "core/SN76489.h"
#include "libpsg.h"

static unsigned int isPAL = 0;
static unsigned long vtimer = 0;
static unsigned int isPlaying = 0;
static unsigned char *data = 0;

void libpsg_init(unsigned int isPALSystem)
{
	isPAL = isPALSystem;
}

void libpsg_play(const unsigned char *const stream, unsigned char track)
{
	vtimer = 0;
	isPlaying = 1;
	data = stream;
	SN76489_play(data, track);
}

void libpsg_stop()
{
	isPlaying = 0;
	data = 0;
}

void libpsg_update()
{
	if (((vtimer++ % 6) || isPAL) && isPlaying)
		SN76489_update(data);
}

void libpsg_pause()
{
	isPlaying = 0;
}

void libpsg_resume()
{
	isPlaying = 1;
}