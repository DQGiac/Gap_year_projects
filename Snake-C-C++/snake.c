#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <unistd.h>

#define UP_ARROW 72
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define ENTER_KEY 13

int n;
int snake[289][2];
int snake_size = 0, best_snake_size = 3;
int hori[20], vert[20];
int curr_arrow_len;
int cherry[2] = {0, 0};

int boxed;
int died = 0;
double time_between_moves = 0.5;
int pastKey;

int delete_tail[2];

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT) x;
    coord.Y = (SHORT) y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int selection(int num, char *options[], int y) {
    int selected = 0;
    int keyPressed;
    int running = 1;

    while (running) {
        gotoxy(0, y);
        for (int i = 0; i < num; i++) {
            if (i == selected) printf("--> %s\n", options[i]);
            else printf("    %s\n", options[i]);
        }
        printf("<ENTER> to choose\n");
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
            gotoxy(0, y);
        } else if (keyPressed == ENTER_KEY) return selected;
    }

    return 0;
}

int first_time_printing = 1;
int print_board() {
    int i, j;
    if (first_time_printing) {
        if (boxed) {
            gotoxy(0, 0);
            for (i = 0; i < n + 2; i ++) printf("#");
            for (i = 1; i < n + 1; i ++) {
                gotoxy(0, i);
                printf("#");
                gotoxy(n + 1, i);
                printf("#");
            }
            gotoxy(0, n + 1);
            for (i = 0; i < n + 2; i ++) printf("#");
        }

        for (i = 1; i < n + 1; i ++) {
            gotoxy(1, i);
            for (j = 0; j < n; j ++) {
                printf(".");
            }
        }
        first_time_printing = 0;
    }

    gotoxy(delete_tail[0] + 1, delete_tail[1] + 1);
    printf(".");
    
    gotoxy(cherry[0] + 1, cherry[1] + 1);
    printf("o");

    for (i = 0; i < snake_size; i ++) {
        gotoxy(snake[i][0] + 1, snake[i][1] + 1);
        printf("@");
    }
    gotoxy(0, n + 2);
    printf("Length: %d    \n", snake_size);
    printf("Last best length: %d    \n", best_snake_size);
}

clock_t start;
int restart() {
    system("cls");
    printf("Choose board size:");
    char *n_options[] = {"9x9", "13x13", "17x17"};
    n = selection(3, n_options, 1) * 4 + 9;
    printf("\nNo or with boundaries:");
    char *boxed_options[] = {"No boundaries", "With boundaries"};
    boxed = selection(2, boxed_options, 7);
    printf("\nSpeed:");
    char *mode_options[] = {"Slow", "Medium", "Fast"};
    time_between_moves = 0.7 - selection(3, mode_options, 12) * 0.2;
    system("cls");
    start = clock();
    died = 0;
    snake[0][0] = n / 2; snake[0][1] = n / 2;
    snake[1][0] = n / 2; snake[1][1] = n / 2 - 1;
    snake[2][0] = n / 2; snake[2][1] = n / 2 - 2;
    cherry[0] = n / 2; cherry[1] = n / 2 + 3;
    snake_size = 3;
    hori[0] = 0; vert[0] = 1; curr_arrow_len = 1;
    first_time_printing = 1;
}

int main() {
    restart();
    int i, j;
    print_board();
    while (1) {
        if (died) {
            best_snake_size = max(best_snake_size, snake_size);
            print_board();
            printf("\nYou died.");
            char *respawn_options[] = {"Restart", "Quit"};
            int a = selection(2, respawn_options, n + 6);
            if (a == 0) restart();
            else return 0;
        }
        if (_kbhit()) {
            int keyPressed = getch();
            if (keyPressed == pastKey) continue;
            if (curr_arrow_len)
            hori[curr_arrow_len] = 0;
            vert[curr_arrow_len] = 0;
            if (keyPressed == LEFT_ARROW && hori[curr_arrow_len - 1] != 1) hori[curr_arrow_len] = -1;
            else if (keyPressed == RIGHT_ARROW && hori[curr_arrow_len - 1] != -1) hori[curr_arrow_len] = 1;
            else if (keyPressed == UP_ARROW && vert[curr_arrow_len - 1] != 1) vert[curr_arrow_len] = -1;
            else if (keyPressed == DOWN_ARROW && vert[curr_arrow_len - 1] != -1) vert[curr_arrow_len] = 1;
            else curr_arrow_len --;
            curr_arrow_len ++;
            pastKey = keyPressed;
        }

        if (((double) (clock() - start) / CLOCKS_PER_SEC) >= time_between_moves) {
            start = clock();
            
            if (curr_arrow_len > 1) {
                for (i = 0; i < curr_arrow_len - 1; i ++) {
                    hori[i] = hori[i + 1];
                    vert[i] = vert[i + 1];
                }
                curr_arrow_len -= 1;
            }

            int ok = -1;
            if (snake[0][0] == cherry[0] && snake[0][1] == cherry[1]) {
                snake[snake_size][0] = snake[snake_size - 1][0];
                snake[snake_size][1] = snake[snake_size - 1][1];
                srand(time(NULL));
                while (1) {
                    cherry[0] = rand() % n;
                    cherry[1] = rand() % n;
                    ok = 1;
                    for (i = 0; i < snake_size; i ++) {
                        if (cherry[0] == snake[i][0] && cherry[1] == snake[i][1]) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) break;
                }
            }

            delete_tail[0] = snake[snake_size - 1][0];
            delete_tail[1] = snake[snake_size - 1][1];

            for (i = snake_size - 1; i > 0; i --) {
                snake[i][0] = snake[i - 1][0];
                snake[i][1] = snake[i - 1][1];
            }
            
            if (ok == 1) snake_size += 1;

            snake[0][0] = snake[0][0] + hori[0];
            snake[0][1] = snake[0][1] + vert[0];
            if (!boxed) {
                snake[0][0] = (snake[0][0] + n) % n;
                snake[0][1] = (snake[0][1] + n) % n;
            }

            if (boxed && (snake[0][0] < 0 || snake[0][0] >= n || snake[0][1] < 0 || snake[0][1] >= n)) died = 1;
            for (i = 0; i < snake_size; i ++)
                for (j = i + 4; j < snake_size; j ++)
                    if (snake[i][0] == snake[j][0] && snake[i][1] == snake[j][1]) died = 1;
            
            print_board();
        }
    }
}
