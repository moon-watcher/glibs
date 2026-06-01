#include "menu.h"
#include "debug.h"

static void option_draw(struct menuOption *option)
{
    if (option->parent->drawOption_f)
        option->parent->drawOption_f(option->data);
}

static void menu_draw_selected(struct menu *menu)
{
    if (menu->drawSelected_f)
        menu->drawSelected_f(menu->selectedOption->data);
}

static void menu_increase_option(struct menu *menu)
{
    if (menu->selectedOption->next)
        menu->selectedOption = menu->selectedOption->next;

    else if (menu->round)
        menu->selectedOption = menu->head;
}

static void menu_decrease_option(struct menu *menu)
{
    if (menu->selectedOption->prev)
        menu->selectedOption = menu->selectedOption->prev;

    else if (menu->round)
        menu->selectedOption = menu->tail;
}

//

void menu_init(struct menu *menu, menuOption_f inc, menuOption_f dec, menuOption_f fire, int16_t (*selected_f)(), int16_t (*draw_f)())
{
    menu->drawOption_f = draw_f;
    menu->drawSelected_f = selected_f;
    menu->round = 0;
    menu->singleOption = 0;
    menu->incOption_f = inc;
    menu->decOption_f = dec;
    menu->fireOption_f = fire;
    menu->head = 0;
    menu->tail = 0;
    menu->selectedOption = 0;
}

void menu_add(struct menu *menu, struct menuOption *option, void *data, menuOption_f exec)
{
    option->parent = menu;
    option->data = data;
    option->child = 0;
    option->next = 0;
    option->prev = 0;
    option->exec_f = exec;
    option->index = menu->tail ? menu->tail->index + 1 : 0;

    if (!menu->head)
    {
        menu->head = option;
        menu->selectedOption = option;
    }
    else
    {
        option->prev = menu->tail;
        menu->tail->next = option;
    }

    menu->tail = option;
}

void menu_draw(struct menu *menu)
{
    if (!menu)
        return;

    if (!menu->singleOption)
    {
        struct menuOption *option = menu->head;

        while (option)
        {
            option_draw(option);
            menu_draw(option->child);

            option = option->next;
        }
    }

    menu_draw_selected(menu);
}

int16_t menu_update(struct menu *menu)
{
    int16_t recursive(struct menu *menu, uint16_t level)
    {
        int16_t ret = 0;
        struct menuOption *option = menu->selectedOption;

        if (!option->child && menu->fireOption_f && menu->fireOption_f(option))
        {
            if (option->exec_f)
                ret = option->exec_f(option->index, option->data);
        }

        else if (menu->incOption_f && menu->incOption_f(option))
            menu_increase_option(menu);

        else if (menu->decOption_f && menu->decOption_f(option))
            menu_decrease_option(menu);

        if (menu->selectedOption != option)
        {
            option_draw(option);
            menu_draw_selected(menu);
        }

        if (menu->selectedOption->child)
            ret = recursive(menu->selectedOption->child, level+1);

        return ret;
    };

    return recursive(menu, 0);
}

void menu_option_submenu(struct menuOption *option, struct menu *submenu)
{
    option->child = submenu;
}

void menu_option_select(struct menuOption *option)
{
    option->parent->selectedOption = option;
}

// static void _deactivate(struct menu *menu, unsigned int recursive)
// {
//     if (menu->selectedOption)
//         option_draw(menu, menu->selectedOption);

//     if (recursive && menu->selectedOption && menu->selectedOption->child)
//         _deactivate(menu->selectedOption->child, recursive);
// }

// static void _activate(struct menu *menu, unsigned int recursive)
// {
//     if (menu->selectedOption)
//         menu_option_select(menu, menu->head);

//     menu_draw_selected(menu);

//     if (recursive && menu->selectedOption && menu->selectedOption->child)
//         _activate(menu->selectedOption->child, recursive);
// }
