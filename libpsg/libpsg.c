/* PSG multichannel sound effects player v1.0 by Shiru, 03.11.07 */

/* gcc version 12.12.07 */

/* Modified by @MoonWatcherMD at 20151019. Thanks, Shiru! */
/* Modified by @MoonWatcherMD at 20170313. Added pause/resume */

#include <genesis.h>
#include "core/SN76489.h"
#include "libpsg.h"

void libpsg_play(struct libpsg *const psg, const unsigned char *const data, unsigned char track)
{
	psg->sn.data = data;
	psg->track = track;
	psg->pause = 0;
	psg->isPAL = SYS_isPAL();

	SN76489_play(&psg->sn, psg->track);
}

void libpsg_stop(struct libpsg *const psg)
{
	libpsg_pause(psg);
	psg->sn.data = 0;
}

void libpsg_update(struct libpsg *const psg)
{
	if ((psg->isPAL || (vtimer % 6)) && !psg->pause)
		SN76489_update(&psg->sn);
}

void libpsg_pause(struct libpsg *const psg)
{
	psg->pause = 1;
}

void libpsg_resume(struct libpsg *const psg)
{
	psg->pause = 0;
}