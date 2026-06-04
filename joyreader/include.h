#pragma once

#define JOYREADER_UPDATE(J, V) ({unsigned short _v = (V); J = ((unsigned int)((unsigned short)(J) ^ _v) << 16) | _v;})
#define JOYREADER_ACTIVE(J)    ((unsigned short)(J))
#define JOYREADER_CHANGED(J)   ((unsigned short)((J) >> 16))
#define JOYREADER_INACTIVE(J)  (~JOYREADER_ACTIVE(J))
#define JOYREADER_PRESSED(J)   (JOYREADER_CHANGED(J) & JOYREADER_ACTIVE(J))
#define JOYREADER_RELEASED(J)  (JOYREADER_CHANGED(J) & JOYREADER_INACTIVE(J))
