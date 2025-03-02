#include "CollisionGrid.h"

void cg_init(CollisionGrid *const this, struct CG_DEF *const def)
{
    unsigned width  = def->right  - def->left + 1;
    unsigned height = def->bottom - def->top  + 1;

    this->def              = def;
    this->cells            = (struct CG_CELL **)((char *)this + sizeof(CollisionGrid));
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
    struct CG_DEF *const def = this->def;
    unsigned const a_left = def->left;
    unsigned const a_top  = def->top;

    if (x < a_left || y < a_top)
        return 0;

    unsigned const cellX = this->lookupTableCellX[x - a_left];
    unsigned const cellY = this->lookupTableCellY[y - a_top ];

    return (cellX < def->hCells && cellY < def->vCells) ? &this->cells[cellY][cellX] : 0;
}

unsigned cg_get_RECT(CollisionGrid *const this, struct CG_RECT *const rect, struct CG_CELL *cell_list[])
{
    unsigned const r_left   = rect->left;
    unsigned const r_top    = rect->top;
    unsigned const r_right  = rect->right;
    unsigned const r_bottom = rect->bottom;

    if (r_left >= r_right || r_top >= r_bottom)
        return 0;

    struct CG_DEF *const def = this->def;
    unsigned const a_left = def->left;
    unsigned const a_top  = def->top;

    unsigned cellX_min = this->lookupTableCellX[r_left   - a_left];
    unsigned cellY_min = this->lookupTableCellY[r_top    - a_top ];
    unsigned cellX_max = this->lookupTableCellX[r_right  - a_left];
    unsigned cellY_max = this->lookupTableCellY[r_bottom - a_top ];

    if (cellX_max >= def->hCells) cellX_max = def->hCells - 1;
    if (cellY_max >= def->vCells) cellY_max = def->vCells - 1;

    unsigned count = 0;

    for (unsigned cellY = cellY_min; cellY <= cellY_max; ++cellY)
        for (unsigned cellX = cellX_min; cellX <= cellX_max; ++cellX)
            cell_list[count++] = &this->cells[cellY][cellX];

    return count;
}

inline struct CG_CELL *cg_addItem_FAST(CollisionGrid *const this, unsigned x, unsigned y, void *const ptr)
{
    struct CG_DEF *const def = this->def;
    unsigned const cellX = this->lookupTableCellX[x - def->left];
    unsigned const cellY = this->lookupTableCellY[y - def->top ];
    struct CG_CELL *cell = &this->cells[cellY][cellX];
    
    cell->items[cell->size++] = ptr;

    return cell;
}

void cg_reset_CELLs(CollisionGrid *const this)
{
    struct CG_DEF *const def = this->def;
    unsigned const vCells = def->vCells;
    unsigned const hCells = def->hCells;

    for (unsigned cellY = 0; cellY < vCells; ++cellY)
        for (unsigned cellX = 0; cellX < hCells; ++cellX)
            this->cells[cellY][cellX].size = 0;
}

void cg_reset(CollisionGrid *const this)
{
    struct CG_DEF *const def = this->def;
    memset(this->cells, 0, def->vCells * def->hCells * sizeof(struct CG_CELL));
}

//

inline void *cg_CELL_addItem(struct CG_CELL *const this, void *const ptr)
{
    return (this->size < this->capacity) ? (this->items[this->size++] = ptr) : 0;
}

unsigned cg_CELL_removeItem(struct CG_CELL *const this, void *const ptr)
{
    unsigned const size = this->size;

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
        unsigned const size = cell->size;

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
