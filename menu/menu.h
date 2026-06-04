#pragma once

#include <stdint.h>

struct menu;
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
    struct menu *parent;
    struct menu *child;
    struct menuOption *next;
    struct menuOption *prev;
    menuOption_f exec;

    uint16_t index;
    void *data;
};

struct menu
{
    struct menuEvents event;

    uint8_t round : 1;
    uint8_t singleOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

void menu_init(struct menu *, struct menuEvents *, int16_t, int16_t);
void menu_add(struct menu *, struct menuOption *, void *, menuOption_f);
void menu_draw(struct menu *);
void menu_draw_selected(struct menu *);
int16_t menu_update(struct menu *);

void menu_option_draw(struct menuOption *);
void menu_option_submenu(struct menuOption *, struct menu *);
void menu_option_select(struct menuOption *);
