#pragma once

#include "option.h"

struct menu
{
    int (*handler)(struct menu *const, struct menuOption *const, int);

    int (*drawOption)(void *const);
    int (*drawSelected)(void *const);

    unsigned int round : 1;
    unsigned int oneOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

struct menu *menu_create(void (*)(), int (*)(), int (*)());
void menu_destroy(struct menu *);
struct menuOption *menu_addOption(struct menu *const, void *const, struct menu *const);
void menu_drawAll(struct menu *const);
int menu_update(struct menu *const);
struct menuOption *menu_incOption(struct menu *const);
struct menuOption *menu_decOption(struct menu *const);
int menu_drawSelected(struct menu *const);
int menu_drawOption(struct menu *const, struct menuOption *const);
struct menu *menu_getSubmenu(struct menu *const);
void menu_selectOption(struct menu *const, struct menuOption *const);
struct menuOption *menu_getOptionByIndex(struct menu *const, unsigned int);
struct menuOption *menu_getSelected(struct menu *const);
int menu_getIndex(struct menu *const);
void menu_deactivate(struct menu *const, unsigned int);
void menu_activate(struct menu *const, unsigned int);

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