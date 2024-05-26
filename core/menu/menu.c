#include "menu.h"

void menu_init(struct menu *this, void (*handler)(), int (*selected)(), int (*draw)())
{
    this->drawOption = draw;
    this->drawSelected = selected;
    this->round = 0;
    this->oneOption = 0;
    this->handler = handler;
    this->head = 0;
    this->tail = 0;
    this->selectedOption = 0;
}

void menu_addOption(struct menu *this, struct menuOption *mo, void *data, struct menu *submenu)
{
    if (this == 0)
        return 0;

    memcpy(&mo->data, data, MENU_CONFIG_DATASIZE);

    mo->submenu = submenu;
    mo->next = 0;
    mo->prev = 0;

    if (this->head == 0)
        this->head = mo;
    else
    {
        mo->prev = this->tail;
        this->tail->next = mo;
    }

    this->tail = mo;
}

void menu_drawAll(struct menu *this)
{
    if (this == 0)
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

    if (this->selectedOption != 0)
        menu_drawSelected(this);
    else
        menu_drawOption(this, menu_getOptionByIndex(this, 0));
}

int menu_update(struct menu *this)
{
    if (this == 0)
        return MENU_ERROR_UPDATE;

    if (this->handler == 0)
        return MENU_ERROR_UPDATE_HANDLER;

    int ret = this->handler(this, menu_getSelected(this), menu_getIndex(this));
    // update submenu
    menu_update(menu_getSubmenu(this));

    return ret;
}

struct menuOption *menu_incOption(struct menu *this)
{
    if (this == 0)
        return 0;

    struct menuOption *mo = this->selectedOption;

    if (mo == 0)
        return 0;

    if (mo->next != 0)
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

struct menuOption *menu_decOption(struct menu *this)
{
    if (this == 0)
        return 0;

    struct menuOption *mo = this->selectedOption;

    if (mo == 0)
        return 0;

    if (mo->prev != 0)
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

int menu_drawSelected(struct menu *this)
{
    if (this == 0)
        return MENU_ERROR_DRAWSELECTED;

    if (this->drawSelected == 0)
        return MENU_ERROR_DRAWSELECTED_DRAWSELECTED;

    if (this->selectedOption == 0)
        return MENU_ERROR_DRAWSELECTED_SELECTEDOPTION;

    if (this->selectedOption->data == 0)
        return MENU_ERROR_DRAWSELECTED_DATA;

    return this->drawSelected(this->selectedOption->data);
}

int menu_drawOption(struct menu *this, struct menuOption *mo)
{
    if (this == 0)
        return MENU_ERROR_DRAWOPTION;

    if (this->drawOption == 0)
        return MENU_ERROR_DRAWOPTION_DRAWOPTION;

    return this->drawOption(&mo->data);
}

struct menu *menu_getSubmenu(struct menu *this)
{
    if (this == 0)
        return 0;

    struct menuOption *mo = this->selectedOption;
    return mo ? mo->submenu : 0;
}

void menu_selectOption(struct menu *this, struct menuOption *mo)
{
    if (this == 0)
        return;

    this->selectedOption = mo;
}

struct menuOption *menu_getOptionByIndex(struct menu *this, unsigned int index)
{
    if (this == 0)
        return 0;

    struct menuOption *mo = this->head;
    unsigned int i = 0;

    while (mo)
    {
        if (i++ == index)
            return mo;

        mo = mo->next;
    }

    return 0;
}

struct menuOption *menu_getSelected(struct menu *this)
{
    if (this == 0)
        return 0;

    return this->selectedOption;
}

int menu_getIndex(struct menu *this)
{
    if (this == 0)
        return MENU_ERROR_GETINDEX;

    if (this->selectedOption == 0)
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

void menu_deactivate(struct menu *this, unsigned int recursive)
{
    if (this == 0)
        return;

    struct menuOption *mo = menu_getSelected(this);

    if (mo != 0)
        menu_drawOption(this, mo);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_deactivate(this->selectedOption->submenu, recursive);
}

void menu_activate(struct menu *this, unsigned int recursive)
{
    if (this == 0)
        return;

    struct menuOption *mo = menu_getSelected(this);

    if (mo == 0)
        menu_selectOption(this, menu_getOptionByIndex(this, 0));

    menu_drawSelected(this);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_activate(this->selectedOption->submenu, recursive);
}
