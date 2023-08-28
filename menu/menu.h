#pragma once

#include "option.h"

typedef struct menu menu;

typedef struct menu
{
    int (*handler)(menu *const, menuOption *const, int);

    int (*drawOption)(void *const);
    int (*drawSelected)(void *const);

    unsigned int round : 1;
    unsigned int oneOption : 1;

    menuOption *head;
    menuOption *tail;
    menuOption *selectedOption;
} menu;

menu *menu_create(void (*)(), int (*)(), int (*)());
void menu_destroy(menu *);
menuOption *menu_addOption(menu *const, void *const, menu *const);
void menu_drawAll(menu *const);
int menu_update(menu *const);
menuOption *menu_incOption(menu *const);
menuOption *menu_decOption(menu *const);
int menu_drawSelected(menu *const);
int menu_drawOption(menu *const, menuOption *const);
menu *menu_getSubmenu(menu *const);
void menu_selectOption(menu *const, menuOption *const);
menuOption *menu_getOptionByIndex(menu *const, unsigned int);
menuOption *menu_getSelected(menu *const);
int menu_getIndex(menu *const);
void menu_deactivate(menu *const, unsigned int);
void menu_activate(menu *const, unsigned int);

enum
{
    MENU_ERROR_UPDATE = -9999,
    MENU_ERROR_UPDATE_HANDLER,
    MENU_ERROR_DRAWSELECTED,
    MENU_ERROR_DRAWSELECTED_DRAWSELECTED,
    MENU_ERROR_DRAWSELECTED_SELECTEDOPTION,
    MENU_ERROR_DRAWSELECTED_DATA,
    MENU_ERROR_DRAWOPTION,
    MENU_ERROR_DRAWOPTION_DRAWOPTION,
    MENU_ERROR_GETINDEX,
    MENU_ERROR_GETINDEX_SELECTEDOPTION,
    MENU_ERROR_GETINDEX_GETINDEX,
};