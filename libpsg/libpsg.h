#pragma once

void libpsg_init(unsigned char);
void libpsg_play(const unsigned char *const, unsigned char);
void libpsg_stop();
void libpsg_update();
void libpsg_pause();
void libpsg_resume();