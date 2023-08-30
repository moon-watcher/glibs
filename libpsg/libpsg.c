#include "core/SN76489.h"

static unsigned char isPAL = 0;
static unsigned long vtimer = 0;
static unsigned char isPlaying = 0;
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