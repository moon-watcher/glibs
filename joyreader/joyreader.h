#pragma once

void joyreader_update(unsigned *, unsigned);
void joyreader_press(unsigned *, unsigned);

#define JOYREADER_UPDATE(J, V) ({auto __V = V; J = ((__V ^ ((J >> 16) & 0xFFFF)) << 16) | __V; J; })
#define JOYREADER_PRESS(J, V)  
#define JOYREADER_ACTIVE(J)    (J & 0xFFFF)
#define JOYREADER_CHANGED(J)   ((J >> 16) & 0xFFFF)
#define JOYREADER_INACTIVE(J)  (~JOYREADER_ACTIVE(J))
#define JOYREADER_PRESSED(J)   (JOYREADER_ACTIVE(J) & JOYREADER_CHANGED(J))
#define JOYREADER_RELEASED(J)  (JOYREADER_INACTIVE(J) & JOYREADER_CHANGED(J))

#include "config.h"
