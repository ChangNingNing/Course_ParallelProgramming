#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int rowColConflict(int try, int row, int col, int sudoku[9][9]){
	for(int i=0; i<9; i++){
		if(sudoku[row][i] == try || sudoku[i][col] == try)
			return 1;
	}
	return 0;
}

int blockConflict(int try, int row, int col, int sudoku[9][9]){
	int blockRow = row / 3, blockCol = col / 3;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			if(sudoku[3 * blockRow + i][3 * blockCol + j] == try)
				return 1;
	return 0;
}

int conflict(int try, int row, int col, int sudoku[9][9]){
	return rowColConflict(try, row, col, sudoku) ||
			blockConflict(try, row, col, sudoku);
}

int placeNumber(int n, int sudoku[9][9]){
	if(n == 81) return 1;
	int row = n / 9, col = n % 9;
	if(sudoku[row][col] != 0) return placeNumber(n + 1, sudoku);
	int numSolution = 0;
	for(int i=1; i<=9; i++){
		if(!conflict(i, row, col, sudoku)){
			sudoku[row][col] = i;
			numSolution += placeNumber(n + 1, sudoku);
		}
	}
	sudoku[row][col] = 0;
	return numSolution;
}

int main(void){
	int sudoku[9][9];
	int firstZero = -1;
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			scanf("%d", &sudoku[i][j]);
			if(sudoku[i][j] == 0 && firstZero == -1)
				firstZero = i * 9 + j;
		}
	}
#ifdef _OPENMP
	omp_set_num_threads(4);
#endif
	int numSolution = 0;
#pragma omp parallel for reduction(+ : numSolution) firstprivate(sudoku)
	for(int i=1; i<=9; i++){
		if(!conflict(i, firstZero/9, firstZero%9, sudoku)){
			sudoku[firstZero/9][firstZero%9] = i;
			numSolution += placeNumber(firstZero, sudoku);
		}
	}
	printf("# of solution = %d\n", numSolution);
	return 0;
}
