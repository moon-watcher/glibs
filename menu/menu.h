#pragma once

#include <stdint.h>

struct menu;
typedef int16_t (*menuOption_handler_f)(struct menuOption *);

#define MENU_OPTION_DATA_MAX 32

struct menuOption
{
    struct menu *submenu;
    struct menuOption *next;
    struct menuOption *prev;
    menuOption_handler_f exec_f;

    uint8_t data[MENU_OPTION_DATA_MAX]; // data buffer
};


struct menu
{
    menuOption_handler_f incOption_f;
    menuOption_handler_f decOption_f;
    menuOption_handler_f fireOption_f;

    int16_t (*drawOption_f)(void *);
    int16_t (*drawSelected_f)(void *);

    uint8_t round : 1;
    uint8_t singleOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

void menu_init(struct menu *, menuOption_handler_f, menuOption_handler_f, menuOption_handler_f, int16_t (*)(), int16_t (*)());
void menu_addOption(struct menu *, struct menuOption *, const void *, struct menu *, menuOption_handler_f);
void menu_draw(struct menu *);
void menu_selectOption(struct menu *, struct menuOption *);
int16_t menu_update(struct menu *);

