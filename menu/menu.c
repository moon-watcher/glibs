#include <genesis.h>
#include "menu.h"

menu_t *menu_create(void (*handler)(), int (*selected)(), int (*draw)())
{
    menu_t *this = malloc(sizeof(menu_t));

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

void menu_destroy(menu_t *this)
{
    if (this == NULL)
        return;

    while (this->head != NULL)
    {
        menuOption_t *mo = this->head;
        this->head = mo->next;

        free(mo);
    }

    free(this);
    this = NULL;
}

menuOption_t *menu_addOption(menu_t *const this, void *const data, menu_t *const submenu)
{
    if (this == NULL)
        return NULL;

    menuOption_t *mo = malloc(sizeof(menuOption_t));
    memcpy(&mo->data, data, CONFIG_MENU_DATASIZE);

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

void menu_drawAll(menu_t *const this)
{
    if (this == NULL)
        return;

    menuOption_t *mo = this->head;

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

int menu_update(menu_t *const this)
{
    if (this == NULL)
        return ERROR_MENU_UPDATE;

    if (this->handler == NULL)
        return ERROR_MENU_UPDATE_HANDLER;

    int ret = this->handler(
        this,
        menu_getSelected(this),
        menu_getIndex(this));

    // update submenu
    menu_update(menu_getSubmenu(this));

    return ret;
}

menuOption_t *menu_incOption(menu_t *const this)
{
    if (this == NULL)
        return NULL;

    menuOption_t *mo = this->selectedOption;

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

menuOption_t *menu_decOption(menu_t *const this)
{
    if (this == NULL)
        return NULL;

    menuOption_t *mo = this->selectedOption;

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

int menu_drawSelected(menu_t *const this)
{
    if (this == NULL)
        return ERROR_MENU_DRAWSELECTED;

    if (this->drawSelected == NULL)
        return ERROR_MENU_DRAWSELECTED_DRAWSELECTED;

    if (this->selectedOption == NULL)
        return ERROR_MENU_DRAWSELECTED_SELECTEDOPTION;

    if (this->selectedOption->data == NULL)
        return ERROR_MENU_DRAWSELECTED_DATA;

    return this->drawSelected(this->selectedOption->data);
}

int menu_drawOption(menu_t *const this, menuOption_t *const mo)
{
    if (this == NULL)
        return ERROR_MENU_DRAWOPTION;

    if (this->drawOption == NULL)
        return ERROR_MENU_DRAWOPTION_DRAWOPTION;

    return this->drawOption(&mo->data);
}

menu_t *menu_getSubmenu(menu_t *const this)
{
    if (this == NULL)
        return NULL;

    menuOption_t *mo = this->selectedOption;
    return mo ? mo->submenu : NULL;
}

void menu_selectOption(menu_t *const this, menuOption_t *const mo)
{
    if (this == NULL)
        return;

    this->selectedOption = mo;
}

menuOption_t *menu_getOptionByIndex(menu_t *const this, unsigned index)
{
    if (this == NULL)
        return NULL;

    menuOption_t *mo = this->head;
    unsigned i = 0;

    while (mo)
    {
        if (i++ == index)
            return mo;

        mo = mo->next;
    }

    return NULL;
}

menuOption_t *menu_getSelected(menu_t *const this)
{
    if (this == NULL)
        return NULL;

    return this->selectedOption;
}

int menu_getIndex(menu_t *const this)
{
    if (this == NULL)
        return ERROR_MENU_GETINDEX;

    if (this->selectedOption == NULL)
        return ERROR_MENU_GETINDEX_SELECTEDOPTION;

    menuOption_t *mo = this->head;
    unsigned i = 0;

    while (mo)
    {
        if (this->selectedOption == mo)
            return i;

        ++i;
        mo = mo->next;
    }

    return ERROR_MENU_GETINDEX_GETINDEX;
}

void menu_deactivate(menu_t *const this, bool recursive)
{
    if (this == NULL)
        return;

    menuOption_t *mo = menu_getSelected(this);

    if (mo != NULL)
        menu_drawOption(this, mo);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_deactivate(this->selectedOption->submenu, recursive);
}

void menu_activate(menu_t *const this, bool recursive)
{
    if (this == NULL)
        return;

    menuOption_t *mo = menu_getSelected(this);

    if (mo == NULL)
        menu_selectOption(this, menu_getOptionByIndex(this, 0));

    menu_drawSelected(this);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        menu_activate(this->selectedOption->submenu, recursive);
}
