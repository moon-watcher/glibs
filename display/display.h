#pragma once

#include "config.h"

#include DISPLAY_HELPER_FILE
#include DISPLAY_CONFIG_FILE

void display_on(unsigned char);
void display_off(unsigned char);
void display_prepare(unsigned short *, unsigned char);