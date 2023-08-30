#include "menu.h"

#include "../_config/free.h"
#include "../_config/malloc.h"
#include "../_config/memcpy.h"

struct menu *menu_create(void (*handler)(), int (*selected)(), int (*draw)())
{
    struct menu *this = malloc(sizeof(struct menu));

    this->drawOption = draw;
    this->drawSelected = selected;
    this->round = 0;
    this->oneOption = 0;
    this->handler = handler;
    this->head = NULL;
    this->tail = NULL;
    this->selectedOption = NULL;

    return this;
}

void menu_destroy(struct menu *this)
{
    if (this == NULL)
        return;

    while (this->head != NULL)
    {
        struct menuOption *mo = this->head;
        this->head = mo->next;

        free(mo);
    }

    free(this);
    this = NULL;
}

struct menuOption *menu_addOption(struct menu *const this, void *const data, struct menu *const submenu)
{
    if (this == NULL)
        return NULL;

    struct menuOption *mo = malloc(sizeof(struct menuOption));
    memcpy(&mo->data, data, MENU_CONFIG_DATASIZE);

    mo->submenu = submenu;
    mo->next = NULL;
    mo->prev = NULL;

    if (this->head == NULL)
        this->head = mo;
    else
    {
        mo->prev = this->tail;
        this->tail->next = mo;
    }

    this->tail = mo;

    return mo;
}

void menu_drawAll(struct menu *const this)
{
    if (this == NULL)
        return;

    struct menuOption *mo = this->head;

    if (this->oneOption == 0)
        while (mo)
        {
            menu_drawOption(this, mo);

            if (mo->submenu)
                menu_drawAll(mo->submenu);

            mo = mo->next;
        }

    if (this->selectedOption != NULL)
        menu_drawSelected(this);
    else
        menu_drawOption(this, menu_getOptionByIndex(this, 0));
}

int menu_update(struct menu *const this)
{
    if (this == NULL)
        return MENU_ERROR_UPDATE;

    if (this->handler == NULL)
        return MENU_ERROR_UPDATE_HANDLER;

    int ret = this->handler(this, menu_getSelected(this), menu_getIndex(this));
    // update submenu
    menu_update(menu_getSubmenu(this));

    return ret;
}

struct menuOption *menu_incOption(struct menu *const this)
{
    if (this == NULL)
        return NULL;

    struct menuOption *mo = this->selectedOption;

    if (mo == NULL)
        return NULL;

    if (mo->next != NULL)
        this->selectedOption = mo->next;
    else if (this->round != 0)
        this->selectedOption = this->head;
    if (this->selectedOption != mo)
    {
        menu_drawOption(this, mo);
        menu_drawSelected(this);
    }

    return this->selectedOption;
}

struct menuOption *menu_decOption(struct menu *const this)
{
    if (this == NULL)
        return NULL;

    struct menuOption *mo = this->selectedOption;

    if (mo == NULL)
        return NULL;

    if (mo->prev != NULL)
        this->selectedOption = mo->prev;
    else if (this->round != 0)
        this->selectedOption = this->tail;

    if (this->selectedOption != mo)
    {
        menu_drawOption(this, mo);
        menu_drawSelected(this);
    }

    return this->selectedOption;
}

int menu_drawSelected(struct menu *const this)
{
    if (this == NULL)
        return MENU_ERROR_DRAWSELECTED;

    if (this->drawSelected == NULL)
        return MENU_ERROR_DRAWSELECTED_DRAWSELECTED;

    if (this->selectedOption == NULL)
        return MENU_ERROR_DRAWSELECTED_SELECTEDOPTION;

    if (this->selectedOption->data == NULL)
        return MENU_ERROR_DRAWSELECTED_DATA;

    return this->drawSelected(this->selectedOption->data);
}

int menu_drawOption(struct menu *const this, struct menuOption *const mo)
{
    if (this == NULL)
        return MENU_ERROR_DRAWOPTION;

    if (this->drawOption == NULL)
        return MENU_ERROR_DRAWOPTION_DRAWOPTION;

    return this->drawOption(&mo->data);
}

struct menu *menu_getSubmenu(struct menu *const this)
{
    if (this == NULL)
        return NULL;

    struct menuOption *mo = this->selectedOption;
    return mo ? mo->submenu : NULL;
}

void menu_selectOption(struct menu *const this, struct menuOption *const mo)
{
    if (this == NULL)
        return;

    this->selectedOption = mo;
}

struct menuOption *menu_getOptionByIndex(struct menu *const this, unsigned int index)
{
    if (this == NULL)
        return NULL;

    struct menuOption *mo = this->head;
    unsigned int i = 0;

    while (mo)
    {
        if (i++ == index)
            return mo;

        mo = mo->next;
    }

    return NULL;
}

struct menuOption *menu_getSelected(struct menu *const this)
{
    if (this == NULL)
        return NULL;

    return this->selectedOption;
}

int menu_getIndex(struct menu *const this)
{
    if (this == NULL)
        return MENU_ERROR_GETINDEX;

    if (this->selectedOption == NULL)
        return MENU_ERROR_GETINDEX_SELECTEDOPTION;

    struct menuOption *mo = this->head;
    unsigned int i = 0;

    while (mo)
    {
        if (this->selectedOption == mo)
            return i;

        ++i;
        mo = mo->next;
    }

    return MENU_ERROR_GETINDEX_GETINDEX;
}

void menu_deactivate(struct menu *const this, unsigned int recursive)
{
    if (this == NULL)
        return;

    struct menuOption *mo = menu_getSelected(this);

    if (mo != NULL)
        menu_drawOption(this, mo);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_deactivate(this->selectedOption->submenu, recursive);
}

void menu_activate(struct menu *const this, unsigned int recursive)
{
    if (this == NULL)
        return;

    struct menuOption *mo = menu_getSelected(this);

    if (mo == NULL)
        menu_selectOption(this, menu_getOptionByIndex(this, 0));

    menu_drawSelected(this);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_activate(this->selectedOption->submenu, recursive);
}
