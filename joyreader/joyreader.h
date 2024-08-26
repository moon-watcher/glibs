#pragma once

#define JOYREADER_INIT(J)      (J = 0)
#define JOYREADER_UPDATE(J, V) ({ J = *(unsigned *)&(struct { unsigned changed : 16, active : 16; }){V ^ J, V}; })
#define JOYREADER_PRESS(J, V)
#define JOYREADER_ACTIVE(J)    ((J << 16) >> 16)
#define JOYREADER_CHANGED(J)   (J >> 16)
#define JOYREADER_INACTIVE(J)  (~JOYREADER_ACTIVE(J))
#define JOYREADER_PRESSED(J)   (JOYREADER_CHANGED(J) & JOYREADER_ACTIVE(J))
#define JOYREADER_RELEASED(J)  (JOYREADER_CHANGED(J) & JOYREADER_INACTIVE(J))

#include "config.h"