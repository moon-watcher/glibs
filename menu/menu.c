#include "menu.h"
#include "debug.h"

static void option_draw(struct menuOption *option)
{
    if (option->parent->event.drawOption)
        option->parent->event.drawOption(option->data);
}

static void menu_draw_selected(struct menu *menu)
{
    if (menu->event.drawSelected)
        menu->event.drawSelected(menu->selectedOption->data);
}

//

void menu_init(struct menu *menu, struct menuEvents *events, int16_t round, int16_t single)
{
    menu->event = *events;
    menu->round = round;
    menu->singleOption = single;
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
    option->exec = exec;
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
    if (menu->singleOption)
    {
        // TODO
        // menu_draw_selected(menu);
    }
    else
    {
        struct menuOption *option = menu->head;

        while (option)
        {
            if (option->child)
                menu_draw(option->child);

            if (option == menu->selectedOption)
                menu_draw_selected(menu);
            else
                option_draw(option);

            option = option->next;
        }
    }
}

int16_t menu_update(struct menu *menu)
{
    int16_t ret = 0;
    struct menuOption *option = menu->selectedOption;

    if (!option->child && menu->event.fire && menu->event.fire(option))
    {
        if (option->exec)
            ret = option->exec(option);
    }

    else if (menu->event.inc && menu->event.inc(option))
    {
        if (menu->selectedOption->next)
            menu->selectedOption = menu->selectedOption->next;
        else if (menu->round)
            menu->selectedOption = menu->head;
    }
    else if (menu->event.dec && menu->event.dec(option))
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

        if (menu->event.change)
            ret = menu->event.change(menu->selectedOption);
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
