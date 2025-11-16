#include "CollisionGrid.h"

void cg_init(pCollisionGrid $, struct CG_DEF *def)
{
    unsigned width  = def->right  - def->x + 1;
    unsigned height = def->bottom - def->y + 1;
    unsigned cap    = def->capacity;
    unsigned hCells = def->hCells;
    unsigned vCells = def->vCells;
    unsigned wh     = (width  + hCells - 1) / hCells;
    unsigned hv     = (height + vCells - 1) / vCells;

    $->def              = def;
    $->cells            = (struct CG_CELL **)((char *)$ + sizeof(CollisionGrid));
    $->lookupTableCellX = (unsigned char *)((char *)$->cells + vCells * sizeof(struct CG_CELL *));
    $->lookupTableCellY = (unsigned char *)((char *)$->lookupTableCellX + width);

    for (unsigned i = 0; i < width;  i++) $->lookupTableCellX[i] = i / wh;
    for (unsigned i = 0; i < height; i++) $->lookupTableCellY[i] = i / hv;

    struct CG_CELL *cellMemory = (struct CG_CELL *)((char *)$->lookupTableCellY + height);

    for (unsigned y = 0; y < vCells; ++y)
    {
        $->cells[y] = &cellMemory[y * hCells];

        for (unsigned x = 0; x < hCells; ++x)
        {
            struct CG_CELL *cell = &$->cells[y][x];
            cell->items = (void **)((char *)cellMemory + vCells * hCells * sizeof(struct CG_CELL) + (y * hCells + x) * cap * sizeof(void *));
            cell->size = 0;
            cell->capacity = cap;
        }
    }
}

inline struct CG_CELL *cg_get_CELL(pCollisionGrid $, unsigned x, unsigned y)
{
    struct CG_DEF *def = $->def;
    
    int a_left = def->x;
    if (x < a_left)
        return 0;

    int a_top = def->y;
    if (y < a_top)
        return 0;
    
    unsigned char cellX = $->lookupTableCellX[x - a_left];    
    if (cellX >= def->hCells)
        return 0;

    unsigned char cellY = $->lookupTableCellY[y - a_top];
    if (cellY >= def->vCells)
        return 0;
        
    return &$->cells[cellY][cellX];
}

inline struct CG_CELL *cg_addItem_FAST(pCollisionGrid $, unsigned x, unsigned y, void *item)
{
    struct CG_DEF *def = $->def;
    unsigned char cellX = $->lookupTableCellX[x - def->x];
    unsigned char cellY = $->lookupTableCellY[y - def->y];
    struct CG_CELL *cell = &$->cells[cellY][cellX];

    cell->items[cell->size++] = item;

    return cell;
}

void cg_reset_CELLs(pCollisionGrid $)
{
    struct CG_DEF *def = $->def;
    unsigned vCells = def->vCells;
    unsigned hCells = def->hCells;

    for (unsigned cellY = 0; cellY < vCells; ++cellY)
        for (unsigned cellX = 0; cellX < hCells; ++cellX)
            $->cells[cellY][cellX].size = 0;
}

void cg_reset(pCollisionGrid $)
{
    struct CG_DEF *def = $->def;
    unsigned vCells = def->vCells;
    unsigned hCells = def->hCells;

    memset($->cells, 0, vCells * hCells * sizeof(struct CG_CELL));
}

//

inline void *cg_CELL_addItem(struct CG_CELL *$, void *item)
{
    if ($->size >= $->capacity)
        return 0;
        
    return $->items[$->size++] = item;
}

unsigned cg_CELL_removeItem(struct CG_CELL *$, void *item)
{
    unsigned char size = $->size;

    for (unsigned i = 0; i < size; i++)
        if ($->items[i] == item)
        {
            $->items[i] = $->items[--$->size];
            return 1;
        }

    return 0;
}

//

void cg_RECT_addItem(struct CG_CELL *cell_list[], unsigned total, void *item)
{
    for (unsigned i = 0; i < total; i++)
        cg_CELL_addItem(cell_list[i], item);
}

unsigned cg_RECT_getItems(struct CG_CELL *cell_list[], unsigned total, void *item_list[])
{
    unsigned count = 0;

    for (unsigned i = 0; i < total; i++)
    {
        struct CG_CELL *cell = cell_list[i];
        unsigned char size = cell->size;

        for (unsigned j = 0; j < size; j++)
            item_list[count++] = cell->items[j];
    }

    return count;
}

void cg_RECT_removeItem(struct CG_CELL *cell_list[], unsigned total, void *item)
{
    for (unsigned i = 0; i < total; i++)
        cg_CELL_removeItem(cell_list[i], item);
}

unsigned cg_getItems_from_RECT(pCollisionGrid $, struct CG_RECT *rect, void *item_list[])
{
    int left  = rect->x;
    int right = rect->right;
    if (left >= right)
        return 0;

    int top    = rect->y;
    int bottom = rect->bottom;
    if (top >= bottom)
        return 0;

    unsigned count           = 0;
    struct CG_DEF *def = $->def;
    int $_left      = def->x;
    int $_top       = def->y;
    unsigned hCells    = def->hCells;
    unsigned vCells    = def->vCells;
    unsigned cellX_min = $->lookupTableCellX[left   - $_left];
    unsigned cellY_min = $->lookupTableCellY[top    - $_top];
    unsigned cellX_max       = $->lookupTableCellX[right  - $_left];
    unsigned cellY_max       = $->lookupTableCellY[bottom - $_top];

    if (cellX_max >= hCells) cellX_max = hCells - 1;
    if (cellY_max >= vCells) cellY_max = vCells - 1;

    for (unsigned y = cellY_min; y <= cellY_max; ++y)
        for (unsigned x = cellX_min; x <= cellX_max; ++x)
        {
            struct CG_CELL *cell = &$->cells[y][x];
            unsigned size = cell->size;
            void **src = cell->items;

            for (unsigned i = 0; i < size; ++i)
                item_list[count++] = src[i];
        }

    return count;
}

inline unsigned cg_RECT_collision_XY(struct CG_RECT *rect, unsigned x, unsigned y)
{
    return (x >= rect->x && x <= rect->right && y >= rect->y && y <= rect->bottom);
}

//
// unsigned cg_get_RECT(pCollisionGrid $, struct CG_RECT *rect, struct CG_CELL *cell_list[])
// {
//     int left = rect->x;
//     int right = rect->right;
//     if (left >= right) return 0;

//     int top = rect->y;
//     int bottom = rect->bottom;
//     if (top >= bottom) return 0;

//     unsigned count = 0;
//     int $_left = $->x;
//     int $_top = $->y;
//     unsigned hCells = $->hCells;
//     unsigned vCells = $->vCells;
//     unsigned char cellX_min = $->lookupTableCellX[left - $_left];
//     unsigned char cellY_min = $->lookupTableCellY[top - $_top];
//     unsigned cellX_max = $->lookupTableCellX[right - $_left];
//     unsigned cellY_max = $->lookupTableCellY[bottom - $_top];

//     if (cellX_max >= hCells) cellX_max = hCells - 1;
//     if (cellY_max >= vCells) cellY_max = vCells - 1;

//     for (unsigned char y = cellY_min; y <= cellY_max; ++y)
//         for (unsigned char x = cellX_min; x <= cellX_max; ++x)
//             cell_list[count++] = &$->cells[y][x];

//     return count;
// }