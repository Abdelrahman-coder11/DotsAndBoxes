#include "board.h"

#include <stdio.h>

static LineMask boxMask[NUM_BOXES];
static int lineBoxes[NUM_LINES][2];
static int tablesReady = 0;

int hline_index(int r, int c) { return r * COLS + c; }
int vline_index(int r, int c) { return NUM_H + r * (COLS + 1) + c; }

static void init_tables(void) {
    if (tablesReady) return;
    for (int l = 0; l < NUM_LINES; l++) lineBoxes[l][0] = lineBoxes[l][1] = -1;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int b = r * COLS + c;
            int ls[4] = {hline_index(r, c), hline_index(r + 1, c),
                         vline_index(r, c), vline_index(r, c + 1)};
            boxMask[b] = 0;
            for (int i = 0; i < 4; i++) {
                boxMask[b] |= ((LineMask)1) << ls[i];
                if (lineBoxes[ls[i]][0] < 0) lineBoxes[ls[i]][0] = b;
                else lineBoxes[ls[i]][1] = b;
            }
        }
    }
    tablesReady = 1;
}

void board_init(Board *b) {
    init_tables();
    b->lines = 0;
    b->score[0] = b->score[1] = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) b->owner[r][c] = ' ';
}

int board_line_from_dots(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 > ROWS || r2 < 0 || r2 > ROWS) return -1;
    if (c1 < 0 || c1 > COLS || c2 < 0 || c2 > COLS) return -1;
    if (r1 == r2 && (c1 - c2 == 1 || c2 - c1 == 1))
        return hline_index(r1, c1 < c2 ? c1 : c2);
    if (c1 == c2 && (r1 - r2 == 1 || r2 - r1 == 1))
        return vline_index(r1 < r2 ? r1 : r2, c1);
    return -1;
}

int board_is_drawn(const Board *b, int line) {
    return (int)((b->lines >> line) & 1);
}

int board_apply_move(Board *b, int line, int player) {
    init_tables();
    b->lines |= ((LineMask)1) << line;
    int n = 0;
    for (int i = 0; i < 2; i++) {
        int bx = lineBoxes[line][i];
        if (bx >= 0 && (b->lines & boxMask[bx]) == boxMask[bx]) {
            b->owner[bx / COLS][bx % COLS] = (char)('A' + player);
            b->score[player]++;
            n++;
        }
    }
    return n;
}

int board_is_full(const Board *b) {
    return (b->lines & FULL_MASK) == FULL_MASK;
}

/* Layout (matches the sample run): dots/horizontal lines on one text row,
   vertical lines and box owners on the next. Column c sits at offset 2c. */
void board_print(const Board *b) {
    printf(" ");
    for (int c = 0; c <= COLS; c++) printf(" %d", c);
    printf("\n");
    for (int r = 0; r <= ROWS; r++) {
        printf("%d ", r);
        for (int c = 0; c <= COLS; c++) {
            printf(".");
            if (c < COLS) printf(board_is_drawn(b, hline_index(r, c)) ? "-" : " ");
        }
        printf("\n");
        if (r < ROWS) {
            printf("  ");
            for (int c = 0; c <= COLS; c++) {
                printf(board_is_drawn(b, vline_index(r, c)) ? "|" : " ");
                if (c < COLS) printf("%c", b->owner[r][c]);
            }
            printf("\n");
        }
    }
}
