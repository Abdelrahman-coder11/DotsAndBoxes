#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#define ROWS 4                          /* boxes per column */
#define COLS 5                          /* boxes per row    */
#define NUM_H ((ROWS + 1) * COLS)       /* horizontal lines: 25 */
#define NUM_V (ROWS * (COLS + 1))       /* vertical lines:   24 */
#define NUM_LINES (NUM_H + NUM_V)       /* 49 lines in total    */
#define NUM_BOXES (ROWS * COLS)         /* 20 boxes             */

/* One bit per line: bit i set <=> line i is drawn. */
typedef uint64_t LineMask;
#define FULL_MASK ((((LineMask)1) << NUM_LINES) - 1)

typedef struct {
    LineMask lines;
    char owner[ROWS][COLS];   /* ' ', 'A' or 'B' */
    int score[2];
} Board;

void board_init(Board *b);

/* Line indexing. hline: dot row r (0..ROWS), box column c.
   vline: box row r, dot column c (0..COLS). */
int hline_index(int r, int c);
int vline_index(int r, int c);

/* Dots (r1,c1)-(r2,c2) -> line index, or -1 if not adjacent/in range. */
int board_line_from_dots(int r1, int c1, int r2, int c2);

int board_is_drawn(const Board *b, int line);

/* Draw a line for player (0 = A, 1 = B). Returns boxes completed. */
int board_apply_move(Board *b, int line, int player);

int board_is_full(const Board *b);
void board_print(const Board *b);

#endif /* BOARD_H */
