#include "../watcher/watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "grid.h"


bool backtrack(Grid * grid, int count)
{
	bool complete = _finished(grid);
	if (complete)
		return true;
	else if (!complete && count == (grid->cols * grid->rows / 2)){
		return false;}
	Cell * puntero = grid->cells[count];
	int array[3] = {1, -1, 0};
	for (int i = 0; i < 3; i++){
		int cell_value = array[i];
		if (_valid_move(grid, puntero, cell_value)){
			_put(grid, puntero, cell_value);
			if (backtrack(grid, count+1))return true;
			_eliminate(grid, puntero);}
	}
	return false;
}
int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Modo de uso: %s test.txt\n", argv[0]);
		return 0;}
	watcher_load_layout(argv[1]);

	FILE* input_file = fopen(argv[1], "r");
	int width;
	int height;
	fscanf(input_file, "%d %d\n", &width, &height);

	Grid* grid;
	grid = _newGrid(height, width);

	grid->top_restrictions = malloc(sizeof(int) * width);
	grid->left_restrictions = malloc(sizeof(int) * height);
	grid->bottom_restrictions = malloc(sizeof(int) * width);
	grid->right_restrictions = malloc(sizeof(int) * height);
	// Leemos restricciones
	for (int side = 0; side < 2; side++){
		for (int col = 0; col < width; col++){
			int val;
			fscanf(input_file, "%d ", &val);
			if (val == 0){
				val = -2;}
			if (side == 0){
				grid->top_restrictions[col] = val;}
			else{
				grid->bottom_restrictions[col] = val;}
		}
	}

	for (int side = 2; side < 4; side++){
		for (int row = 0; row < height; row++){
			int val;
			fscanf(input_file, "%d ", &val);
			if (val == 0){
				val = -2;}
			if (side == 2){
				grid->left_restrictions[row] = val;}
			else{
				grid->right_restrictions[row] = val;}
		}
	}
	// Leemos posicionamiento
	/** Leemos las casillas del tablero para poblarlo */
  	for (int row= 0; row < height; row++){
      grid->data[row] = malloc(sizeof(Cell *) * width);}

    for (int row = 0; row < height; row++){
  		for (int col = 0; col < width; col++){
  			char type;
  			fscanf(input_file, "%c ", &type);
  			if (type == 'L'){
					_set(grid, row, col+1, 0, -1, false);
          _set(grid, row, col, 0, +1, true);
          }
				else if (type == 'T'){
					_set(grid, row + 1, col,  -1, 0, false);
					_set(grid, row, col, +1, 0, true);
					}
  		}
  	}

		fclose(input_file);

	if(!backtrack(grid, 0))
		printf("No pude resolverlo / algo salio mal\n");
	else
		printf("Termine\n");
	for (int i = 0; i < (grid->cols * grid->rows / 2); i++){
		Cell * pointer = grid->cells[i];
		if (pointer->polarity != 0){
			bool polarity = true;
			bool direccion = true;
			if (pointer->position_x == pointer->other_half_x){
				direccion = false;}
			if (pointer->polarity == -1){
				polarity = false;}
			watcher_set_magnet(pointer->position_x, pointer->position_y, direccion, polarity);}
	}
	watcher_close();
	bonus(grid);
	return 0;
}
