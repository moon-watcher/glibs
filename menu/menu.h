#pragma once

struct menu;
typedef int (*menu_handler_f)(struct menu *, struct menuOption *, int);

struct menuOption
{
    struct menu *submenu;
    struct menuOption *next;
    struct menuOption *prev;
    menu_handler_f exec_f;

    unsigned char *data;
};


struct menu
{
    menu_handler_f inc_f;
    menu_handler_f dec_f;
    menu_handler_f fireOption_f;

    int (*drawOption_f)(void *);
    int (*drawSelected_f)(void *);

    unsigned int round : 1;
    unsigned int singleOption : 1;

    struct menuOption *head;
    struct menuOption *tail;
    struct menuOption *selectedOption;
};

void menu_init(struct menu *, menu_handler_f, menu_handler_f, menu_handler_f, int (*)(), int (*)());
void menu_addOption(struct menu *, struct menuOption *, void *, struct menu *, menu_handler_f);
void menu_draw(struct menu *);
void menu_selectOption(struct menu *, struct menuOption *);
int menu_update(struct menu *);
