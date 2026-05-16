// Used more advanced data structures in this matrix calculator.
// TODO: add matrix transposition and inverse of matrix

#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <conio.h>
#include <unistd.h>

#define UP_ARROW 72
#define DOWN_ARROW 80
#define ENTER_KEY 13

int cell_width = 15;

struct Matrix {
    int height;
    int width;
    double **matrix;
};

typedef struct Matrix* MatrixPtr;

void gotoxy(int x, int y);
int selection(int num, char *options[], int x, int y);
double** createMatrix(int height, int width);
MatrixPtr inputMatrix();
void outputMatrix(MatrixPtr A);
double Laplace(MatrixPtr target);
MatrixPtr scalarMultiplication(MatrixPtr target, double scalar);
MatrixPtr matrixAddition(MatrixPtr A, MatrixPtr B);
MatrixPtr matrixMultiplication(MatrixPtr A, MatrixPtr B);

int main() {
	MatrixPtr A = inputMatrix();
	
    while (1) {
        system("cls");
        outputMatrix(A);

        gotoxy(0, A->height * 2 + 3);
        printf("What do you want to do next:");
        char *options[] = {"Calculate determinant", "Scalar multiplication", "Multiply/Addition with another matrix", "Quit"};
        int selected = selection(4, options, 0, A->height * 2 + 4);

        outputMatrix(A);

        if (selected == 0) {
            if (A->height != A->width) {
                printf("Not possible. Matrix needs to be square.");
            } else {
                double det = Laplace(A);
                printf("Determinant of the matrix is: %.3lf\n", det);
            }
            char *no_options[] = {};
            selection(0, options, 0, A->height * 2 + 5);
        } else if (selected == 1) {
            double scalar;
            printf("Input the scalar: ");
            scanf("%lf", &scalar);
            outputMatrix(scalarMultiplication(A, scalar));
            char *no_options[] = {};
            selection(0, options, 0, A->height * 2 + 5);
        } else if (selected == 2) {
            MatrixPtr B = inputMatrix();

            printf("Choose calculation: ");
            char *options[] = {"Addition", "Multiplication"};
            int a = selection(2, options, 0, B->height * 2 + 2);
            system("cls");
            if (a == 0) {
                B = matrixAddition(A, B);
                if (B == NULL) {
                    printf("Condition not met.\n");
                    continue;
                }
                else outputMatrix(B);
            } else {
                MatrixPtr C = matrixMultiplication(A, B);
                if (C == NULL) {
                    printf("Condition not met.\n");
                    continue;
                }
                else outputMatrix(C);
            }

            char *no_options[] = {};
            selection(0, options, 0, A->height * 2 + 5);
        } else {
            break;
        }
    }
	return 0;
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT) x;
    coord.Y = (SHORT) y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int selection(int num, char *options[], int x, int y) {
    int selected = 0;
    int keyPressed;
    int running = 1;

    while (running) {
        gotoxy(x, y);
        for (int i = 0; i < num; i++) {
            gotoxy(x, y + i);
            if (i == selected) printf("--> %s", options[i]);
            else printf("    %s", options[i]);
        }
        gotoxy(x, y + num);
        printf("An <ENTER> de tiep tuc.\n");
        keyPressed = getch();
        if (keyPressed == 0 || keyPressed == 224) {
            keyPressed = getch();
            switch (keyPressed) {
                case UP_ARROW:
                    selected = (selected - 1 + num) % num;
                    break;
                case DOWN_ARROW:
                    selected = (selected + 1) % num;
                    break;
            }
            gotoxy(x, y);
        } else if (keyPressed == ENTER_KEY) return selected;
    }

    return 0;
}

double** createMatrix(int height, int width) {
    double **arr = (double **) malloc(height * sizeof(double*));
    for (int i = 0; i < height; i ++) {
        arr[i] = (double *) malloc(width * sizeof(double));
    }
    return arr;
}

