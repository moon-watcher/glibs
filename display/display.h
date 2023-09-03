#pragma once

#include "config/helper.h"
#include GLIBS_DISPLAY_HELPER_FILE

void display_init();
void display_on(unsigned char);
void display_off(unsigned char);
void display_prepare(unsigned short *, unsigned char);