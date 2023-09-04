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
	short ptr;
	short wait;
	short time;
	unsigned short div;
	unsigned char vol;
};

static struct Slot slots[SN76489_CHN_MAX][SN76489_VCH_MAX] = {};

void SN76489_update(const unsigned char *const data)
{
	if (!data)
	{
		return;
	}

	volatile unsigned char *pb = (unsigned char *)SN76489_DATA;

	for (unsigned short pchn = 0; pchn < SN76489_CHN_MAX; pchn++)
	{
		for (unsigned short vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			struct Slot *const slot = &slots[pchn][vchn];

			if (slot->ptr < 0)
			{
				continue;
			}

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
					slot->div = ((unsigned short)mbyte << 8) | data[slot->ptr++];
					break;
				case 0xc0: // vol and div
					slot->vol = (mbyte >> 2) & 0x0f;
					slot->div = ((unsigned short)(mbyte & 0x03) << 8) | data[slot->ptr++];
					break;
			}
		}

		short rchn = -1;
		unsigned short mvol = 16;

		for (unsigned short vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
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

	for (unsigned short pchn = 0; pchn < SN76489_CHN_MAX; pchn++)
	{
		for (unsigned short vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			struct Slot *const slot = &slots[pchn][vchn];

			slot->ptr = -1;
			slot->wait = 0;
			slot->time = 0;
		}
	}

	unsigned short eoff = 2 + (track << 1);
	unsigned short doff = (data[eoff] << 8) + data[eoff + 1];
	unsigned short chcnt = data[doff++];

	while (chcnt--)
	{
		eoff = (data[doff++] << 8);
		eoff += data[doff++];
		unsigned short chn = data[eoff++];
		unsigned short maxvch = SN76489_VCH_MAX;
		short tmax = -1;
		short value = -1;

		if (chn < 2)
		{
			for (short i = 2; i >= 0; i--)
			{
				unsigned short vcnt = 0;

				for (unsigned short vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
				{
					struct Slot *const slot = &slots[i][vchn];

					if (slot->ptr >= 0)
					{
						vcnt++;
					}
				}

				if (vcnt == 0)
				{
					chn = i;
					break;
				}

				if (vcnt < maxvch)
				{
					maxvch = vcnt;
					chn = i;
				}
			}
		}

		for (unsigned short vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
		{
			struct Slot *const slot = &slots[chn][vchn];

			if (slot->ptr < 0)
			{
				value = vchn;
				break;
			}
		}

		if (value < 0)
		{
			for (unsigned short vchn = 0; vchn < SN76489_VCH_MAX; vchn++)
			{
				struct Slot *const slot = &slots[chn][vchn];

				if (slot->time > tmax)
				{
					tmax = slot->time;
					value = vchn;
				}
			}
		}

		struct Slot *const slot = &slots[chn][value];

		slot->ptr = eoff;
	}
}