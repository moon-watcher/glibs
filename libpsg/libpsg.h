#pragma once

#include "core/SN76489.h"

struct libpsg
{
	struct SN76489 sn;
	unsigned long vtimer;
	unsigned int isPlaying : 1;
	unsigned int isPAL : 1;
};

void libpsg_init(struct libpsg *const, const unsigned char *const, unsigned char);
void libpsg_play(struct libpsg *const, unsigned char);
void libpsg_stop(struct libpsg *const);
void libpsg_update(struct libpsg *const);
void libpsg_pause(struct libpsg *const);
void libpsg_resume(struct libpsg *const);
