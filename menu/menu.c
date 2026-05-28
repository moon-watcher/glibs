#include "menu.h"
#include "debug.h"

static void _drawOption(struct menu *this, struct menuOption *mo)
{
    this->drawOption_f && mo->data && this->drawOption_f(mo->data);
}

static void _drawSelected(struct menu *this)
{
    this->drawSelected_f && this->selectedOption->data && this->drawSelected_f(this->selectedOption->data);
}

static void _incOption(struct menu *this)
{
    if (this->selectedOption->next)
        this->selectedOption = this->selectedOption->next;

    else if (this->round)
        this->selectedOption = this->head;
}

static void _decOption(struct menu *this)
{
    if (this->selectedOption->prev)
        this->selectedOption = this->selectedOption->prev;

    else if (this->round)
        this->selectedOption = this->tail;
}

//

void menu_init(struct menu *this, menuOption_handler_f inc_f, menuOption_handler_f dec_f, menuOption_handler_f fire_f, int (*selected_f)(), int (*draw_f)())
{
    this->drawOption_f = draw_f;
    this->drawSelected_f = selected_f;
    this->round = 0;
    this->singleOption = 0;
    this->incOption_f = inc_f;
    this->decOption_f = dec_f;
    this->fireOption_f = fire_f;
    this->head = 0;
    this->tail = 0;
    this->selectedOption = 0;
}

void menu_addOption(struct menu *this, struct menuOption *mo, void *data, struct menu *submenu, menuOption_handler_f exec_f)
{
    mo->data = data;
    mo->submenu = submenu;
    mo->next = 0;
    mo->prev = 0;
    mo->exec_f = exec_f;

    if (!this->head)
    {
        this->head = mo;
        this->selectedOption = mo;
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
    if (!this)
        return;

    if (!this->singleOption)
    {
        struct menuOption *mo = this->head;

        while (mo)
        {
            _drawOption(this, mo);
            menu_draw(mo->submenu);

            mo = mo->next;
        }
    }

    _drawSelected(this);
}

void menu_selectOption(struct menu *this, struct menuOption *mo)
{
    this->selectedOption = mo;
}

int menu_update(struct menu *this)
{
    struct menuOption *mo = this->selectedOption;

    if (this->fireOption_f && this->fireOption_f(mo))
        mo->exec_f && mo->exec_f(mo);

    else if (this->incOption_f && this->incOption_f(mo))
        _incOption(this);

    else if (this->decOption_f && this->decOption_f(mo))
        _decOption(this);

    if (this->selectedOption != mo)
    {
        _drawOption(this, mo);
        _drawSelected(this);
    }

    if (this->selectedOption->submenu)
        menu_update(this->selectedOption->submenu);

    return 1;
}

// static void _deactivate(struct menu *this, unsigned int recursive)
// {
//     if (this->selectedOption)
//         _drawOption(this, this->selectedOption);

//     if (recursive && this->selectedOption && this->selectedOption->submenu)
//         _deactivate(this->selectedOption->submenu, recursive);
// }

// static void _activate(struct menu *this, unsigned int recursive)
// {
//     if (this->selectedOption)
//         menu_selectOption(this, this->head);

//     _drawSelected(this);

//     if (recursive && this->selectedOption && this->selectedOption->submenu)
//         _activate(this->selectedOption->submenu, recursive);
// }
