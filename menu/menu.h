#pragma once

#include "option.h"

typedef struct menu_t menu_t;

typedef struct menu_t
{
    int (*handler)(menu_t *const menu, menuOption_t *const, int);

    int (*drawOption)(void *const);
    int (*drawSelected)(void *const);

    unsigned round : 1;
    unsigned oneOption : 1;

    menuOption_t *head;
    menuOption_t *tail;
    menuOption_t *selectedOption;
} menu_t;

menu_t *menu_create(void (*handler)(), int (*selected)(), int (*draw)());
void menu_destroy(menu_t *menu);
menuOption_t *menu_addOption(menu_t *const menu, void *const data, menu_t *const submenu);
void menu_drawAll(menu_t *const menu);
int menu_update(menu_t *const menu);
menuOption_t *menu_incOption(menu_t *const menu);
menuOption_t *menu_decOption(menu_t *const menu);
int menu_drawSelected(menu_t *const menu);
int menu_drawOption(menu_t *const menu, menuOption_t *const);
menu_t *menu_getSubmenu(menu_t *const menu);
void menu_selectOption(menu_t *const menu, menuOption_t *const);
menuOption_t *menu_getOptionByIndex(menu_t *const menu, unsigned index);
menuOption_t *menu_getSelected(menu_t *const menu);
int menu_getIndex(menu_t *const menu);
void menu_deactivate(menu_t *const menu, bool recursive);
void menu_activate(menu_t *const menu, bool recursive);

enum
{
    ERROR_MENU_UPDATE = -9999,
    ERROR_MENU_UPDATE_HANDLER,
    ERROR_MENU_DRAWSELECTED,
    ERROR_MENU_DRAWSELECTED_DRAWSELECTED,
    ERROR_MENU_DRAWSELECTED_SELECTEDOPTION,
    ERROR_MENU_DRAWSELECTED_DATA,
    ERROR_MENU_DRAWOPTION,
    ERROR_MENU_DRAWOPTION_DRAWOPTION,
    ERROR_MENU_GETINDEX,
    ERROR_MENU_GETINDEX_SELECTEDOPTION,
    ERROR_MENU_GETINDEX_GETINDEX,
};