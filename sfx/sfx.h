#pragma once

struct sfx
{
	char *title;
	unsigned char *data;
	unsigned long length;
};

void sfxInit();
void sfxPlay(unsigned int);
void sfxStop();