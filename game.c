#include "game.h"

#include "board.h"

#include <stdio.h>
#include <string.h>

#define STARS "*****************************************************************************"

static int read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) return 0;
    size_t n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n') {
        buf[n - 1] = '\0';
    } else {                      /* line too long: discard the rest */
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    }
    return 1;
}

/* Returns a valid undrawn line index, or -1 on EOF. */
static int ask_move(const Board *b, int player) {
    char buf[128], extra;
    int r1, c1, r2, c2;
    for (;;) {
        printf("Player %c's turn. Enter the row and column of the first dot "
               "(e.g., A0 -> 0 0) and second dot: \n", 'A' + player);
        fflush(stdout);
        if (!read_line(buf, sizeof buf)) return -1;
        if (sscanf(buf, " %d %d %d %d %c", &r1, &c1, &r2, &c2, &extra) != 4) {
            printf("Invalid input. Enter four numbers: row1 col1 row2 col2.\n");
            continue;
        }
        int line = board_line_from_dots(r1, c1, r2, c2);
        if (line < 0)
            printf("Invalid move. Dots must be adjacent (rows 0-%d, columns 0-%d).\n",
                   ROWS, COLS);
        else if (board_is_drawn(b, line))
            printf("That line is already drawn. Choose another.\n");
        else
            return line;
    }
}

static void print_state(const Board *b) {
    printf("%s\n", STARS);
    printf("Player A score: %d\n", b->score[0]);
    printf("Player B score: %d\n", b->score[1]);
    printf("%s\n", STARS);
    board_print(b);
}

void game_run(void) {
    Board b;
    board_init(&b);
    int player = 0;

    printf("%s\n", STARS);
    board_print(&b);

    while (!board_is_full(&b)) {
        int line = ask_move(&b, player);
        if (line < 0) { printf("\nInput closed. Game aborted.\n"); return; }
        if (board_apply_move(&b, line, player) == 0)
            player = 1 - player;      /* a completed box keeps the turn */
        print_state(&b);
    }

    printf("\n=== GAME OVER ===\n");
    printf("Player A: %d boxes\nPlayer B: %d boxes\n", b.score[0], b.score[1]);
    if (b.score[0] > b.score[1]) printf("Winner: Player A!\n");
    else if (b.score[1] > b.score[0]) printf("Winner: Player B!\n");
    else printf("It's a draw!\n");
}
