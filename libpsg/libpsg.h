#pragma once

#include "core/SN76489.h"

void libpsg_init(unsigned int);
void libpsg_play(const unsigned char *const data, unsigned char track);
void libpsg_stop();
void libpsg_update();
void libpsg_pause();
void libpsg_resume();