/*
 * Original code from the Cilk project (by Keith Randall)
 * 
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 */

#include <stdlib.h>
#include <stdio.h>

#include "nqueens.h"

/* Checking information */
 static int solutions[] = {
        1,
        0,
        0,
        2,
        10, /* 5 */
        4,
        40,
        92,
        352,
        724, /* 10 */
        2680,
        14200,
        73712,
        365596,
        2279184, /* 15 */
        14772512,
        95815104,
        666090624
};
#define MAX_SOLUTIONS sizeof(solutions)/sizeof(int) 

int total_count;

int ok(int queen_number, int row_position, int *position) {
	// Check each queen before this one
	for (int i = 0; i < queen_number; i++) {
		// Get another queen's row_position
		int other_row_pos = position[i];
		// Now check if they're in the same row or diagonals
		if ( other_row_pos == row_position || // Same row
			 other_row_pos == row_position - (queen_number - i) || // Same diagonal
			 other_row_pos == row_position + (queen_number - i) )  // Same diagonal
			return 0;
	}
	return 1;
}

int put_queen(int size, int queen_number, int *position) {
	for (int i = position[queen_number] + 1; i < size; i++) {
		if ( ok(queen_number, i, position) ) {
			position[queen_number] = i;
			return 1;
		}
	}
	position[queen_number] = -1;
	return 0;
}

void nqueens(int size, int *solutions, int qtd_threads) {
	int i, count = 0;

	#pragma omp parallel private(i) num_threads(qtd_threads)
	{
		int *position = (int*) malloc(sizeof(int) * size);

		// #pragma omp for schedule(dynamic) reduction(+:count)
		#pragma omp for schedule(static, size/qtd_threads) reduction(+:count)
		for (i = 0; i < size; i++) {
			int j;
			position[0] = i;
		
			for (j = 1; j < size; j++)
				position[j] = -1;
		
			int queen_number = 1;
			while (queen_number > 0) {
				if ( put_queen(size, queen_number, position) ) {
					queen_number++;
					if (queen_number == size) {
						count += 1;
						position[queen_number-1] = -1;
						queen_number -= 2;
					}
				} else {
					queen_number--;
				}
			}
		}
	}
	
	*solutions = count;
}

int find_queens(int size, int qtd_threads) {
	total_count = 0;
	nqueens(size, &total_count, qtd_threads);
	return total_count;
}
