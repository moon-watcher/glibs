#include "menu.h"
#include "debug.h"

static void drawOption(struct menu *$, struct menuOption *mo)
{
    $->drawOption_f && mo->data && $->drawOption_f(mo->data);
}

static void drawSelected(struct menu *$)
{
    $->drawSelected_f && $->selectedOption->data && $->drawSelected_f($->selectedOption->data);
}

static void incOption(struct menu *$)
{
    if ($->selectedOption->next)
        $->selectedOption = $->selectedOption->next;

    else if ($->round)
        $->selectedOption = $->head;
}

static void decOption(struct menu *$)
{
    if ($->selectedOption->prev)
        $->selectedOption = $->selectedOption->prev;

    else if ($->round)
        $->selectedOption = $->tail;
}

//

void menu_init(struct menu *$, menuOption_f inc, menuOption_f dec, menuOption_f fire, int16_t (*selected_f)(), int16_t (*draw_f)())
{
    $->drawOption_f = draw_f;
    $->drawSelected_f = selected_f;
    $->round = 0;
    $->singleOption = 0;
    $->incOption_f = inc;
    $->decOption_f = dec;
    $->fireOption_f = fire;
    $->head = 0;
    $->tail = 0;
    $->selectedOption = 0;
}

void menu_add(struct menu *$, struct menuOption *mo, void *data, menuOption_f exec)
{
    mo->menu = $;
    mo->data = data;
    mo->submenu = 0;
    mo->next = 0;
    mo->prev = 0;
    mo->exec_f = exec;

    if (!$->head)
    {
        $->head = mo;
        $->selectedOption = mo;
    }
    else
    {
        mo->prev = $->tail;
        $->tail->next = mo;
    }

    $->tail = mo;
}

void menu_draw(struct menu *$)
{
    if (!$)
        return;

    if (!$->singleOption)
    {
        struct menuOption *mo = $->head;

        while (mo)
        {
            drawOption($, mo);
            menu_draw(mo->submenu);

            mo = mo->next;
        }
    }

    drawSelected($);
}

int16_t menu_update(struct menu *$)
{
    int16_t ret = 0;
    struct menuOption *mo = $->selectedOption;

    if (!mo->submenu && $->fireOption_f && $->fireOption_f(mo))
    {
        if (mo->exec_f)
            ret = mo->exec_f(mo);
    }

    else if ($->incOption_f && $->incOption_f(mo))
        incOption($);

    else if ($->decOption_f && $->decOption_f(mo))
        decOption($);

    if ($->selectedOption != mo)
    {
        drawOption($, mo);
        drawSelected($);
    }

    if ($->selectedOption->submenu)
        menu_update($->selectedOption->submenu);

    return ret;
}

void menu_option_submenu(struct menuOption *$, struct menu *submenu)
{
    $->submenu = submenu;
}

void menu_option_select(struct menuOption *$)
{
    $->menu->selectedOption = $;
}

// static void _deactivate(struct menu *$, unsigned int recursive)
// {
//     if ($->selectedOption)
//         drawOption($, $->selectedOption);

//     if (recursive && $->selectedOption && $->selectedOption->submenu)
//         _deactivate($->selectedOption->submenu, recursive);
// }

// static void _activate(struct menu *$, unsigned int recursive)
// {
//     if ($->selectedOption)
//         menu_option_select($, $->head);

//     drawSelected($);

//     if (recursive && $->selectedOption && $->selectedOption->submenu)
//         _activate($->selectedOption->submenu, recursive);
// }
