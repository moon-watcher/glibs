#pragma once

struct menu;
typedef int (*menuOption_handler_f)(struct menuOption *);

struct menuOption
{
    struct menu *submenu;
    struct menuOption *next;
    struct menuOption *prev;
    menuOption_handler_f exec_f;

    unsigned char *data;
};


struct menu
{
    menuOption_handler_f incOption_f;
    menuOption_handler_f decOption_f;
    menuOption_handler_f fireOption_f;

    int (*drawOption_f)(void *);
    int (*drawSelected_f)(void *);

    unsigned int round : 1;
    unsigned int singleOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

void menu_init(struct menu *, menuOption_handler_f, menuOption_handler_f, menuOption_handler_f, int (*)(), int (*)());
void menu_addOption(struct menu *, struct menuOption *, void *, struct menu *, menuOption_handler_f);
void menu_draw(struct menu *);
void menu_selectOption(struct menu *, struct menuOption *);
int menu_update(struct menu *);

