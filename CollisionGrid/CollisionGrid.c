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

    for (uint16_t i = 0; i < $->width; i++)  $->lookupTableCellX[i] = i / wh;
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
    uint16_t offsetX = (uint16_t)(x - $->left);
    uint16_t offsetY = (uint16_t)(y - $->top);
    
    return (offsetX < $->width && offsetY < $->height) 
        ? &$->cells[$->lookupTableCellY[offsetY]][$->lookupTableCellX[offsetX]]
        : NULL;
}

uint16_t cg_itemsGet_RECT(pCollisionGrid $, struct CG_RECT *rect, void *item_list[])
{
    uint16_t $_width = $->width;
    uint16_t offL = rect->left - $->left;
    if (offL >= $_width) return 0;

    uint16_t $_height = $->height;
    uint16_t offT = rect->top - $->top;
    if (offT >= $_height) return 0;

    uint16_t offR = offL + rect->width - 1;
    if (offR >= $_width) return 0;

    uint16_t offB = offT + rect->height - 1;
    if (offB >= $_height) return 0;

    uint16_t cx0 = $->lookupTableCellX[offL];
    uint16_t cy0 = $->lookupTableCellY[offT];
    uint16_t cx1 = $->lookupTableCellX[offR];
    uint16_t cy1 = $->lookupTableCellY[offB];

    uint16_t count = 0;

    struct CG_CELL **rowPtr = $->cells + (cy0 + 0);
    struct CG_CELL **rowEnd = $->cells + (cy1 + 1);

    for ( ; rowPtr < rowEnd; ++rowPtr)
    {
        struct CG_CELL *row = *rowPtr;
        struct CG_CELL *cell = row + cx0;
        struct CG_CELL *cellEnd = row + (cx1 + 1);

        for ( ; cell < cellEnd; ++cell)
        {
            void **it = cell->items;
            void **itEnd = it + cell->size;

            for ( ; it < itEnd; ++it)
                item_list[count++] = *it;
        }
    }

    return count;



    // uint16_t $_width = $->width;
    // uint16_t offL = rect->left - $->left;
    // if (offL >= $_width) return 0;

    // uint16_t $_height = $->height;
    // uint16_t offT = rect->top - $->top;
    // if (offT >= $_height) return 0;

    // uint16_t offR = offL + rect->width - 1;
    // if (offR >= $_width) return 0;

    // uint16_t offB = offT + rect->height - 1;
    // if (offB >= $_height) return 0;

    // uint16_t cx0 = $->lookupTableCellX[offL];
    // uint16_t cy0 = $->lookupTableCellY[offT];
    // uint16_t cx1 = $->lookupTableCellX[offR];
    // uint16_t cy1 = $->lookupTableCellY[offB];

    // uint16_t count = 0;

    // struct CG_CELL **rows = $->cells;

    // for (uint16_t y = cy0; y <= cy1; ++y)
    // {
    //     struct CG_CELL *row = rows[y];

    //     for (uint16_t x = cx0; x <= cx1; ++x)
    //     {
    //         struct CG_CELL *cell = &row[x];

    //         uint16_t s = cell->size;
    //         void **it = cell->items;

    //         while (s--)
    //             out[count++] = it[s];
    //     }
    // }

    // return count;




    // uint16_t left         = rect->left;
    // uint16_t $_left       = $->left;
    // uint16_t $_width      = $->width;
    // uint16_t offsetLeft   = (uint16_t)(left - $_left);
    // if (offsetLeft   >= $_width ) return 0;

    // uint16_t top          = rect->top;
    // uint16_t $_top        = $->top;
    // uint16_t $_height     = $->height;
    // uint16_t offsetTop    = (uint16_t)(top - $_top);
    // if (offsetTop    >= $_height) return 0;
    
    // uint16_t width        = rect->width;
    // uint16_t offsetRight  = (uint16_t)(left + width - 1 - $_left);
    // if (offsetRight  >= $_width ) return 0;

    // uint16_t height       = rect->height;
    // uint16_t offsetBottom = (uint16_t)(top + height - 1 - $_top);
    // if (offsetBottom >= $_height) return 0;
    
    // uint16_t cellX_min = $->lookupTableCellX[offsetLeft  ];
    // uint16_t cellY_min = $->lookupTableCellY[offsetTop   ];
    // uint16_t cellX_max = $->lookupTableCellX[offsetRight ];
    // uint16_t cellY_max = $->lookupTableCellY[offsetBottom];
    
    // uint16_t count = 0;
    // struct CG_CELL **cells = $->cells;

    // for (uint16_t y = cellY_min; y <= cellY_max; ++y)
    // {
    //     struct CG_CELL *row = cells[y];
        
    //     for (uint16_t x = cellX_min; x <= cellX_max; ++x)
    //     {
    //         struct CG_CELL *cell = &row[x];
    //         void **items = cell->items;
    //         uint16_t size = cell->size;
            
    //         while (size--)
    //             item_list[count++] = items[size];
    //     }
    // }

    // return count;



    // uint16_t offsetLeft   = (uint16_t)(rect->left - $->left);
    // uint16_t offsetTop    = (uint16_t)(rect->top - $->top);
    // uint16_t offsetRight  = (uint16_t)(rect->left + rect->width - 1 - $->left);
    // uint16_t offsetBottom = (uint16_t)(rect->top + rect->height - 1 - $->top);
    
    // if (offsetLeft >= $->width || offsetTop >= $->height || offsetRight >= $->width || offsetBottom >= $->height)
    //     return 0;
    
    // uint16_t cellX_min = $->lookupTableCellX[offsetLeft];
    // uint16_t cellY_min = $->lookupTableCellY[offsetTop];
    // uint16_t cellX_max = $->lookupTableCellX[offsetRight];
    // uint16_t cellY_max = $->lookupTableCellY[offsetBottom];
    
    // uint16_t count = 0;
    // struct CG_CELL **cells = $->cells;  // Cache pointer

    // for (uint16_t y = cellY_min; y <= cellY_max; ++y)
    // {
    //     struct CG_CELL *row = cells[y];  // Cache row
        
    //     for (uint16_t x = cellX_min; x <= cellX_max; ++x)
    //     {
    //         struct CG_CELL *cell = &row[x];
    //         void **items = cell->items;
    //         uint16_t size = cell->size;
            
    //         while (size--)
    //             item_list[count++] = items[size];
    //     }
    // }

    // return count;
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
    uint16_t offsetX = (uint16_t)(x - rect->left);
    uint16_t offsetY = (uint16_t)(y - rect->top);
    
    return (offsetX < rect->width && offsetY < rect->height);
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
