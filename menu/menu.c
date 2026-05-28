#include "menu.h"
#include "debug.h"

#define GUARD(cond, ret) \
    if (!cond)            \
        return ret;

void menu_init(struct menu *this, void (*handler)(), int (*selected_f)(), int (*draw_f)())
{
    this->drawOption_f = draw_f;
    this->drawSelected_f = selected_f;
    this->round = 0;
    this->singleOption = 0;
    this->handler = handler;
    this->head = 0;
    this->tail = 0;
    this->selectedOption = 0;
}

void menu_addOption(struct menu *this, struct menuOption *mo, void *data, struct menu *submenu)
{
    mo->data = data;
    mo->submenu = submenu;
    mo->next = 0;
    mo->prev = 0;

    if (!this->head)
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
    struct menuOption *mo = this->head;

    if (!this->singleOption)
        while (mo)
        {
            menu_drawOption(this, mo);

            if (mo->submenu)
                menu_drawAll(mo->submenu);

            mo = mo->next;
        }

    if (this->selectedOption)
        menu_drawSelected(this);
    else
        menu_drawOption(this, menu_getOptionByIndex(this, 0));
}

int menu_update(struct menu *this)
{
    GUARD(this->handler, MENU_ERROR_HANDLER);

    unsigned index = menu_getIndex(this);
    struct menuOption *mo = menu_getSelected(this);
    
    this->handler(this, mo, index);
    struct menu *submenu = menu_getSubmenu(this);

    if (submenu)
        menu_update(submenu);

    return 1;
}

struct menuOption *menu_incOption(struct menu *this)
{
    struct menuOption *mo = this->selectedOption;

    GUARD(mo, 0);

    if (mo->next)
        this->selectedOption = mo->next;
        
    else if (this->round)
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
    struct menuOption *mo = this->selectedOption;

    GUARD(mo, 0);

    if (mo->prev)
        this->selectedOption = mo->prev;
    else if (this->round)
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
    GUARD(this->drawSelected_f, MENU_ERROR_DRAWSELECTED);
    GUARD(this->selectedOption, MENU_ERROR_SELECTEDOPTION);
    GUARD(this->selectedOption->data, MENU_ERROR_DATA);

    return this->drawSelected_f(this->selectedOption->data);
}

int menu_drawOption(struct menu *this, struct menuOption *mo)
{
    GUARD(this->drawOption_f, MENU_ERROR_DRAWOPTION);

    return this->drawOption_f(mo->data);
}

struct menu *menu_getSubmenu(struct menu *this)
{
    struct menuOption *mo = this->selectedOption;
    return mo ? mo->submenu : 0;
}

void menu_selectOption(struct menu *this, struct menuOption *mo)
{
    this->selectedOption = mo;
}

struct menuOption *menu_getOptionByIndex(struct menu *this, unsigned int index)
{
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
    return this->selectedOption;
}

int menu_getIndex(struct menu *this)
{
    GUARD(this->selectedOption, MENU_ERROR_GETINDEX);

    struct menuOption *mo = this->head;
    unsigned int i = 0;

    while (mo)
    {
        if (this->selectedOption == mo)
            return i;

        ++i;
        mo = mo->next;
    }

    return MENU_ERROR_NOINDEX;
}

void menu_deactivate(struct menu *this, unsigned int recursive)
{
    struct menuOption *mo = menu_getSelected(this);

    if (mo)
        menu_drawOption(this, mo);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_deactivate(this->selectedOption->submenu, recursive);
}

void menu_activate(struct menu *this, unsigned int recursive)
{
    struct menuOption *mo = menu_getSelected(this);

    if (mo)
        menu_selectOption(this, menu_getOptionByIndex(this, 0));

    menu_drawSelected(this);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_activate(this->selectedOption->submenu, recursive);
}
