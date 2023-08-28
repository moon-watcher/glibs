#include "core/SN76489.h"
#include "libpsg.h"

void libpsg_init(struct libpsg *const psg, const unsigned char *const data, unsigned char isPAL)
{
	psg->sn.data = data;
	psg->isPAL = isPAL;
	libpsg_pause(psg);
}

void libpsg_play(struct libpsg *const psg, unsigned char track)
{
	psg->vtimer = 0;
	libpsg_resume(psg);
	SN76489_play(&psg->sn, track);
}

void libpsg_stop(struct libpsg *const psg)
{
	libpsg_pause(psg);
	psg->sn.data = 0;
}

void libpsg_update(struct libpsg *const psg)
{
	if (psg->isPlaying && (psg->isPAL || (psg->vtimer % 6)))
		SN76489_update(&psg->sn);

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
