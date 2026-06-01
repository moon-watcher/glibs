#pragma once

#include <stdint.h>

struct menu;
struct menuOption;

struct menuOption
{
    struct menu *parent;
    struct menu *child;
    struct menuOption *next;
    struct menuOption *prev;
    int16_t (*exec_f)(uint16_t, uint8_t *);

    uint16_t index;
    uint8_t *data;
};

typedef int16_t (*menuOption_f)(struct menuOption *);

struct menu
{
    menuOption_f incOption_f;
    menuOption_f decOption_f;
    menuOption_f fireOption_f;

    int16_t (*drawOption_f)(void *);
    int16_t (*drawSelected_f)(void *);

    uint8_t round : 1;
    uint8_t singleOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

void menu_init(struct menu *, menuOption_f, menuOption_f, menuOption_f, int16_t (*)(), int16_t (*)());
void menu_add(struct menu *, struct menuOption *, void *, menuOption_f);
void menu_draw(struct menu *);
int16_t menu_update(struct menu *);

void menu_option_submenu(struct menuOption *, struct menu *);
void menu_option_select(struct menuOption *);
