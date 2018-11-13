#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/** http://theory.stanford.edu/~arbrad/pfe/06/matrix.c **/
/* Creates a ``rows by cols'' matrix with all values 0.
Returns NULL if rows <= 0 or cols <= 0, otherwise a pointer to the new matrix. */
// utilice mucho de: http://www.techiedelight.com/magnet-puzzle/

void bonus(Grid * grid){
  for (int row = 0; row < grid->rows; row++){
    for (int col = 0; col < grid->cols; col++){
      free(grid->data[row][col]);}
    free(grid->data[row]);}
  free(grid->data);
  free(grid->cells);
  free(grid->top_restrictions);
  free(grid->bottom_restrictions);
  free(grid->right_restrictions);
  free(grid->left_restrictions);
  free(grid);
}

void _eliminate(Grid * grid, Cell * cell){
  int x = cell->position_x;
  int y = cell->position_y;
  int x_2 = cell->other_half_x;
  int y_2 = cell->other_half_y;
  int cell_value = _polarity(grid, x, y);
  if (cell_value == 1){
    if (grid->left_restrictions[x] != -2) grid->left_restrictions[x]++;
    if (grid->top_restrictions[y] != -2) grid->top_restrictions[y]++;
    if (grid->right_restrictions[x_2] != -2) grid->right_restrictions[x_2]++;
    if (grid->bottom_restrictions[y_2] != -2) grid->bottom_restrictions[y_2]++;}
  else if (cell_value == -1){
    if (grid->right_restrictions[x] != -2) grid->right_restrictions[x]++;
    if (grid->bottom_restrictions[y] != -2) grid->bottom_restrictions[y]++;
    if (grid->left_restrictions[x_2] != -2) grid->left_restrictions[x_2]++;
    if (grid->top_restrictions[y_2] != -2) grid->top_restrictions[y_2]++;}
  grid->data[x][y]->polarity = 0;
  grid->data[x_2][y_2]->polarity = 0;}

Grid * _newGrid(int rows, int cols) {
  if (rows <= 0 || cols <= 0) return NULL;
  Grid * grid = malloc(sizeof(Grid));
  grid -> rows = rows;
  grid -> cols = cols;
  grid -> data = malloc(sizeof(Cell**)*rows);
  grid -> cells = malloc(sizeof(Cell*)*(rows * cols / 2));
  return grid;
}

int aux = 0;
void _set(Grid* grid, int row, int col, int other_half_row, int other_half_col, bool direction){
  Cell * mag = malloc(sizeof(Cell));
  mag->polarity = 0;
  mag->direction = direction;
  mag->position_x = row;
  mag->position_y = col;
  mag->other_half_x = row + other_half_row;
  mag->other_half_y = col + other_half_col;
  grid->data[row][col] = mag;
  if (direction){
    grid->cells[aux] = mag;
    aux++;
  }
}

int _polarity(Grid * grid, int x, int y){
  return grid->data[x][y]->polarity;
}

bool _sides(Grid * grid, int x, int y, int cell_value){
  if (cell_value != 0){
    if (x + 1 < grid->rows){
      if (_polarity(grid, x + 1, y) == cell_value){
        return false;}}
    if (x - 1 >= 0){
      if (_polarity(grid, x - 1, y) == cell_value){
        return false;}}
    if (y + 1 < grid->cols){
      if (_polarity(grid, x, y + 1) == cell_value){
        return false;}}
    if (y - 1 >= 0){
      if (_polarity(grid, x, y - 1) == cell_value){
        return false;}}
  }
  return true;
}




bool _finished(Grid * grid){
  for (int row = 0; row < grid->rows; row++){
    int neg = grid->right_restrictions[row];
    int pos = grid->left_restrictions[row];
    if (!(neg == -2 || neg == 0)){
      return false;}
    if (!(pos == -2 || pos == 0)){
      return false;}
  }
  for (int col = 0; col < grid->cols; col++){
    int neg = grid->bottom_restrictions[col];
    int pos = grid->top_restrictions[col];
    if (!(neg == -2 || neg == 0)){
      return false;}
    if (!(pos == -2 || pos == 0)){
      return false;}
  }return true;
}

bool _valid_move(Grid * grid, Cell * cell, int cell_value){
  int x_2 = cell->other_half_x;
  int y_2 = cell->other_half_y;
  int x = cell->position_x;
  int y = cell->position_y;
  int cell_value_2 = - cell_value;

  if (!_sides(grid, x, y, cell_value)){return false;}
  if (!_sides(grid, x_2, y_2, cell_value_2)){return false;}
  if (!_total_polarity(grid, x, y, cell_value)){return false;}
  if (!_total_polarity(grid, x_2, y_2, cell_value_2)){return false;}
  return true;
}


bool _total_polarity(Grid * grid, int x, int y, int cell_value){
  if (cell_value == 0){return true;}
  int y_pos = grid->top_restrictions[y];
  int y_neg = grid->bottom_restrictions[y];
  int x_pos = grid->left_restrictions[x];
  int x_neg = grid->right_restrictions[x];

  if ((cell_value == 1) && (x_pos == 0 || y_pos == 0)){
    return false;}
  else if ((cell_value == -1) && (x_neg == 0 || y_neg == 0)){
    return false;}
  return true;
}

void _put(Grid * grid, Cell * cell, int cell_value){
  int x = cell->position_x;
  int y = cell->position_y;
  int x_2 = cell->other_half_x;
  int y_2 = cell->other_half_y;
  grid->data[x][y]->polarity = cell_value;
  if (cell_value == 1){
    if (grid->left_restrictions[x] != -2) grid->left_restrictions[x]--;
    if (grid->top_restrictions[y] != -2) grid->top_restrictions[y]--;}
  else if (cell_value == -1){
    if (grid->right_restrictions[x] != -2) grid->right_restrictions[x]--;
    if (grid->bottom_restrictions[y] != -2) grid->bottom_restrictions[y]--;}
  grid->data[x_2][y_2]->polarity = -cell_value;
  if (cell_value == -1){
    if (grid->left_restrictions[x_2] != -2) grid->left_restrictions[x_2]--;
    if (grid->top_restrictions[y_2] != -2) grid->top_restrictions[y_2]--;}
  else if (cell_value == 1){
    if (grid->right_restrictions[x_2] != -2) grid->right_restrictions[x_2]--;
    if (grid->bottom_restrictions[y_2] != -2) grid->bottom_restrictions[y_2]--;
  }
}
