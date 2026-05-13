#include <stdio.h>
#include <math.h>
#include <Windows.h>

int n = 0;
double matrix[100][100];
int cell_width = 15;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT) x;
    coord.Y = (SHORT) y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Input_matrix(double target[][100], int m, int n) {
	int i, j, k;
    for (i = 0; i < n; i ++) {
        for (j = 0; j < m; j ++) {
            gotoxy(j * cell_width, i * 2 + 1);
            printf("+");
            for (k = 0; k < cell_width - 1; k ++) printf("-");
            gotoxy(j * cell_width, i * 2 + 2);
            printf("|");
        }
        gotoxy(m * cell_width, i * 2 + 1);
        printf("+");
        gotoxy(m * cell_width, i * 2 + 2);
        printf("|");
    }

    for (i = 0; i < m; i ++) {
        gotoxy(i * cell_width, n * 2 + 1);
        printf("+");
        for (j = 0; j < cell_width - 1; j ++) printf("-");
    }
    printf("+");

	double value;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j ++) {
            gotoxy(j * cell_width + 2, i * 2 + 2);
			scanf("%lf", &value);
			target[i][j] = value;
		}
	}
    gotoxy(0, n * 2 + 3);
}

double Laplace(double target[][100], int n) {
	if (n == 1) return target[0][0];
	int i, j, k;
	double result = 0;
	for (i = 0; i < n; i++) {
		double A[100][100];
		for (k = 0; k < n; k ++) {
			A[0][k] = target[0][k + (i > k ? -1 : 0)];
		}
		result += pow(-1, i) * target[0][i] * Laplace(A, n - 1);
	}
	return result;
}

int main() {
	printf("Size of matrix - ");
    while (n <= 0 || n > 100) {
        printf("Please input a number less than or equal to 100: ");
        scanf("%d", &n);
    }
    system("cls");
	printf("Input matrix of size %dx%d:\n", n, n);
	Input_matrix(matrix, n, n);
	
    double det = Laplace(matrix, n);
	printf("Determinant of the matrix is: %.3lf", det);

	return 0;
}
