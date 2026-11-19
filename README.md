# Dots and Boxes – Sprint 1 (two-player)

Build: `make`   Run: `./dotsandboxes`   Clean: `make clean`

Move = row and column of the first dot, then of the second dot (adjacent), e.g. `0 4 0 3`.
Dots: rows 0-4, columns 0-5 (4x5 boxes). Completing a box shows A/B inside it and gives that player another turn.

Files: board.h/.c (board + box detection), game.h/.c (turn loop, validation, scores, winner), main.c, Makefile.

Debugging (take screenshots for the report):
    valgrind --leak-check=full --show-leak-kinds=all ./dotsandboxes
    gdb ./dotsandboxes     # break board_apply_move ; run ; bt ; print/x b->lines

Alpine autostart (no login): create /usr/local/bin/autogame
    #!/bin/sh
    while true; do /usr/local/bin/dotsandboxes; done
chmod +x it, then in /etc/inittab use:
    tty1::respawn:/sbin/getty -n -l /usr/local/bin/autogame 38400 tty1
(Not tested on your VM – verify.)
