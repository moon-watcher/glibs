/**
 * PSG multichannel sound effects player v1.0 by Shiru, 03.11.07
 *
 * gcc version 12.12.07
 *
 * @MoonWatcherMD at 20151019. Thanks, Shiru!
 * @MoonWatcherMD at 20230822. Refactor
 */

#define SN76489_DATA 0xc00011
#define SN76489_VCH_MAX 4
#define SN76489_CHN_MAX 4

struct Slot
{
	int ptr;
	int wait;
	int time;
	unsigned int div;
	unsigned char vol;
};

static struct Slot slots[SN76489_CHN_MAX][SN76489_VCH_MAX] = {};
static void addch(unsigned int, unsigned int);

void SN76489_init()
{
	for (unsigned int pchn = 0; pchn < SN76489_CHN_MAX; pchn++)
		for (unsigned int vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			struct Slot *const slot = &slots[pchn][vchn];

			slot->ptr = -1;
			slot->wait = -1;
			slot->time = -1;
			slot->div = 0;
			slot->vol = 0;
		}
}

void SN76489_update(const unsigned char *const data)
{
	if (!data)
		return;

	for (unsigned int pchn = 0; pchn < SN76489_CHN_MAX; pchn++)
	{
		for (unsigned int vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			struct Slot *const slot = &slots[pchn][vchn];

			if (slot->ptr < 0)
				continue;

			slot->time++;

			if (slot->wait)
			{
				slot->wait--;
				continue;
			}

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
		unsigned int mvol = 16;

		for (unsigned int vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			struct Slot *const slot = &slots[pchn][vchn];

			if (slot->ptr >= 0 && slot->vol < mvol)
			{
				mvol = slot->vol;
				rchn = vchn;
			}
		}

		if (rchn >= 0)
		{
			volatile unsigned char *pb = (unsigned char *)SN76489_DATA;
			struct Slot *const slot = &slots[pchn][rchn];

			rchn = pchn << 5;
			*pb = 0x80 | 0x10 | rchn | slot->vol;
			*pb = 0x80 | rchn | (slot->div & 0x0f);
			*pb = slot->div >> 4;
		}
	}
}

void SN76489_play(const unsigned char *const data, unsigned char track)
{
	volatile unsigned char *pb = (unsigned char *)SN76489_DATA;

	*pb = 0x9f;
	*pb = 0xbf;
	*pb = 0xdf;
	*pb = 0xff;

	for (unsigned int i = 0; i < SN76489_CHN_MAX; i++)
		for (unsigned int j = 0; j < SN76489_VCH_MAX; j++)
		{
			struct Slot *const slot = &slots[i][j];

			slot->ptr = -1;
			slot->wait = 0;
		}

	unsigned int eoff = 2 + (track << 1);
	unsigned int doff = (data[eoff] << 8) + data[eoff + 1];
	unsigned int chcnt = data[doff++];

	for (unsigned int i = 0; i < chcnt; i++)
	{
		eoff = (data[doff++] << 8);
		eoff += data[doff++];
		unsigned int chn = data[eoff++];
		addch(chn, eoff);
	}
}

static void addch(unsigned int chn, unsigned int eoff)
{
	if (chn < 2)
	{
		unsigned int tmax = SN76489_VCH_MAX;

		for (int i = 2; i >= 0; i--)
		{
			unsigned int vcnt = 0;

			for (unsigned int j = 0; j < SN76489_VCH_MAX; j++)
				if (slots[i][j].ptr >= 0)
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

	for (unsigned int i = 0; i < SN76489_VCH_MAX; i++)
		if (slots[chn][i].ptr < 0)
		{
			vchn = i;
			break;
		}

	if (vchn < 0)
	{
		int tmax = -1;

		for (unsigned int i = 0; i < SN76489_VCH_MAX; i++)
		{
			struct Slot *const slot = &slots[chn][i];

			if (slot->time > tmax)
			{
				tmax = slot->time;
				vchn = i;
			}
		}
	}

	struct Slot *const slot = &slots[chn][vchn];

	slot->ptr = eoff;
	slot->wait = 0;
	slot->time = 0;
}