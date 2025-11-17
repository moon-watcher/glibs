#include "CollisionGrid.h"

void cg_init(pCollisionGrid $, struct CG_DEF *def)
{
    uint16_t width  = def->rect.w - def->rect.x + 1;
    uint16_t height = def->rect.h - def->rect.y + 1;
    uint16_t cap    = def->capacity;
    uint16_t hCells = def->hCells;
    uint16_t vCells = def->vCells;
    uint16_t wh     = (width  + hCells - 1) / hCells;
    uint16_t hv     = (height + vCells - 1) / vCells;

    $->def              = def;
    $->cells            = (struct CG_CELL **)((char *)$ + sizeof(CollisionGrid));
    $->lookupTableCellX = (uint8_t *)((char *)$->cells + vCells * sizeof(struct CG_CELL *));
    $->lookupTableCellY = (uint8_t *)((char *)$->lookupTableCellX + width);

    for (uint16_t i = 0; i < width;  i++) $->lookupTableCellX[i] = i / wh;
    for (uint16_t i = 0; i < height; i++) $->lookupTableCellY[i] = i / hv;

    struct CG_CELL *cellMemory = (struct CG_CELL *)((char *)$->lookupTableCellY + height);

    for (uint16_t y = 0; y < vCells; ++y)
    {
        $->cells[y] = &cellMemory[y * hCells];

        for (uint16_t x = 0; x < hCells; ++x)
        {
            struct CG_CELL *cell = &$->cells[y][x];
            cell->items = (void **)((char *)cellMemory + vCells * hCells * sizeof(struct CG_CELL) + (y * hCells + x) * cap * sizeof(void *));
            cell->size = 0;
            cell->capacity = cap;
        }
    }
}

inline struct CG_CELL *cg_get_CELL(pCollisionGrid $, uint16_t x, uint16_t y)
{
    struct CG_DEF *def = $->def;
    
    int16_t a_left = def->rect.x;
    if (x < a_left)
        return 0;

    int16_t a_top = def->rect.y;
    if (y < a_top)
        return 0;
    
    uint16_t cellX = $->lookupTableCellX[x - a_left];    
    if (cellX >= def->hCells)
        return 0;

    uint16_t cellY = $->lookupTableCellY[y - a_top];
    if (cellY >= def->vCells)
        return 0;
        
    return &$->cells[cellY][cellX];
}

inline struct CG_CELL *cg_addItem_FAST(pCollisionGrid $, uint16_t x, uint16_t y, void *item)
{
    struct CG_DEF *def = $->def;
    uint16_t cellX = $->lookupTableCellX[x - def->rect.x];
    uint16_t cellY = $->lookupTableCellY[y - def->rect.y];
    struct CG_CELL *cell = &$->cells[cellY][cellX];

    cell->items[cell->size++] = item;

    return cell;
}

void cg_reset_CELLs(pCollisionGrid $)
{
    struct CG_DEF *def = $->def;
    uint16_t vCells = def->vCells;
    uint16_t hCells = def->hCells;

    for (uint16_t cellY = 0; cellY < vCells; ++cellY)
        for (uint16_t cellX = 0; cellX < hCells; ++cellX)
            $->cells[cellY][cellX].size = 0;
}

void cg_reset(pCollisionGrid $)
{
    struct CG_DEF *def = $->def;
    uint16_t vCells = def->vCells;
    uint16_t hCells = def->hCells;

    memset($->cells, 0, vCells * hCells * sizeof(struct CG_CELL));
}

//

inline void *cg_CELL_addItem(struct CG_CELL *$, void *item)
{
    if ($->size >= $->capacity)
        return 0;
        
    return $->items[$->size++] = item;
}

uint16_t cg_CELL_removeItem(struct CG_CELL *$, void *item)
{
    uint16_t size = $->size;

    for (uint16_t i = 0; i < size; i++)
        if ($->items[i] == item)
        {
            $->items[i] = $->items[--$->size];
            return 1;
        }

    return 0;
}

