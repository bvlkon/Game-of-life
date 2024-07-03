
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 80

#define HEIGTH 25

void start_gen(int field[][WIDTH]);

int choose_speed(double *seconds);

void print_gen(const int field[][WIDTH]);

int check_cell(int field[][WIDTH]);

int check_static_end(const int nextgen[][WIDTH], const int currentgen[][WIDTH]);

int main() {
    int field[HEIGTH][WIDTH];

    double seconds = 500 * 1000;

    initscr();

    start_gen(field);

    if (freopen("/dev/tty", "r+", stdin) != NULL) {
        nodelay(stdscr, TRUE);  // rejim bez zaderzhki

        int flag = 1;

        while (flag) {
            if (choose_speed(&seconds) && seconds > 500) {
                print_gen(field);

                flag = check_cell(field);

                usleep(seconds);

            } else

                break;
        }

        print_gen(field);
    }

    endwin();

    return 0;
}

//

void start_gen(int field[][WIDTH]) {
    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            scanf("%1d", &field[i][j]);
        }
    }
}

int choose_speed(double *seconds) {
    noecho();

    int button = getch();

    int flag = 1;

    switch (button) {
        case 'a':

            *seconds *= 2;

            break;

        case 'z':

            *seconds /= 2;

            break;

        case 'q':

            flag = 0;

            break;
    }

    echo();

    refresh();

    return flag;
}

int check_cell(int field[][WIDTH]) {
    int temp_array[HEIGTH][WIDTH];

    int flag = 1;

    for (int i = 0; i < HEIGTH; i++)

        for (int j = 0; j < WIDTH; j++) temp_array[i][j] = 0;  // sozdaem buffer

    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int count = 0, row_up = i - 1, row_down = i + 1, coll_left = j - 1,
                coll_right = j + 1;  // proveryaem index sosednih kletok

            if (i == 0) row_up = HEIGTH - 1;  // logika zamknutosti polya

            if (i == HEIGTH - 1) row_down = 0;

            if (j == 0) coll_left = WIDTH - 1;

            if (j == WIDTH - 1) coll_right = 0;

            count = field[row_up][coll_left] + field[row_up][j] + field[row_up][coll_right] +

                    field[i][coll_left] + field[i][coll_right] + field[row_down][coll_left] +

                    field[row_down][j] + field[row_down][coll_right];

            if (field[i][j] == 1) {
                if (count == 2 || count == 3) {
                    temp_array[i][j] = 1;

                } else

                    temp_array[i][j] = 0;

            } else {
                if (count == 3) {
                    temp_array[i][j] = 1;

                } else

                    temp_array[i][j] = 0;
            }
        }
    }

    flag = !check_static_end(temp_array, field);

    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = temp_array[i][j];
        }
    }

    return flag;
}

void print_gen(const int field[][WIDTH]) {
    clear();

    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 0) {
                printw(" ");

            } else if (field[i][j] == 1) {
                printw("*");
            }
        }

        printw("\n");
    }

    refresh();
}

int check_static_end(const int nextgen[][WIDTH], const int currentgen[][WIDTH])

{
    //проверяем есть ли отличия между предыдущим и текущим поколением

    int same_gens = 1;

    //проверяем все ли клетки мертвы

    int is_dead = 1;

    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (nextgen[i][j] != currentgen[i][j]) {
                same_gens = 0;
            }

            if (nextgen[i][j] == 1) {
                is_dead = 0;
            }
        }
    }

    if (same_gens == 1 || is_dead == 1) {
        return 1;

    } else {
        return 0;
    }
}