#include <stdio.h>

#include "knight_move.h"

static void PrintPathIMP(step_ty path_[BOARD_SIZE]);
static const char *StatusToStringIMP(int status_);

int main(void)
{
    step_ty path[BOARD_SIZE];
    int status = 0;
    size_t i = 0;

    status = KnightTour(0, 0, path);

    printf("KnightTour from (0,0): %s\n", StatusToStringIMP(status));

    if (KNIGHT_SUCCESS == status)
    {
        PrintPathIMP(path);
    }

    printf("\n");

    status = KnightTourRandom(path);

    printf("KnightTourRandom: %s\n", StatusToStringIMP(status));

    if (KNIGHT_SUCCESS == status)
    {
        for (i = 0; i < BOARD_SIZE; ++i)
        {
            printf("%2lu -> (%lu, %lu)\n",
                   (unsigned long)i,
                   (unsigned long)path[i].row,
                   (unsigned long)path[i].col);
        }
    }

    return 0;
}

static void PrintPathIMP(step_ty path_[BOARD_SIZE])
{
    size_t i = 0;

    for (i = 0; i < BOARD_SIZE; ++i)
    {
        printf("%2lu -> (%lu, %lu)\n",
               (unsigned long)i,
               (unsigned long)path_[i].row,
               (unsigned long)path_[i].col);
    }
}

static const char *StatusToStringIMP(int status_)
{
    switch (status_)
    {
        case KNIGHT_SUCCESS:
            return "SUCCESS";
        case KNIGHT_FAIL:
            return "FAIL";
        case KNIGHT_TIMEOUT:
            return "TIMEOUT";
        default:
            return "UNKNOWN";
    }
}
