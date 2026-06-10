#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t *buffer;
    uint16_t size;
    uint16_t offset;
} stack_t;

#define stack_init(STACK, BUFFER)        \
    do                                   \
    {                                    \
        stack_t *s = (STACK);            \
        s->buffer = (uint8_t *)(BUFFER); \
        s->size = sizeof(BUFFER);        \
        s->offset = 0;                   \
    } while (0)

#define stack_push(STACK, TYPE, VALUE)                  \
    ({                                                  \
        stack_t *s = (STACK);                           \
        TYPE *ret = 0;                                  \
        uint16_t new_offset = s->offset + sizeof(TYPE); \
        if (new_offset <= s->size)                      \
        {                                               \
            ret = (TYPE *)(s->buffer + s->offset);      \
            *ret = (VALUE);                             \
            s->offset = new_offset;                     \
        }                                               \
        ret;                                            \
    })

#define stack_pop(STACK, TYPE)                     \
    ({                                             \
        stack_t *s = (STACK);                      \
        TYPE *ret = 0;                             \
        if (s->offset >= sizeof(TYPE))             \
        {                                          \
            s->offset -= sizeof(TYPE);             \
            ret = (TYPE *)(s->buffer + s->offset); \
        }                                          \
        ret;                                       \
    })

#define stack_peek(STACK, TYPE)                              \
    ({                                                       \
        stack_t *s = (STACK);                                \
        s->offset >= sizeof(TYPE)                            \
            ? (TYPE *)(s->buffer + s->offset - sizeof(TYPE)) \
            : (TYPE *)0;                                     \
    })

#define stack_used(STACK) ((STACK)->offset)
#define stack_left(STACK) ((STACK)->size - (STACK)->offset)
#define stack_total(STACK) ((STACK)->size)
#define stack_reset(STACK) ((STACK)->offset = 0)
