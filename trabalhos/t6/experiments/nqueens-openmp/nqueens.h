#ifndef _NQUEENS_H_
#define _NQUEENS_H_

int ok(int queen_number, int row_position, int *position);
int put_queen(int size, int queen_number, int *position);
void nqueens(int size, int *solutions, int qtd_threads);
int find_queens(int size, int qtd_threads);

#endif /* _NQUEENS_H_ */