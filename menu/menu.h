#pragma once

#include <stdint.h>

struct menu;
struct menuOption;

typedef int16_t (*menuOption_f)(struct menuOption *);

struct menuOption
{
    struct menu *parentmenu; // parent
    struct menu *submenu;    // children
    struct menuOption *next;
    struct menuOption *prev;
    menuOption_f exec_f;

    uint8_t *data;
    uint16_t index;
};

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
