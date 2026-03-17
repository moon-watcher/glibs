#include <genesis.h>
#include "CollisionGrid.h"

#define CG_ERROR_INIT(X) \
    X->error = 0

#define CG_ERROR_CTRL(X, ERR, COND) \
    if (COND)                       \
    {                               \
        X->error = ERR;             \
        return 0;                   \
    }

void cg_init(pCollisionGrid $, struct CG_DEF *def)
{
    $->left   = def->left;
    $->top    = def->top;
    $->width  = def->width;
    $->height = def->height;
    $->hCells = def->hCells;
    $->vCells = def->vCells;
    $->error  = 0;

    uint16_t wh = ($->width  + $->hCells - 1) / $->hCells;
    uint16_t hv = ($->height + $->vCells - 1) / $->vCells;

    $->cells = (struct CG_CELL **)((uint8_t *)$ + sizeof(CollisionGrid));
    $->lookupTableCellX = (uint8_t *)($->cells + $->vCells);
    $->lookupTableCellY = $->lookupTableCellX + $->width;

    struct CG_CELL *cellMemory = (struct CG_CELL *)($->lookupTableCellY + $->height);

    for (uint16_t i = 0; i < $->width;  i++) $->lookupTableCellX[i] = i / wh;
    for (uint16_t i = 0; i < $->height; i++) $->lookupTableCellY[i] = i / hv;

    for (uint16_t y = 0; y < $->vCells; ++y)
    {
        $->cells[y] = &cellMemory[y * $->hCells];

        for (uint16_t x = 0; x < $->hCells; ++x)
        {
            struct CG_CELL *cell = &$->cells[y][x];
            cell->items = (void **)(cellMemory + $->vCells * $->hCells) + (y * $->hCells + x) * def->capacity;
            cell->size = 0;
            cell->capacity = def->capacity;
        }
    }
}

inline struct CG_CELL *cg_getCell_XY(pCollisionGrid $, uint16_t x, uint16_t y)
{
    CG_ERROR_INIT($);

    uint16_t offsetX = x - $->left;
    CG_ERROR_CTRL($, 1, offsetX >= $->width);

    uint16_t offsetY = y - $->top;
    CG_ERROR_CTRL($, 2, offsetY >= $->height);

    return &$->cells[$->lookupTableCellY[offsetY]][$->lookupTableCellX[offsetX]];
}

inline struct CG_CELL *cg_addItem_XY(pCollisionGrid $, uint16_t x, uint16_t y, void *item)
{
    CG_ERROR_INIT($);
    CG_ERROR_CTRL($, 3, !item);

    uint16_t offsetX = x - $->left;
    CG_ERROR_CTRL($, 4, offsetX >= $->width);

    uint16_t offsetY = y - $->top;
    CG_ERROR_CTRL($, 5, offsetY >= $->height);

    struct CG_CELL *cell = &$->cells[$->lookupTableCellY[offsetY]][$->lookupTableCellX[offsetX]];
    CG_ERROR_CTRL($, 6, cell->size >= cell->capacity);

    cell->items[cell->size++] = item;

    return cell;
}

uint16_t cg_getItems_RECT(pCollisionGrid $, struct CG_RECT *rect, void *item_list[])
{
    CG_ERROR_INIT($);

    uint16_t offL = rect->left - $->left, width = $->width;
    CG_ERROR_CTRL($, 7, offL >= width);

    uint16_t offR = offL + rect->width - 1;
    CG_ERROR_CTRL($, 8, offR >= width);

    uint16_t offT = rect->top - $->top, height = $->height;
    CG_ERROR_CTRL($, 9, offT >= height);

    uint16_t offB = offT + rect->height - 1;
    CG_ERROR_CTRL($, 10, offB >= height);

    uint16_t cx0 = $->lookupTableCellX[offL] + 0;
    uint16_t cy0 = $->lookupTableCellY[offT] + 0;
    uint16_t cx1 = $->lookupTableCellX[offR] + 1;
    uint16_t cy1 = $->lookupTableCellY[offB] + 1;

    struct CG_CELL **cells = $->cells;
    void **out = item_list;

    for (uint16_t y = cy0; y < cy1; ++y)
    {
        struct CG_CELL *row = cells[y];

        for (uint16_t x = cx0; x < cx1; ++x)
        {
            struct CG_CELL *cell = &row[x];
            void **items = cell->items;
            uint16_t size = cell->size;
            
            items += size;
            while (size--)
                *out++ = *--items;
        }
    }

    return out - item_list;
}

