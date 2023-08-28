#pragma once

#include <genesis.h>

void drawText(const char *, u16, u16);
void drawInt(u32, u8, u8, u8);
void drawUInt(u32, u8, u8, u8);
void drawFix32(fix32, u8, u8, u8);
void drawFix16(fix16, u8, u8, u8);
void drawImage(int, const Image *, int);
void drawImageA(const Image *);
void drawImageB(const Image *);

// void drawFadeInA(const Image *, u16);
// void drawFadeInA_XY(const Image *, u16, int, int);
// void drawMem(u8, u8);