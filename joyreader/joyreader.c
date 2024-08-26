#include "joyreader.h"

void joyreader_update(unsigned *joy, unsigned value)
{
    *joy = ((value ^ ((*joy >> 16) & 0xFFFF)) << 16) | value;

    // joy->changed = active ^ joy->active;
    // joy->active = active;

    /*
    x = 0b1010;  printf("x: %32b \n", x);
    y = 200;     printf("y: %32b \n", y);

    c = ( x << 16 ) | y;      printf("c: %32b \n", c);
    x = ( c>>16 ) & 0xFFFF;   printf("x: %32b \n", x);
    y = c & 0xFFFF;           printf("y: %32b \n", y);
    */

    //     *joy = ((active ^ ((*joy >> 16) & 0xFFFF)) << 16) | active;

    // c = ( x << 16 ) | y;

    // joy->changed = active ^ joy->active;
    // joy->active = active;

    // X: changed
    // Y: active
}

void joyreader_press(unsigned *joy, unsigned value)
{
    // joy->active = value;
}