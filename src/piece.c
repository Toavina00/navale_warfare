#include "piece.h"

void init() {
    
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            BOARD[i][j] = 20;
        }
    }
    
    SHIPS[0].length = 5;
    SHIPS[1].length = 4;
    SHIPS[2].length = 3;
    SHIPS[3].length = 3;
    SHIPS[4].length = 2;

    int c = 0;
    for (int i = 0; i < N_SHIPS; i++) {
        SHIPS[i].id = i;
        SHIPS[i].offset = c;
        for (int j = 0; j < SHIPS[i].length; j++) {
            PARTS[c].id = c+1;
            PARTS[c].ownerId = i;
            c++;
        }
    }

}

int getShip(int row, int col) {
    return PARTS[BOARD[row][col]-1].ownerId;
}

int isOutOfBound(int row, int col) {
    return row < 0 || row >= DIMENSION || col < 0 || col >= DIMENSION;
}

int isFloating(int row, int col) {
    return BOARD[row][col] >= 1 && BOARD[row][col] <= N_PARTS;
}

int isOver() {
    for (int i = 0; i < N_SHIPS; i++) {
        if (SHIPS[i].length != 0) return FALSE;
    }
    return TRUE;
}

int attack(int row, int col) {
    if (isOutOfBound(row, col) ) return -1;
    if (BOARD[row][col] < 0 || !isFloating(row, col)) return 0;

    int shipId = getShip(row, col);
    SHIPS[shipId].length -= 1;
    BOARD[row][col] *= -1;

    if (SHIPS[shipId].length > 0) return 1;
    else return 2; 
}

int place(int shipId, int row, int col, Orientation o) {
    
    int r, c;
    switch (o) {
    case UP:
        r = -1; c = 0;
        break;
    
    case DOWN:
        r = 1; c = 0;
        break;

    case LEFT:
        c = -1; r = 0;
        break;
    
    case RIGHT:
        c = 1; r = 0;
        break;
    }

    for (int i = 0; i < SHIPS[shipId].length; i++) {
        if (isOutOfBound(row + (r*i), col + (c*i))) return 0;
        if (BOARD[row + (r*i)][col + (c*i)] != 20 ) return 0;
    }

    for (int i = 0; i < SHIPS[shipId].length; i++) {
        BOARD[row + (r*i)][col + (c*i)] = PARTS[SHIPS[shipId].offset + i].id;
    }

    return 1;
}

void placeRandom() {
    srand(time(NULL));
    int i = 0, r, c, o;
    while (i < N_SHIPS) {
        r = rand() % DIMENSION;
        c = rand() % DIMENSION;
        o = rand() % 4;
        if (!place(i, r, c, o)) continue;
        i++;
    }
}

void displayBoard(WINDOW* win) {

    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            if (BOARD[i][j] == 20) {
                mvwprintw(win, 1+i, 1+3*j, "   ");
            }
            else if (BOARD[i][j] == -20) {
                mvwprintw(win, 1+i, 1+3*j, " X ");
            }
            else if (BOARD[i][j] >= 1 && BOARD[i][j] <= N_PARTS) {
                wattron(win, A_REVERSE);
                mvwprintw(win, 1+i, 1+3*j, " # ");
                wattroff(win, A_REVERSE);
            }
            else {
                wattron(win, A_REVERSE);
                mvwprintw(win, 1+i, 1+3*j, " X ");
                wattroff(win, A_REVERSE);
            }
        }
    }

    wrefresh(win);
}