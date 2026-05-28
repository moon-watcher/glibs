#include "menu.h"
#include "debug.h"

static struct menuOption *_incOption(struct menu *);
static struct menuOption *_decOption(struct menu *);
static int _drawSelected(struct menu *);
static int _drawOption(struct menu *, struct menuOption *);
static struct menu *_getSubmenu(struct menu *);
static struct menuOption *_getOptionByIndex(struct menu *, unsigned);
static struct menuOption *_getSelected(struct menu *);
static int _getIndex(struct menu *);

//

void menu_init(struct menu *this, menu_handler_f inc_f, menu_handler_f dec_f, int (*selected_f)(), int (*draw_f)())
{
    this->drawOption_f = draw_f;
    this->drawSelected_f = selected_f;
    this->round = 0;
    this->singleOption = 0;
    this->inc_f = inc_f;
    this->dec_f = dec_f;
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
    {
        this->head = mo;
        menu_selectOption(this, mo);
    }
    else
    {
        mo->prev = this->tail;
        this->tail->next = mo;
    }

    this->tail = mo;
}

void menu_draw(struct menu *this)
{
    if (!this->singleOption)
    {
        struct menuOption *mo = this->head;

        while (mo)
        {
            _drawOption(this, mo);

            if (mo->submenu)
                menu_draw(mo->submenu);

            mo = mo->next;
        }
    }

    if (this->selectedOption)
        _drawSelected(this);
    else
        _drawOption(this, _getOptionByIndex(this, 0));
}

void menu_selectOption(struct menu *this, struct menuOption *mo)
{
    this->selectedOption = mo;
}

int menu_update(struct menu *this)
{
    unsigned index = _getIndex(this);
    struct menuOption *mo = _getSelected(this);

    if (this->inc_f && this->inc_f(this, mo, index))
        _incOption(this);

    if (this->dec_f && this->dec_f(this, mo, index))
        _decOption(this);

    struct menu *submenu = _getSubmenu(this);

    if (submenu)
        menu_update(submenu);

    return 1;
}

//

#define GUARD(cond, ret) \
    if (!cond)           \
        return ret;

static struct menuOption *_incOption(struct menu *this)
{
    struct menuOption *mo = this->selectedOption;

    GUARD(mo, 0);

    if (mo->next)
        this->selectedOption = mo->next;

    else if (this->round)
        this->selectedOption = this->head;

    if (this->selectedOption != mo)
    {
        _drawOption(this, mo);
        _drawSelected(this);
    }

    return this->selectedOption;
}

static struct menuOption *_decOption(struct menu *this)
{
    struct menuOption *mo = this->selectedOption;

    GUARD(mo, 0);

    if (mo->prev)
        this->selectedOption = mo->prev;
    else if (this->round)
        this->selectedOption = this->tail;

    if (this->selectedOption != mo)
    {
        _drawOption(this, mo);
        _drawSelected(this);
    }

    return this->selectedOption;
}

static int _drawSelected(struct menu *this)
{
    GUARD(this->drawSelected_f,       -1);
    GUARD(this->selectedOption,       -2);
    GUARD(this->selectedOption->data, -3);

    return this->drawSelected_f(this->selectedOption->data);
}

static int _drawOption(struct menu *this, struct menuOption *mo)
{
    GUARD(this->drawOption_f, -4);

    return this->drawOption_f(mo->data);
}

static struct menu *_getSubmenu(struct menu *this)
{
    struct menuOption *mo = this->selectedOption;
    return mo ? mo->submenu : 0;
}

static struct menuOption *_getOptionByIndex(struct menu *this, unsigned int index)
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

static struct menuOption *_getSelected(struct menu *this)
{
    return this->selectedOption;
}

static int _getIndex(struct menu *this)
{
    GUARD(this->selectedOption, -5);

    struct menuOption *mo = this->head;
    unsigned int i = 0;

    while (mo)
    {
        if (this->selectedOption == mo)
            return i;

        ++i;
        mo = mo->next;
    }

    return -6;
}



/**
 * Igual se van!
 */
static void _deactivate(struct menu *this, unsigned int recursive)
{
    struct menuOption *mo = _getSelected(this);

    if (mo)
        _drawOption(this, mo);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        _deactivate(this->selectedOption->submenu, recursive);
}

static void _activate(struct menu *this, unsigned int recursive)
{
    struct menuOption *mo = _getSelected(this);

    if (mo)
        menu_selectOption(this, _getOptionByIndex(this, 0));

    _drawSelected(this);

    if (recursive && this->selectedOption && this->selectedOption->submenu)
        _activate(this->selectedOption->submenu, recursive);
}
