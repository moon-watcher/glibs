#include "../src/include.h"

#include "files/back.h"
#include "files/flechas.h"
#include "files/item.h"
#include "files/key.h"
#include "files/kill.h"
#include "files/select_2.h"
#include "files/select.h"
#include "files/sfx_steps.h"
#include "files/start.h"
#include "files/wrong.h"
#include "../libpsg.h"

void libpsgTest()
{
    struct libpsg x;

    libpsg_play(&x, kill_data, 0);

    while(1)
    {
        libpsg_update(&x);
        SYS_doVBlankProcess();
    }
}
