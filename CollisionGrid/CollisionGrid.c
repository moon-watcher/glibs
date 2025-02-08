#include "CollisionGrid.h"


unsigned cg_size(struct CG_RECT gridArea, unsigned items, unsigned hCells, unsigned vCells)
{
    unsigned width  = gridArea.right  - gridArea.left + 1;
    unsigned height = gridArea.bottom - gridArea.top  + 1;
    return sizeof(CollisionGrid) +
                          vCells * sizeof(struct CG_CELL *) +
                          width * sizeof(unsigned char) +
                          height * sizeof(unsigned char) +
                          vCells * hCells * sizeof(struct CG_CELL) +
                          vCells * hCells * items * sizeof(void *);
}

void cg_init(CollisionGrid * cg, struct CG_RECT gridArea, unsigned items, unsigned hCells, unsigned vCells) 
{
    unsigned width  = gridArea.right  - gridArea.left + 1;
    unsigned height = gridArea.bottom - gridArea.top  + 1;
    // unsigned totalBytes = sizeof(CollisionGrid) +
    //                       vCells * sizeof(struct CG_CELL *) +
    //                       width * sizeof(unsigned char) +
    //                       height * sizeof(unsigned char) +
    //                       vCells * hCells * sizeof(struct CG_CELL) +
    //                       vCells * hCells * items * sizeof(void *);

    // CollisionGrid *cg = malloc(totalBytes);
    // if (!cg) return 0;

    // memset(cg, 0, totalBytes);
    // cg->totalBytes       = totalBytes;
    cg->area             = gridArea;
    cg->hCells           = hCells;
    cg->vCells           = vCells;
    cg->cells            = (struct CG_CELL **)((char *)cg + sizeof(CollisionGrid));
    cg->lookupTableCellX = (unsigned char *)((char *)cg->cells + vCells * sizeof(struct CG_CELL *));
    cg->lookupTableCellY = (unsigned char *)((char *)cg->lookupTableCellX + width * sizeof(unsigned char));

    unsigned wh = (width  + hCells - 1) / hCells;
    unsigned hv = (height + vCells - 1) / vCells;

    for (unsigned i = 0; i < width;  i++) cg->lookupTableCellX[i] = i / wh;
    for (unsigned i = 0; i < height; i++) cg->lookupTableCellY[i] = i / hv;

    struct CG_CELL *cellMemory = (struct CG_CELL *)((char *)cg->lookupTableCellY + height * sizeof(unsigned char));

    for (unsigned i = 0; i < vCells; ++i) {
        cg->cells[i] = &cellMemory[i * hCells];
        for (unsigned j = 0; j < hCells; ++j) {
            cg->cells[i][j].items = (void **)((char *)cellMemory + vCells * hCells * sizeof(struct CG_CELL) + (i * hCells + j) * items * sizeof(void *));
            cg->cells[i][j].capacity = items;
        }
    }
}

void cg_reset(CollisionGrid *const this)
{
    memset(this->cells, 0, this->vCells * this->hCells * sizeof(struct CG_CELL));
}

//

inline struct CG_CELL *cg_CELL_get(CollisionGrid *const this, unsigned x, unsigned y)
{
    int a_left, a_top;

    if ((x < (a_left = this->area.left)) || (y < (a_top  = this->area.top ))) return 0;

    unsigned cellX = this->lookupTableCellX[x - a_left];
    unsigned cellY = this->lookupTableCellY[y - a_top ];
    
    return (cellX < this->hCells && cellY < this->vCells) ? &this->cells[cellY][cellX] : 0;
}

inline void *cg_CELL_addItem(struct CG_CELL *const this, void *const ptr)
{
    return (this->size < this->capacity) ? (this->items[this->size++] = ptr) : 0;
}

unsigned cg_CELL_removeItem(struct CG_CELL *const this, void *const ptr)
{
    unsigned size = this->size;

    for (unsigned i = 0; i < size; i++)
        if (this->items[i] == ptr)
            return this->items[i] = this->items[--this->size];
    
    return 0;
}

//

unsigned cg_RECT_get(CollisionGrid *const this, struct CG_RECT *const rect, struct CG_CELL *cell_list[])
{
    int r_left, r_top, r_right, r_bottom;

    if ((r_left = rect->left) >= (r_right = rect->right) || (r_top = rect->top) >= (r_bottom = rect->bottom)) return 0;

    int a_left         = this->area.left;
    int a_top          = this->area.top;
    unsigned cellX_min = this->lookupTableCellX[r_left   - a_left];
    unsigned cellY_min = this->lookupTableCellY[r_top    - a_top ];
    unsigned cellX_max = this->lookupTableCellX[r_right  - a_left];
    unsigned cellY_max = this->lookupTableCellY[r_bottom - a_top ];

    if (cellX_max >= this->hCells) cellX_max = this->hCells - 1;
    if (cellY_max >= this->vCells) cellY_max = this->vCells - 1;

    unsigned count = 0;

    for (unsigned cellY = cellY_min; cellY <= cellY_max; ++cellY)
        for (unsigned cellX = cellX_min; cellX <= cellX_max; ++cellX)
            cell_list[count++] = &this->cells[cellY][cellX];
    
    return count;
}

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
        
        for(unsigned j = 0; j < size; j++)
            item_list[count++] = cell->items[j];
    }

    return count;
}

void cg_RECT_removeItem(struct CG_CELL *cell_list[], unsigned total, void *const ptr)
{
    for (unsigned i = 0; i < total; i++)
        cg_CELL_removeItem(cell_list[i], ptr);
}
