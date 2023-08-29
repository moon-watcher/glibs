#pragma once

#define SN76489_DATA 0xc00011
#define SN76489_VCH_MAX 4
#define SN76489_CHN_MAX 4

struct SN76489
{
    const unsigned char *data;

    struct SN76489_slot
    {
        int ptr;
        int wait;
        int time;
        unsigned int div;
        unsigned char vol;
    } slot[SN76489_CHN_MAX][SN76489_VCH_MAX];
};

void SN76489_play(struct SN76489 *const, unsigned char);
void SN76489_update(struct SN76489 *const);