MatrixPtr inputMatrix() {
    system("cls");
    int height = 101, width = 101;
    printf("Size of matrix\n");
    while (height <= 0 || height > 100) {
        printf("Height: Please input a number less than or equal to 100: ");
        scanf("%d", &height);
    }
    while (width <= 0 || width > 100) {
        printf("Width: Please input a number less than or equal to 100: ");
        scanf("%d", &width);
    }

    system("cls");
	printf("Input matrix of size %dx%d:\n", height, width);

	int i, j, k;
    for (i = 0; i < width; i ++) {
        for (j = 0; j < height; j ++) {
            gotoxy(j * cell_width, i * 2 + 1);
            printf("+");
            for (k = 0; k < cell_width - 1; k ++) printf("-");
            gotoxy(j * cell_width, i * 2 + 2);
            printf("|");
        }
        gotoxy(height * cell_width, i * 2 + 1);
        printf("+");
        gotoxy(height * cell_width, i * 2 + 2);
        printf("|");
    }

    for (i = 0; i < height; i ++) {
        gotoxy(i * cell_width, width * 2 + 1);
        printf("+");
        for (j = 0; j < cell_width - 1; j ++) printf("-");
    }
    printf("+");

	double value;
    double** matrix = createMatrix(height, width);
	for (j = 0; j < width; j ++) {
		for (i = 0; i < height; i++) {
            gotoxy(i * cell_width + 2, j * 2 + 2);
			scanf("%lf", &matrix[i][j]);
		}
	}
    gotoxy(0, height * 2 + 3);

    MatrixPtr result = malloc(sizeof(struct Matrix));
    result->height = height;
    result->width = width;
    result->matrix = matrix;
    return result;
}

void outputMatrix(MatrixPtr A) {
    system("cls");
    printf("Matrix");
    int height = A->height, width = A->width;
    int i, j, k;
    for (i = 0; i < width; i ++) {
        for (j = 0; j < height; j ++) {
            gotoxy(j * cell_width, i * 2 + 1);
            printf("+");
            for (k = 0; k < cell_width - 1; k ++) printf("-");
            gotoxy(j * cell_width, i * 2 + 2);
            printf("|");
        }
        gotoxy(height * cell_width, i * 2 + 1);
        printf("+");
        gotoxy(height * cell_width, i * 2 + 2);
        printf("|");
    }

    for (i = 0; i < height; i ++) {
        gotoxy(i * cell_width, width * 2 + 1);
        printf("+");
        for (j = 0; j < cell_width - 1; j ++) printf("-");
    }
    printf("+");

    for (i = 0; i < height; i++) {
		for (j = 0; j < width; j ++) {
            gotoxy(i * cell_width + 2, j * 2 + 2);
			printf("%.3f", A->matrix[i][j]);
		}
	}
    gotoxy(0, height * 2 + 3);
}

double Laplace(MatrixPtr matrix) {
    int n = matrix->height;
    if (n == 1) return matrix->matrix[0][0];
	double result = 0;
    MatrixPtr submatrix = malloc(sizeof(struct Matrix));
    for (int ind = 0; ind < n; ind++) {
        double **A = createMatrix(n - 1, n - 1);
		for (int i = 1; i < n; i ++) {
            for (int j = 0; j < n; j ++) {
                A[i - 1][j - (j > ind ? 1 : 0)] = matrix->matrix[i][j];
            }
        }
        submatrix->height = n - 1;
        submatrix->width = n - 1;
        submatrix->matrix = A;
		result += pow(-1, ind) * matrix->matrix[0][ind] * Laplace(submatrix);
        free(A);
	}
	return result;
}

MatrixPtr scalarMultiplication(MatrixPtr matrix, double scalar) {
    printf("hehe");
    for (int i = 0; i < matrix->height; i ++) {
        for (int j = 0; j < matrix->width; j ++) {
            matrix->matrix[i][j] *= scalar;
        }
    }
    return matrix;
}

MatrixPtr matrixAddition(MatrixPtr A, MatrixPtr B) {
    if (A->width != B->width) return NULL;
    if (A->height != B->height) return NULL;
    double** C = createMatrix(A->height, A->width);
    C = A->matrix;

    for (int i = 0; i < A->height; i ++) {
        for (int j = 0; j < A->width; j ++) {
            C[i][j] += B->matrix[i][j];
        }
    }

    A->matrix = C;
    return A;
}

MatrixPtr matrixMultiplication(MatrixPtr A, MatrixPtr B) {
    double** C = createMatrix(A->height, B->width);
    if (A->width != B->height) return NULL;

    double** matrix1 = A->matrix;
    double** matrix2 = B->matrix;

	int i, j, k;
	for (i = 0; i < A->width; i++) {
		for (j = 0; j < A->width; j++) {
			C[i][j] = 0;
			for (k = 0; k < A->height; k++) {
				C[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
    
    MatrixPtr result = malloc(sizeof(struct Matrix));
    result->height = A->height;
    result->width = B->width; 
    result->matrix = C;
    return result;
}

MatrixPtr matrixTranspose(MatrixPtr matrix) {
    MatrixPtr result = malloc(sizeof(struct Matrix));
    result->height = matrix->width;
    result->width = matrix->height;
    double** resultingMatrix = createMatrix(matrix->width, matrix->height);
    for (int i = 0; i < matrix->width; i ++) {
        for (int j = 0; j < matrix->height; j ++) {
            resultingMatrix[i][j] = matrix->matrix[j][i];
        }
    }
    return matrix;
}
