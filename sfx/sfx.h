#pragma once

typedef struct _sfx {
	char *title;
	unsigned char *data;
	unsigned long length;
}
SFX;

void sfxInit ( );
void sfxPlay ( int );
void sfxStop ( );