#include "core/SN76489.h"
#include "libpsg.h"

void libpsg_init(struct libpsg *const psg, unsigned char *data, unsigned char isPAL)
{
	psg->isPAL = isPAL;
	psg->data = data;
	psg->isPlaying = 0;

	SN76489_init();
}

void libpsg_play(struct libpsg *const psg, unsigned char track)
{
	psg->vtimer = 0;
	psg->isPlaying = 1;

	SN76489_play(psg->data, track);
}

void libpsg_stop(struct libpsg *const psg)
{
	psg->data = 0;
	psg->isPlaying = 0;
}

void libpsg_update(struct libpsg *const psg)
{
	if (psg->isPlaying && (psg->isPAL || (psg->vtimer % 6)))
		SN76489_update(psg->data);

	++psg->vtimer;
}

void libpsg_pause(struct libpsg *const psg)
{
	psg->isPlaying = 0;
}

void libpsg_resume(struct libpsg *const psg)
{
	psg->isPlaying = 1;
}
