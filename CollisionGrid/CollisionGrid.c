#include <genesis.h>
#include "CollisionGrid.h"

void cg_init(pCollisionGrid $, struct CG_DEF *def)
{
    $->left   = def->left;
    $->top    = def->top;
    $->width  = def->width;
    $->height = def->height;
    $->hCells = def->hCells;
    $->vCells = def->vCells;

    uint16_t wh = ($->width  + $->hCells - 1) / $->hCells;
    uint16_t hv = ($->height + $->vCells - 1) / $->vCells;

    $->cells            = (struct CG_CELL **)((uint8_t *)$ + sizeof(CollisionGrid));
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

inline struct CG_CELL *cg_cellGet_XY(pCollisionGrid $, uint16_t x, uint16_t y)
{
    uint16_t offsetX = x - $->left;
    uint16_t offsetY = y - $->top;

    return (offsetX < $->width && offsetY < $->height)
               ? &$->cells[$->lookupTableCellY[offsetY]][$->lookupTableCellX[offsetX]]
               : NULL;
}

uint16_t cg_itemsGet_RECT(pCollisionGrid $, struct CG_RECT *rect, void *item_list[])
{
    uint16_t offL = rect->left - $->left, width = $->width;
    if (offL >= width) return 0;

    uint16_t offT = rect->top - $->top, height = $->height;
    if (offT >= height) return 0;

    uint16_t offR = offL + rect->width - 1;
    if (offR >= width) return 0;

    uint16_t offB = offT + rect->height - 1;
    if (offB >= height) return 0;

    uint8_t *lookupX = $->lookupTableCellX;
    uint8_t *lookupY = $->lookupTableCellY;
    
    uint16_t cx0 = lookupX[offL] + 0;
    uint16_t cy0 = lookupY[offT] + 0;
    uint16_t cx1 = lookupX[offR] + 1;
    uint16_t cy1 = lookupY[offB] + 1;

    struct CG_CELL **row    = $->cells + cy0;
    struct CG_CELL **rowEnd = $->cells + cy1;
    void **out = item_list;

    for (; row < rowEnd; ++row)
    {
        struct CG_CELL *cell = *row + cx0;
        struct CG_CELL *cellEnd = *row + cx1;

        for (; cell < cellEnd; ++cell)
        {
            void **items = cell->items;
            void **end = items + cell->size;
            
            while (end > items)
                *out++ = *--end;
        }
    }

    return out - item_list;
}

void cg_resetCells(pCollisionGrid $)
{
    uint16_t hCells = $->hCells;
    uint16_t vCells = $->vCells;

    for (uint16_t y = 0; y < vCells; ++y)
        for (uint16_t x = 0; x < hCells; ++x)
            $->cells[y][x].size = 0;
}

void cg_reset(pCollisionGrid $)
{
    memset($->cells, 0, $->vCells * $->hCells * sizeof(struct CG_CELL));
}

//

inline uint16_t cg_cell_itemAdd(struct CG_CELL *$, void *item)
{
    if ($->size >= $->capacity)
        return 0;
        
    return $->items[$->size++] = item, 1;
}

inline uint16_t cg_cell_itemRemove(struct CG_CELL *$, void *item)
{
    void **items = $->items;
    void **end = items + $->size;

    while (end > items)
        if (*--end == item)
            return *end = items[--$->size], 1;

    return 0;
}

// /**
//  * @brief Add item to multiple cells
//  * @extends Unused. Is it usefull?
//  */
// void cg_CELLS_itemAdd(struct CG_CELL *list[], uint16_t total, void *item)
// {
//     for (uint16_t i = 0; i < total; i++)
//         cg_cell_itemAdd(list[i], item);
// }

// /**
//  * @brief Get all items form multiples cells
//  * @extends Unused. Is it usefull?
//  */
// uint16_t cg_CELLS_itemsGet(struct CG_CELL *list[], uint16_t total, void *item_list[])
// {
//     uint16_t count = 0;

//     for (uint16_t i = 0; i < total; i++)
//     {
//         struct CG_CELL *cell = list[i];
//         uint16_t size = cell->size;

//         for (uint16_t j = 0; j < size; j++)
//             item_list[count++] = cell->items[j];
//     }

//     return count;
// }

// /**
//  * @brief Remove item form all cells
//  * @extends Unused. Is it usefull?
//  */
// void cg_CELLS_itemRemove(struct CG_CELL *list[], uint16_t total, void *item)
// {
//     for (uint16_t i = 0; i < total; i++)
//         cg_cell_itemRemove(list[i], item);
// }

// uint16_t cg_get_RECT(pCollisionGrid $, struct CG_RECT *rect, struct CG_CELL *list[])
// {
//     int16_t left = rect->left;
//     int16_t right = rect->right;
//     if (left >= right) return 0;

//     int16_t top = rect->top;
//     int16_t bottom = rect->bottom;
//     if (top >= bottom) return 0;

//     uint16_t count = 0;
//     int16_t $_left = $->left;
//     int16_t $_top = $->top;
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
//             list[count++] = &$->cells[y][x];

//     return count;
// }
