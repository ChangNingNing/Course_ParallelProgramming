#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAXN 4096
#define SIDE (MAXN + 2)
#define nLive(A, i, j) A[i][j-1]+A[i][j+1]+A[i-1][j-1]+A[i-1][j]+A[i-1][j+1]+A[i+1][j-1]+A[i+1][j]+A[i+1][j+1]

char T[2][SIDE][SIDE] = {0};

int main(){
	int generation = 200, n = MAXN;
	for(int i=1; i<=n; i++)
		for(int j=1; j<=n; j++)
			T[0][i][j] = rand() % 2;
	for(int g=1; g<=generation; g++){
		#pragma omp parallel for
		for(int i=1; i<=n; i++){
			for(int j=1; j<=n; j++){
				int pre = (g-1) % 2, cur = g % 2;
				int nln = nLive(T[pre], i ,j);
				T[cur][i][j] = (T[pre][i][j]==0 && nln==3) || (T[pre][i][j]==1 && (nln==2 || nln==3));
			}
		}
	}
	return 0;
}
