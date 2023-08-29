#include "core/SN76489.h"
#include "libpsg.h"

void libpsg_init(struct libpsg *const psg, const unsigned char *const data, unsigned char isPAL)
{
	psg->isPAL = isPAL;
	psg->sn.data = data;
	psg->isPlaying = 0;
}

void libpsg_play(struct libpsg *const psg, unsigned char track)
{
	psg->vtimer = 0;
	psg->isPlaying = 1;
	SN76489_play(&psg->sn, track);
}

void libpsg_stop(struct libpsg *const psg)
{
	psg->sn.data = 0;
	psg->isPlaying = 0;
}

void libpsg_update(struct libpsg *const psg)
{
	if (!psg->sn.data)
		return;

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
