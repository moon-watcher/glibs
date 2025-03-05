#include "CollisionGrid.h"

void cg_init(CollisionGrid *const this, struct CG_DEF *const def)
{
    unsigned width  = def->right  - def->left + 1;
    unsigned height = def->bottom - def->top  + 1;

    this->capacity = def->capacity;
    this->hCells   = def->hCells;
    this->vCells   = def->vCells;
    this->left     = def->left;
    this->top      = def->top;
    this->right    = def->right;
    this->bottom   = def->bottom;

    this->cells = (struct CG_CELL **)((char *)this + sizeof(CollisionGrid));
    this->lookupTableCellX = (unsigned char *)((char *)this->cells + def->vCells * sizeof(struct CG_CELL *));
    this->lookupTableCellY = (unsigned char *)((char *)this->lookupTableCellX + width * sizeof(unsigned char));

    unsigned wh = (width  + def->hCells - 1) / def->hCells;
    unsigned hv = (height + def->vCells - 1) / def->vCells;

    for (unsigned i = 0; i < width;  i++) this->lookupTableCellX[i] = i / wh;
    for (unsigned i = 0; i < height; i++) this->lookupTableCellY[i] = i / hv;

    struct CG_CELL *cellMemory = (struct CG_CELL *)((char *)this->lookupTableCellY + height * sizeof(unsigned char));

    for (unsigned i = 0; i < def->vCells; ++i)
    {
        this->cells[i] = &cellMemory[i * def->hCells];

        for (unsigned j = 0; j < def->hCells; ++j)
        {
            this->cells[i][j].items = (void **)((char *)cellMemory + def->vCells * def->hCells * sizeof(struct CG_CELL) + (i * def->hCells + j) * def->capacity * sizeof(void *));
            this->cells[i][j].capacity = def->capacity;
        }
    }
}

inline struct CG_CELL *cg_get_CELL(CollisionGrid *const this, unsigned x, unsigned y)
{
    int a_left, a_top;

    if ((x < (a_left = this->left)) || (y < (a_top = this->top))) return 0;

    unsigned cellX = this->lookupTableCellX[x - a_left];
    unsigned cellY = this->lookupTableCellY[y - a_top ];

    return (cellX < this->hCells && cellY < this->vCells) ? &this->cells[cellY][cellX] : 0;
}

// unsigned cg_get_RECT(CollisionGrid *const this, struct CG_RECT *const rect, struct CG_CELL *cell_list[])
// {
//     int const left = rect->left;
//     int const right = rect->right;
//     if (left >= right) return 0;

//     int const top = rect->top;
//     int const bottom = rect->bottom;
//     if (top >= bottom) return 0;

//     unsigned count = 0;
//     int const this_left = this->left;
//     int const this_top = this->top;
//     unsigned const hCells = this->hCells;
//     unsigned const vCells = this->vCells;
//     unsigned char const cellX_min = this->lookupTableCellX[left - this_left];
//     unsigned char const cellY_min = this->lookupTableCellY[top - this_top];
//     unsigned cellX_max = this->lookupTableCellX[right - this_left];
//     unsigned cellY_max = this->lookupTableCellY[bottom - this_top];

//     if (cellX_max >= hCells) cellX_max = hCells - 1;
//     if (cellY_max >= vCells) cellY_max = vCells - 1;

//     for (unsigned char y = cellY_min; y <= cellY_max; ++y)
//         for (unsigned char x = cellX_min; x <= cellX_max; ++x)
//             cell_list[count++] = &this->cells[y][x];

//     return count;
// }

inline struct CG_CELL *cg_addItem_FAST(CollisionGrid *const this, unsigned x, unsigned y, void *const ptr)
{
    unsigned const cellX = this->lookupTableCellX[x - this->left];
    unsigned const cellY = this->lookupTableCellY[y - this->top];
    struct CG_CELL *cell = &this->cells[cellY][cellX];

    cell->items[cell->size++] = ptr;

    return cell;
}

void cg_reset_CELLs(CollisionGrid *const this)
{
    for (unsigned cellY = 0; cellY < this->vCells; ++cellY)
        for (unsigned cellX = 0; cellX < this->hCells; ++cellX)
            this->cells[cellY][cellX].size = 0;
}

void cg_reset(CollisionGrid *const this)
{
    memset(this->cells, 0, this->vCells * this->hCells * sizeof(struct CG_CELL));
}

//

inline void *cg_CELL_addItem(struct CG_CELL *const this, void *const ptr)
{
    return (this->size < this->capacity) ? (this->items[this->size++] = ptr) : 0;
}

unsigned cg_CELL_removeItem(struct CG_CELL *const this, void *const ptr)
{
    unsigned size = this->size;

    for (unsigned i = 0; i < size; i++)
        if (this->items[i] == ptr)
        {
            this->items[i] = this->items[--this->size];
            return 1;
        }

    return 0;
}

//

void cg_RECT_addItem(struct CG_CELL *cell_list[], unsigned total, void *const ptr)
{
    for (unsigned i = 0; i < total; i++)
        cg_CELL_addItem(cell_list[i], ptr);
}

unsigned cg_RECT_getItems(struct CG_CELL *cell_list[], unsigned total, void *item_list[])
{
    unsigned count = 0;

    for (unsigned i = 0; i < total; i++)
    {
        struct CG_CELL *const cell = cell_list[i];
        unsigned size = cell->size;

        for (unsigned j = 0; j < size; j++)
            item_list[count++] = cell->items[j];
    }

    return count;
}

void cg_RECT_removeItem(struct CG_CELL *cell_list[], unsigned total, void *const ptr)
{
    for (unsigned i = 0; i < total; i++)
        cg_CELL_removeItem(cell_list[i], ptr);
}

unsigned cg_getItems_from_RECT(CollisionGrid *const this, struct CG_RECT *const rect, void *item_list[])
{
    int const left = rect->left;
    int const right = rect->right;
    if (left >= right) return 0;

    int const top = rect->top;
    int const bottom = rect->bottom;
    if (top >= bottom) return 0;

    unsigned count = 0;
    unsigned const sofv = sizeof(void *);
    int const this_left = this->left;
    int const this_top = this->top;
    unsigned const hCells = this->hCells;
    unsigned const vCells = this->vCells;
    unsigned char const cellX_min = this->lookupTableCellX[left - this_left];
    unsigned char const cellY_min = this->lookupTableCellY[top - this_top];
    unsigned cellX_max = this->lookupTableCellX[right - this_left];
    unsigned cellY_max = this->lookupTableCellY[bottom - this_top];

    if (cellX_max >= hCells) cellX_max = hCells - 1;
    if (cellY_max >= vCells) cellY_max = vCells - 1;

    for (unsigned char y = cellY_min; y <= cellY_max; ++y)
        for (unsigned char x = cellX_min; x <= cellX_max; ++x)
        {
            struct CG_CELL *const cell = &this->cells[y][x];
            unsigned size = cell->size;
            memcpy(&item_list[count], cell->items, size * sofv);
            count += size;
        }

    return count;
}

inline unsigned cg_RECT_collision_XY(struct CG_RECT *const rect, unsigned x, unsigned y)
{
    return (x >= rect->left && x <= rect->right && y >= rect->top && y <= rect->bottom);
}
