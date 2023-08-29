/**
 *  PSG multichannel sound effects player v1.0 by Shiru, 03.11.07
 *
 * gcc version 12.12.07
 *
 * @MoonWatcherMD at 20151019. Thanks, Shiru!
 * @MoonWatcherMD at 20230822. Refactor
 */

#include "SN76489.h"

static void addchn(struct SN76489 *const sn, unsigned int chn, unsigned int off)
{
	if (chn < 2)
	{
		int tmax = SN76489_VCH_MAX;

		for (int i = 2; i >= 0; i--)
		{
			unsigned int vcnt = 0;

			for (int j = 0; j < SN76489_VCH_MAX; j++)
				if (sn->slot[i][j].ptr >= 0)
					vcnt++;

			if (vcnt == 0)
			{
				chn = i;
				break;
			}

			if (vcnt < tmax)
			{
				tmax = vcnt;
				chn = i;
			}
		}
	}

	int vchn = -1;

	for (int i = 0; i < SN76489_VCH_MAX; i++)
		if (sn->slot[chn][i].ptr < 0)
		{
			vchn = i;
			break;
		}

	if (vchn < 0)
	{
		int ntime = 0;
		int tmax = -1;

		for (int i = 0; i < SN76489_VCH_MAX; i++)
		{
			int ntime = sn->slot[chn][i].time;

			if (ntime > tmax)
			{
				tmax = ntime;
				vchn = i;
			}
		}
	}

	struct SN76489_slot *const slot = &sn->slot[chn][vchn];
	slot->ptr = off;
	slot->wait = 0;
	slot->time = 0;
}

//

void SN76489_play(struct SN76489 *const sn, unsigned char track)
{
	volatile unsigned char *pb = (unsigned char *)SN76489_DATA;

	*pb = 0x9f;
	*pb = 0xbf;
	*pb = 0xdf;
	*pb = 0xff;

	for (unsigned int i = 0; i < SN76489_CHN_MAX; i++)
		for (unsigned int j = 0; j < SN76489_VCH_MAX; j++)
		{
			struct SN76489_slot *const slot = &sn->slot[i][j];

			slot->ptr = -1;
			slot->wait = 0;
		}

	unsigned int eoff = 2 + (track << 1);
	unsigned int doff = (sn->data[eoff] << 8) + sn->data[eoff + 1];
	unsigned int chcnt = sn->data[doff++];

	for (unsigned int i = 0; i < chcnt; i++)
	{
		eoff = (sn->data[doff++] << 8);
		eoff += sn->data[doff++];
		unsigned int chn = sn->data[eoff++];
		addchn(sn, chn, eoff);
	}
}

void SN76489_update(struct SN76489 *const sn)
{
	if (!sn->data)
		return;

	volatile unsigned char *pb = (unsigned char *)SN76489_DATA;

	for (unsigned int pchn = 0; pchn < SN76489_CHN_MAX; pchn++)
	{
		for (unsigned int vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			struct SN76489_slot *const slot = &sn->slot[pchn][vchn];

			if (slot->ptr < 0)
				continue;

			slot->time++;

			if (slot->wait)
			{
				slot->wait--;
				continue;
			}

			unsigned char *const data = sn->data;
			unsigned char mbyte = data[slot->ptr++];

			switch (mbyte & 0xc0)
			{
			case 0x00: // 0=eof 1..31=wait
				if (!mbyte)
					slot->ptr = -1;
				else
					slot->wait = mbyte - 1;
				break;
			case 0x40: // vol only
				slot->vol = mbyte & 0x0f;
				break;
			case 0x80: // div only
				slot->div = ((unsigned int)mbyte << 8) | data[slot->ptr++];
				break;
			case 0xc0: // vol and div
				slot->vol = (mbyte >> 2) & 0x0f;
				slot->div = ((unsigned int)(mbyte & 0x03) << 8) | data[slot->ptr++];
				break;
			}
		}

		int rchn = -1;
		unsigned char mvol = 16;

		for (unsigned int vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			if (sn->slot[pchn][vchn].ptr < 0)
				continue;

			unsigned char nvol = sn->slot[pchn][vchn].vol;

			if (nvol < mvol)
			{
				mvol = nvol;
				rchn = vchn;
			}
		}

		if (rchn >= 0)
		{
			unsigned int vchn = rchn;
			rchn = pchn << 5;
			*pb = 0x80 | 0x10 | rchn | sn->slot[pchn][vchn].vol;
			unsigned int div = sn->slot[pchn][vchn].div;
			*pb = 0x80 | rchn | (div & 0x0f);
			*pb = div >> 4;
		}
	}
}