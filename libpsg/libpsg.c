#include "core/SN76489.h"

static unsigned char isPAL = 0;
static unsigned long vtimer = 0;
static unsigned char paused = 0;
static unsigned char *data = 0;

void libpsg_init(unsigned char isPALSystem)
{
	isPAL = isPALSystem;
}

void libpsg_play(const unsigned char *const stream, unsigned char track)
{
	vtimer = 0;
	data = stream;
	libpsg_resume();
	SN76489_play(data, track);
}

void libpsg_stop()
{
	data = 0;
	libpsg_pause();
}

void libpsg_update()
{
	if (((vtimer++ % 6) || isPAL) && paused == 0)
		SN76489_update(data);
}

void libpsg_pause()
{
	paused = 1;
}

void libpsg_resume()
{
	paused = 0;
}