void cg_reset(pCollisionGrid $)
{
    struct CG_CELL *cell = $->cells[0];
    struct CG_CELL *end = cell + $->vCells * $->hCells;

    while (cell < end)
        (cell++)->size = 0;
}

//

inline struct CG_CELL *cg_cell_itemAdd(struct CG_CELL *$, void *item)
{
    CG_ERROR_INIT($);
    CG_ERROR_CTRL($, 11, !item);
    CG_ERROR_CTRL($, 12, $->size >= $->capacity);

    return $->items[$->size++] = item, $;
}

struct CG_CELL *cg_cell_itemRemove(struct CG_CELL *$, void *item)
{
    CG_ERROR_INIT($);
    CG_ERROR_CTRL($, 13, !item);

    void **items = $->items;
    void **end = items + $->size;

    while (end > items)
        if (*--end == item)
            return *end = items[--$->size], $;

    CG_ERROR_CTRL($, 14, 1);
}

//

inline uint16_t cg_rect_collision_xy(struct CG_RECT *rect, int16_t x, int16_t y)
{
    return ((uint16_t)(x - rect->left) < rect->width && (uint16_t)(y - rect->top) < rect->height);
}

// //

// void cg_RECT_addItem(struct CG_CELL *cell_list[], uint16_t total, void *item)
// {
//     for (uint16_t i = 0; i < total; i++)
//         cg_cell_itemAdd(cell_list[i], item);
// }

// uint16_t cg_RECT_getItems(struct CG_CELL *cell_list[], uint16_t total, void *item_list[])
// {
//     uint16_t count = 0;

//     for (uint16_t i = 0; i < total; i++)
//     {
//         struct CG_CELL *cell = cell_list[i];
//         uint16_t size = cell->size;

//         for (uint16_t j = 0; j < size; j++)
//             item_list[count++] = cell->items[j];
//     }

//     return count;
// }

// void cg_RECT_removeItem(struct CG_CELL *cell_list[], uint16_t total, void *item)
// {
//     for (uint16_t i = 0; i < total; i++)
//         cg_cell_itemRemove(cell_list[i], item);
// }

// uint16_t cg_get_RECT(pCollisionGrid $, struct CG_RECT *rect, struct CG_CELL *cell_list[])
// {
//     int left = rect->left;
//     int right = rect->right;
//     if (left >= right) return 0;

//     int top = rect->top;
//     int bottom = rect->bottom;
//     if (top >= bottom) return 0;

//     uint16_t count = 0;
//     int $_left = $->left;
//     int $_top = $->top;
//     uint16_t hCells = $->hCells;
//     uint16_t vCells = $->vCells;
//     uint8_t cellX_min = $->lookupTableCellX[left - $_left];
//     uint8_t cellY_min = $->lookupTableCellY[top - $_top];
//     uint16_t cellX_max = $->lookupTableCellX[right - $_left];
//     uint16_t cellY_max = $->lookupTableCellY[bottom - $_top];

//     if (cellX_max >= hCells) cellX_max = hCells - 1;
//     if (cellY_max >= vCells) cellY_max = vCells - 1;

//     for (uint8_t y = cellY_min; y <= cellY_max; ++y)
//         for (uint8_t x = cellX_min; x <= cellX_max; ++x)
//             cell_list[count++] = &$->cells[y][x];

//     return count;
// }