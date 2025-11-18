#include "CollisionGrid.h"

void cg_init(pCollisionGrid $, struct CG_DEF *def)
{
    uint16_t width  = def->rect.w;
    uint16_t height = def->rect.h;
    uint16_t cap    = def->capacity;
    uint16_t hCells = def->hCells;
    uint16_t vCells = def->vCells;
    uint16_t wh     = (width  + hCells - 1) / hCells;
    uint16_t hv     = (height + vCells - 1) / vCells;

    $->rect             = def->rect;
    $->capacity         = def->capacity;
    $->hCells           = def->hCells;
    $->vCells           = def->vCells;
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
    if (x < $->rect.x || y < $->rect.y)
        return 0;
    
    uint16_t offsetX = x - $->rect.x;
    if (offsetX >= $->rect.w)
        return 0;

    uint16_t offsetY = y - $->rect.y;
    if (offsetY >= $->rect.h)
        return 0;
    
    uint8_t cellX = $->lookupTableCellX[offsetX];
    if (cellX >= $->hCells)
        return 0;

    uint8_t cellY = $->lookupTableCellY[offsetY];
    if (cellY >= $->vCells)
        return 0;
        
    return &$->cells[cellY][cellX];
}

inline struct CG_CELL *cg_addItem_FAST(pCollisionGrid $, uint16_t x, uint16_t y, void *item)
{
    uint8_t cellX = $->lookupTableCellX[$->rect.x];
    uint8_t cellY = $->lookupTableCellY[$->rect.y];
    struct CG_CELL *cell = &$->cells[cellY][cellX];

    cell->items[cell->size++] = item;

}

void cg_reset_CELLs(pCollisionGrid $)
{
    uint16_t vCells = $->vCells;
    uint16_t hCells = $->hCells;

    for (uint16_t cellY = 0; cellY < vCells; ++cellY)
        for (uint16_t cellX = 0; cellX < hCells; ++cellX)
            $->cells[cellY][cellX].size = 0;
}

void cg_reset(pCollisionGrid $)
{
    uint16_t vCells = $->vCells;
    uint16_t hCells = $->hCells;

    memset($->cells, 0, vCells * hCells * sizeof(struct CG_CELL));
}

//

inline void *cg_CELL_addItem(struct CG_CELL *$, void *item)
{
    if ($->size >= $->capacity)
        return 0;
        
    return $->items[$->size++] = item;
}

void *cg_CELL_removeItem(struct CG_CELL *$, void *item)
{
    uint16_t size = $->size;

    for (uint16_t i = 0; i < size; i++)
        if ($->items[i] == item)
            return $->items[i] = $->items[--$->size];

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
    if (rect->w == 0 || rect->h == 0)
        return 0;

    uint16_t x1 = rect->x;
    uint16_t y1 = rect->y;
    uint16_t x2 = rect->x + rect->w;
    uint16_t y2 = rect->y + rect->h;

    // Verificar si el rectángulo está completamente fuera del grid
    uint16_t gridX2 = $->rect.x + $->rect.w;
    uint16_t gridY2 = $->rect.y + $->rect.h;
    
    if (x2 <= $->rect.x || x1 >= gridX2 || y2 <= $->rect.y || y1 >= gridY2)
        return 0;
    
    // Clampear a los límites del grid
    if (x1 < $->rect.x) x1 = $->rect.x;
    if (y1 < $->rect.y) y1 = $->rect.y;
    if (x2 > gridX2) x2 = gridX2;
    if (y2 > gridY2) y2 = gridY2;
    
    // Calcular offsets
    uint16_t offsetX1 = x1 - $->rect.x;
    uint16_t offsetY1 = y1 - $->rect.y;
    uint16_t offsetX2 = x2 - 1 - $->rect.x;
    uint16_t offsetY2 = y2 - 1 - $->rect.y;
    
    // Obtener celdas
    uint16_t cellX_min = $->lookupTableCellX[offsetX1];
    uint16_t cellY_min = $->lookupTableCellY[offsetY1];
    uint16_t cellX_max = $->lookupTableCellX[offsetX2];
    uint16_t cellY_max = $->lookupTableCellY[offsetY2];

    if (cellX_max >= $->hCells) cellX_max = $->hCells - 1;
    if (cellY_max >= $->vCells) cellY_max = $->vCells - 1;

    uint16_t count = 0;
    
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
    int16_t x2 = rect->x + rect->w;
    int16_t y2 = rect->y + rect->h;
    
    return (x >= rect->x && x < x2 && y >= rect->y && y < y2);
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