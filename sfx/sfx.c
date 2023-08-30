#include <genesis.h>

static unsigned int channel = 0;

void sfxInit()
{
    channel = SOUND_PCM_CH1;
}

void sfxPlay(unsigned int index)
{
    if (channel > SOUND_PCM_CH4)
        sfxInit();

    XGM_startPlayPCM(64 + index, 1, channel);

    ++channel;
}

void sfxStop()
{
    sfxInit();
    XGM_stopPlayPCM(SOUND_PCM_CH1);
    XGM_stopPlayPCM(SOUND_PCM_CH2);
    XGM_stopPlayPCM(SOUND_PCM_CH3);
    XGM_stopPlayPCM(SOUND_PCM_CH4);
}