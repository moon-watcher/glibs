#pragma once

#define UCLIST_ASSERT(EVAL, RETURN) \
    if (EVAL)                       \
    {                               \
        return RETURN;              \
    }
