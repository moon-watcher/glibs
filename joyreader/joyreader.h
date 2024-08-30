#pragma once

#define JOYREADER_UPDATE(J, V) ({ J = *(unsigned int*)&(struct { unsigned short changed, active; }){V ^ J, V}; })
#define JOYREADER_ACTIVE(J)    ((unsigned short)J)
#define JOYREADER_CHANGED(J)   (J >> 16)
#define JOYREADER_INACTIVE(J)  (~JOYREADER_ACTIVE(J))
#define JOYREADER_PRESSED(J)   (JOYREADER_CHANGED(J) & JOYREADER_ACTIVE(J))
#define JOYREADER_RELEASED(J)  (JOYREADER_CHANGED(J) & JOYREADER_INACTIVE(J))

#include "config.h"