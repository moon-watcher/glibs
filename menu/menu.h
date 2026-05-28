#pragma once

struct menu;

struct menuOption
{
    struct menu *submenu;
    struct menuOption *next;
    struct menuOption *prev;

    unsigned char *data;
};

typedef int (*menu_handler_f)(struct menu *, struct menuOption *, int);

struct menu
{
    menu_handler_f inc_f;
    menu_handler_f dec_f;

    int (*drawOption_f)(void *);
    int (*drawSelected_f)(void *);

    unsigned int round : 1;
    unsigned int singleOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

void menu_init(struct menu *, menu_handler_f, menu_handler_f, int (*)(), int (*)());
void menu_addOption(struct menu *, struct menuOption *mo, void *, struct menu *);
void menu_drawAll(struct menu *);
int menu_update(struct menu *);
struct menuOption *menu_incOption(struct menu *);
struct menuOption *menu_decOption(struct menu *);
int menu_drawSelected(struct menu *);
int menu_drawOption(struct menu *, struct menuOption *);
struct menu *menu_getSubmenu(struct menu *);
void menu_selectOption(struct menu *, struct menuOption *);
struct menuOption *menu_getOptionByIndex(struct menu *, unsigned int);
struct menuOption *menu_getSelected(struct menu *);
int menu_getIndex(struct menu *);
void menu_deactivate(struct menu *, unsigned int);
void menu_activate(struct menu *, unsigned int);

// enum
// {
//     MENU_ERROR_DRAWSELECTED = -99,
//     MENU_ERROR_SELECTEDOPTION,
//     MENU_ERROR_DATA,
//     MENU_ERROR_DRAWOPTION,
//     MENU_ERROR_GETINDEX,
//     MENU_ERROR_NOINDEX,
// };
