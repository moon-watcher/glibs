#pragma once

#include <stdint.h>
#include <string.h>

typedef struct
{
    uint8_t *buffer;
    uint16_t size;
    uint16_t offset;
} pool_t;

#define pool_init(POOL, BUFFER)          \
    do                                   \
    {                                    \
        pool_t *p = (POOL);              \
        p->buffer = (uint8_t *)(BUFFER); \
        p->size = sizeof(BUFFER);        \
        p->offset = 0;                   \
        memset(p->buffer, 0, p->size);   \
    } while (0)

#define pool_alloc(POOL, TYPE)                                                                 \
    ({                                                                                         \
        pool_t *p = (POOL);                                                                    \
        void *ret = 0;                                                                         \
                                                                                               \
        if (p->buffer)                                                                         \
        {                                                                                      \
            uint32_t start = (uint32_t)(p->buffer + p->offset);                                \
            uint32_t aligned = (start + _Alignof(TYPE) - 1) & ~((uint32_t)_Alignof(TYPE) - 1); \
            uint16_t new_offset = (uint16_t)((aligned - (uint32_t)p->buffer) + sizeof(TYPE));  \
                                                                                               \
            if (new_offset <= p->size)                                                         \
            {                                                                                  \
                p->offset = new_offset;                                                        \
                ret = (void *)aligned;                                                         \
            }                                                                                  \
        }                                                                                      \
                                                                                               \
        ret;                                                                                   \
    })

#define pool_used(POOL) \
    ({ const pool_t *p = (POOL); p->offset; })

#define pool_left(POOL) \
    ({ const pool_t *p = (POOL); p->size - p->offset; })

#define pool_total(POOL) \
    ({ const pool_t *p = (POOL); p->size; })
