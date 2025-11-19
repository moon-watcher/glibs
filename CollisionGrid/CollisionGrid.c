#include <genesis.h>
#include "CollisionGrid.h"

void cg_init(pCollisionGrid $, struct CG_DEF *def)
{
    $->left   = def->left;
    $->top    = def->top;
    $->hCells = def->hCells;
    $->vCells = def->vCells;

    uint16_t w  = def->right  - $->left + 1;
    uint16_t h  = def->bottom - $->top  + 1;
    uint16_t wh = (w + $->hCells - 1) / $->hCells;
    uint16_t hv = (h + $->vCells - 1) / $->vCells;

    $->cells            = (struct CG_CELL **)((int8_t *)$ + sizeof(CollisionGrid));
    $->lookupTableCellX = (uint8_t *)((int8_t *)$->cells + $->vCells * sizeof(struct CG_CELL *));
    $->lookupTableCellY = (uint8_t *)((int8_t *)$->lookupTableCellX + w);
    
    struct CG_CELL *cellMemory = (struct CG_CELL *)((int8_t *)$->lookupTableCellY + h);

    while (w--) $->lookupTableCellX[w] = w / wh;
    while (h--) $->lookupTableCellY[h] = h / hv;

    for (uint16_t y = 0; y < $->vCells; ++y)
    {
        $->cells[y] = &cellMemory[y * $->hCells];

        for (uint16_t x = 0; x < $->hCells; ++x)
        {
            struct CG_CELL *cell = &$->cells[y][x];
            cell->items = (void **)((int8_t *)cellMemory + $->vCells * $->hCells * sizeof(struct CG_CELL) + (y * $->hCells + x) * def->capacity * sizeof(void *));
            cell->size = 0;
            cell->capacity = def->capacity;
        }
    }
}

inline struct CG_CELL *cg_cellGet_XY(pCollisionGrid $, uint16_t x, uint16_t y)
{
    return &$->cells[$->lookupTableCellY[y - $->top]][$->lookupTableCellX[x - $->left]];
}

/**
 * @cond rect->left < rect->right && rect->top < rect->bottom
 */
uint16_t cg_itemsGet_RECT(pCollisionGrid $, struct CG_RECT *rect, void *item_list[])
{
    uint16_t count     = 0;
    uint16_t cellX_min = $->lookupTableCellX[rect->left   - $->left];
    uint16_t cellY_min = $->lookupTableCellY[rect->top    - $->top ];
    uint16_t cellX_max = $->lookupTableCellX[rect->right  - $->left];
    uint16_t cellY_max = $->lookupTableCellY[rect->bottom - $->top ];

    if (cellX_max >= $->hCells) cellX_max = $->hCells - 1;
    if (cellY_max >= $->vCells) cellY_max = $->vCells - 1;

    for (uint16_t y = cellY_min; y <= cellY_max; ++y)
        for (uint16_t x = cellX_min; x <= cellX_max; ++x)
        {
            struct CG_CELL *cell = &$->cells[y][x];
            uint16_t size = cell->size;
            void **src = cell->items;

            while (size--)
                item_list[count++] = src[size];
        }

    return count;
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

inline void *cg_CELL_itemAdd(struct CG_CELL *$, void *item)
{
    return ($->size < $->capacity) ? $->items[$->size++] = item : 0;
}

void *cg_CELL_itemRemove(struct CG_CELL *$, void *item)
{
    for (uint16_t i = 0; i < $->size; i++)
        if ($->items[i] == item)
            return $->items[i] = $->items[--$->size];

    return 0;
}

//

inline uint16_t cg_RECT_collision_XY(struct CG_RECT *rect, uint16_t x, uint16_t y)
{
    return (x >= rect->left && x <= rect->right && y >= rect->top && y <= rect->bottom);
}


// /**
//  * @brief Add item to multiple cells
//  * @extends Unused. Is it usefull?
//  */
// void cg_CELLS_itemAdd(struct CG_CELL *list[], uint16_t total, void *item)
// {
//     for (uint16_t i = 0; i < total; i++)
//         cg_CELL_itemAdd(list[i], item);
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
//         cg_CELL_itemRemove(list[i], item);
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
