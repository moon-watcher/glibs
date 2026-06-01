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

//

void menu_init(struct menu *menu, menuOption_f inc_f, menuOption_f dec_f, menuOption_f fire_f, int16_t (*selected_f)(), int16_t (*draw_f)())
{
    menu->drawOption_f = draw_f;
    menu->drawSelected_f = selected_f;
    menu->round = 0;
    menu->singleOption = 0;
    menu->incOption_f = inc_f;
    menu->decOption_f = dec_f;
    menu->fireOption_f = fire_f;
    menu->changeOption_f = 0;
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
    int16_t ret = 0;
    struct menuOption *option = menu->selectedOption;

    if (!option->child && menu->fireOption_f && menu->fireOption_f(option))
    {
        if (option->exec_f)
            ret = option->exec_f(option);
    }

    else if (menu->incOption_f && menu->incOption_f(option))
    {
        if (menu->selectedOption->next)
            menu->selectedOption = menu->selectedOption->next;
        else if (menu->round)
            menu->selectedOption = menu->head;
    }
    else if (menu->decOption_f && menu->decOption_f(option))
    {
        if (menu->selectedOption->prev)
            menu->selectedOption = menu->selectedOption->prev;
        else if (menu->round)
            menu->selectedOption = menu->tail;
    }

    if (menu->selectedOption != option)
    {
        option_draw(option);
        if (option->child && option->child->singleOption)
            option_draw(option->child->selectedOption);

        menu_draw_selected(menu);
        if (menu->selectedOption->child)
            menu_draw_selected(menu->selectedOption->child);

        if (menu->changeOption_f)
            ret = menu->changeOption_f(menu->selectedOption);
    }

    if (menu->selectedOption->child)
        ret = menu_update(menu->selectedOption->child);

    return ret;
}

void menu_option_submenu(struct menuOption *option, struct menu *submenu)
{
    option->child = submenu;
}

void menu_option_select(struct menuOption *option)
{
    option->parent->selectedOption = option;
}