//

void cg_RECT_addItem(struct CG_CELL *cell_list[], uint16_t total, void *item)
{
    for (uint16_t i = 0; i < total; i++)
        cg_CELL_addItem(cell_list[i], item);
}

uint16_t cg_RECT_getItems(struct CG_CELL *cell_list[], uint16_t total, void *item_list[])
{
    uint16_t count = 0;

    for (uint16_t i = 0; i < total; i++)
    {
        struct CG_CELL *cell = cell_list[i];
        uint16_t size = cell->size;

        for (uint16_t j = 0; j < size; j++)
            item_list[count++] = cell->items[j];
    }

    return count;
}

void cg_RECT_removeItem(struct CG_CELL *cell_list[], uint16_t total, void *item)
{
    for (uint16_t i = 0; i < total; i++)
        cg_CELL_removeItem(cell_list[i], item);
}

uint16_t cg_getItems_from_RECT(pCollisionGrid $, struct CG_RECT *rect, void *item_list[])
{
    int16_t x = rect->x;
    int16_t w = rect->w;
    if (x >= w)
        return 0;

    int16_t y = rect->y;
    int16_t h = rect->h;
    if (y >= h)
        return 0;

    uint16_t count     = 0;
    struct CG_DEF *def = $->def;
    int16_t $_left     = def->rect.x;
    int16_t $_top      = def->rect.y;
    uint16_t hCells    = def->hCells;
    uint16_t vCells    = def->vCells;
    uint16_t cellX_min = $->lookupTableCellX[x - $_left];
    uint16_t cellY_min = $->lookupTableCellY[y - $_top ];
    uint16_t cellX_max = $->lookupTableCellX[w - $_left];
    uint16_t cellY_max = $->lookupTableCellY[h - $_top ];

    if (cellX_max >= hCells) cellX_max = hCells - 1;
    if (cellY_max >= vCells) cellY_max = vCells - 1;

    for (uint16_t y = cellY_min; y <= cellY_max; ++y)
        for (uint16_t x = cellX_min; x <= cellX_max; ++x)
        {
            struct CG_CELL *cell = &$->cells[y][x];
            uint16_t size = cell->size;
            void **src = cell->items;

            for (uint16_t i = 0; i < size; ++i)
                item_list[count++] = src[i];
        }

    return count;
}

inline uint16_t cg_RECT_collision_XY(struct CG_RECT *rect, uint16_t x, uint16_t y)
{
    return ((uint16_t)(x - rect->x) < rect->w && (uint16_t)(y - rect->y) < rect->h);
}

//
// uint16_t cg_get_RECT(pCollisionGrid $, struct CG_RECT *rect, struct CG_CELL *cell_list[])
// {
//     int16_t x = rect->x;
//     int16_t w = rect->w;
//     if (x >= w) return 0;

//     int16_t y = rect->y;
//     int16_t h = rect->h;
//     if (y >= h) return 0;

//     uint16_t count = 0;
//     int16_t $_left = $->x;
//     int16_t $_top = $->y;
//     uint16_t hCells = $->hCells;
//     uint16_t vCells = $->vCells;
//     uint8_t cellX_min = $->lookupTableCellX[x - $_left];
//     uint8_t cellY_min = $->lookupTableCellY[y - $_top];
//     uint16_t cellX_max = $->lookupTableCellX[w - $_left];
//     uint16_t cellY_max = $->lookupTableCellY[h - $_top];

//     if (cellX_max >= hCells) cellX_max = hCells - 1;
//     if (cellY_max >= vCells) cellY_max = vCells - 1;

//     for (uint8_t y = cellY_min; y <= cellY_max; ++y)
//         for (uint8_t x = cellX_min; x <= cellX_max; ++x)
//             cell_list[count++] = &$->cells[y][x];

//     return count;
// }