#pragma once

#include "option.h"

struct menu
{
    int (*handler)(struct menu*, struct menuOption*, int);

    int (*drawOption)(void*);
    int (*drawSelected)(void*);

    unsigned int round : 1;
    unsigned int oneOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

void menu_init(struct menu*, void (*)(), int (*)(), int (*)());
void menu_addOption(struct menu*, struct menuOption* mo, void*, struct menu*);
void menu_drawAll(struct menu*);
int menu_update(struct menu*);
struct menuOption *menu_incOption(struct menu*);
struct menuOption *menu_decOption(struct menu*);
int menu_drawSelected(struct menu*);
int menu_drawOption(struct menu*, struct menuOption*);
struct menu *menu_getSubmenu(struct menu*);
void menu_selectOption(struct menu*, struct menuOption*);
struct menuOption *menu_getOptionByIndex(struct menu*, unsigned int);
struct menuOption *menu_getSelected(struct menu*);
int menu_getIndex(struct menu*);
void menu_deactivate(struct menu*, unsigned int);
void menu_activate(struct menu*, unsigned int);

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