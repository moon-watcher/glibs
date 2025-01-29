#pragma once

#define DARKEN_LOG(MESSAGE)

#include "config.h"

#define DARKEN_ASSERT(EVAL, RETURN, MESSAGE) \
    ({                                       \
        if (EVAL)                            \
        {                                    \
            DARKEN_LOG(MESSAGE);             \
            return RETURN;                   \
        }                                    \
        RETURN;                              \
    })
