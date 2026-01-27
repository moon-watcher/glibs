#pragma once

#include <stdint.h>

void display_on(uint16_t);
void display_off(uint16_t);
uint16_t display_prepare(uint16_t *, uint16_t);
uint16_t display_prepareColor(uint16_t, uint16_t);
