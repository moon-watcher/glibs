#pragma once

#include <stdint.h>

struct menuOption;

typedef int16_t (*menuOption_f)(struct menuOption *);

struct menuEvents
{
    menuOption_f inc;
    menuOption_f dec;
    menuOption_f fire;
    menuOption_f change;
    void (*drawOption)(void *);
    void (*drawSelected)(void *);
};

struct menuOption
{
    struct menu *child;
    struct menuOption *next;
    struct menuOption *prev;
    menuOption_f exec;

    uint16_t index;
    void *data;
};

struct menu
{
    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;

    struct menuEvents event;

    uint8_t singleOption : 1;
    uint8_t round : 1;
};

void menu_init(struct menu *, struct menuEvents *, int16_t, int16_t);
void menu_add(struct menu *, struct menuOption *, void *, menuOption_f);
void menu_draw(struct menu *);
void menu_draw_selected(struct menu *);
int16_t menu_update(struct menu *);
void menu_draw_option(struct menu *, struct menuOption *);
void menu_select_option(struct menu *, struct menuOption *);

void menu_option_submenu(struct menuOption *, struct menu *);
