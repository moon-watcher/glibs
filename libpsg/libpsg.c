#include "core/SN76489.h"
#include "config.h"

static unsigned char *data;

void libpsg_play(const unsigned char *const stream, unsigned char track)
{
	data = stream;
	SN76489_play(data, track);
}

void libpsg_update()
{
	if (data && ((_vtimer % 6) || _isPAL))
		SN76489_update(data);
}

void libpsg_stop()
{
	data = 0;
}