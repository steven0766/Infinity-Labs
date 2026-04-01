#ifndef __KNIGHT_MOVE__
#define __KNIGHT_MOVE__

#include <stddef.h> /* size_t */

enum
{
    BOARD_ROWS = 8,
    BOARD_COLS = 8,
    BOARD_SIZE = 64,
    MAX_MOVES = 8
};

typedef struct step
{
    size_t row;
    size_t col;
} step_ty;

typedef enum knight_status
{
    KNIGHT_SUCCESS = 0,
    KNIGHT_FAIL = 1,
    KNIGHT_TIMEOUT = 2
} knight_status_ty;

int KnightTour(size_t row_, size_t column_, step_ty path_[BOARD_SIZE]);

int KnightTourRandom(step_ty path_[BOARD_SIZE]);

#endif /* __KNIGHT_MOVE__ */
