#include "menu.h"

void menu_init(struct menu *menu, struct menuEvents *events, int16_t round, int16_t single)
{
    *menu = (struct menu){.event = *events, .round = round, .singleOption = single};
}

void menu_add(struct menu *menu, struct menuOption *option, void *data, menuOption_f exec)
{
    *option = (struct menuOption){.data = data, .exec = exec, .index = menu->tail ? menu->tail->index + 1 : 0};

    if (!menu->head)
        menu->head = menu->selectedOption = option;
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
        menu_draw_option(menu, menu->selectedOption);
        return;
    }

    struct menuOption *option = menu->head;
    while (option)
    {
        if (option->child)
            menu_draw(option->child);

        if (option == menu->selectedOption)
            menu_draw_selected(menu);
        else
            menu_draw_option(menu, option);

        option = option->next;
    }
}

void menu_draw_selected(struct menu *menu)
{
    if (menu->event.drawSelected)
        menu->event.drawSelected(menu->selectedOption->data);
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
        menu_draw_option(menu, option);
        menu_draw_selected(menu);

        if (option->child && option->child->singleOption)
            menu_draw_option(menu, option->child->selectedOption);

        if (menu->selectedOption->child)
            menu_draw_selected(menu->selectedOption->child);

        if (menu->event.change)
            ret = menu->event.change(menu->selectedOption);
    }

    if (menu->selectedOption->child)
        ret = menu_update(menu->selectedOption->child);

    return ret;
}

void menu_draw_option(struct menu *menu, struct menuOption *option)
{
    if (menu->event.drawOption)
        menu->event.drawOption(option->data);
}

void menu_option_submenu(struct menuOption *option, struct menu *submenu)
{
    option->child = submenu;
}

void menu_select_option(struct menu *menu, struct menuOption *option)
{
    menu->selectedOption = option;
}
