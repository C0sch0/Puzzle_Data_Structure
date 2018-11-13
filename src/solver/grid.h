#pragma once
#include <stdbool.h>
// Grid and nodes def

struct cell {
  int position_x;
  int position_y;
  int other_half_x;
  int other_half_y;
  bool direction; // Magnetto para arriba o pa abaho
  int polarity; // 1 (positive), 0 (neutral), -1 neg
};

struct grid {
    int cols;
    int rows;
    struct cell *** data; // DATA, triple pointer para almacenar todo
    struct cell ** cells; // CELLS
    int * top_restrictions;// TOP restriction, POSITIVAS
    int * left_restrictions;// LEFT restriction, POSITIVAS
    int * bottom_restrictions;// BOTTOM restriction, NEGATIVAS
    int * right_restrictions;// RIGHT restriction, NEGATIVAS
};

typedef struct grid Grid;
typedef struct cell Cell;

// funciones
Grid* _newGrid(int rows, int cols);
void _set(Grid* grid, int row, int col, int other_half_row, int other_half_col, bool direction);
bool _total_polarity(Grid* grid, int x, int y, int cell_value);
bool _valid_move(Grid* grid, Cell* cell, int cell_value);
int _polarity(Grid* grid, int x, int y);
bool _finished(Grid* grid);
void _eliminate(Grid* grid, Cell* cell);
void _put(Grid* grid, Cell* cell, int cell_value);
bool _sides(Grid* grid, int x, int y, int cell_value);
void bonus(Grid* grid);
