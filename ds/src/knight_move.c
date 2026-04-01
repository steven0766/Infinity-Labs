#include <stdlib.h> /* rand, srand, qsort */
#include <stddef.h> /* size_t */
#include <time.h>   /* time, difftime */
#include <assert.h> /* assert */

#include "knight_move.h"
#include "bit_array.h"

#define TIMEOUT_SECONDS (5 * 60)
#define INVALID_EVAL 9

typedef struct point
{
    int row;
    int col;
} point_ty;

typedef struct move_info
{
    point_ty pos;
    size_t eval;
} move_info_ty;

typedef struct lut_entry
{
    point_ty moves[MAX_MOVES];
    size_t count;
} lut_entry_ty;

static int SolveIMP(point_ty current_,
                    bit_array_ty visited_,
                    step_ty path_[BOARD_SIZE],
                    size_t depth_,
                    time_t start_time_);

static int IsPositionValidIMP(point_ty pos_);
static int IsVisitedIMP(point_ty pos_, bit_array_ty visited_);
static int IsAllVisitedIMP(bit_array_ty visited_);
static bit_array_ty AddPositionIMP(point_ty pos_, bit_array_ty visited_);
static size_t GetPositionIMP(point_ty pos_);
static point_ty IndexToPointIMP(size_t index_);

static void InitLUTIMP(void);
static void SeedRandIMP(void);
static int IsTimedOutIMP(time_t start_time_);

static size_t BuildMovesIMP(point_ty current_,
                            bit_array_ty visited_,
                            move_info_ty moves_[MAX_MOVES]);

static size_t GetMoveEvalIMP(point_ty pos_, bit_array_ty visited_);
static int CompareMovesIMP(const void *a_, const void *b_);

static lut_entry_ty g_lut[BOARD_SIZE] = {{{{0, 0}}, 0}};
static int g_lut_init = 0;
static int g_rand_seeded = 0;

int KnightTour(size_t row_, size_t column_, step_ty path_[BOARD_SIZE])
{
    point_ty start = {0, 0};
    bit_array_ty visited = 0;
    time_t start_time = 0;

    assert(NULL != path_);

    if (row_ >= BOARD_ROWS || column_ >= BOARD_COLS)
    {
        return KNIGHT_FAIL;
    }

    InitLUTIMP();

    start.row = (int)row_;
    start.col = (int)column_;
    start_time = time(NULL);

    return SolveIMP(start, visited, path_, 0, start_time);
}

int KnightTourRandom(step_ty path_[BOARD_SIZE])
{
    size_t index = 0;
    point_ty start = {0, 0};

    assert(NULL != path_);

    SeedRandIMP();

    index = (size_t)(rand() % BOARD_SIZE);
    start = IndexToPointIMP(index);

    return KnightTour((size_t)start.row, (size_t)start.col, path_);
}

static int SolveIMP(point_ty current_,
                    bit_array_ty visited_,
                    step_ty path_[BOARD_SIZE],
                    size_t depth_,
                    time_t start_time_)
{
    move_info_ty moves[MAX_MOVES] = {{{0, 0}, 0}};
    size_t move_count = 0;
    size_t i = 0;
    int status = KNIGHT_FAIL;

    if (IsTimedOutIMP(start_time_))
    {
        return KNIGHT_TIMEOUT;
    }

    if (!IsPositionValidIMP(current_))
    {
        return KNIGHT_FAIL;
    }

    if (IsVisitedIMP(current_, visited_))
    {
        return KNIGHT_FAIL;
    }

    visited_ = AddPositionIMP(current_, visited_);

    path_[depth_].row = (size_t)current_.row;
    path_[depth_].col = (size_t)current_.col;

    if (BOARD_SIZE - 1 == depth_)
    {
        return KNIGHT_SUCCESS;
    }

    if (IsAllVisitedIMP(visited_))
    {
        return KNIGHT_SUCCESS;
    }

    move_count = BuildMovesIMP(current_, visited_, moves);

    qsort(moves, move_count, sizeof(move_info_ty), CompareMovesIMP);

    for (i = 0; i < move_count; ++i)
    {
        status = SolveIMP(moves[i].pos,
                          visited_,
                          path_,
                          depth_ + 1,
                          start_time_);

        if (KNIGHT_SUCCESS == status)
        {
            return KNIGHT_SUCCESS;
        }

        if (KNIGHT_TIMEOUT == status)
        {
            return KNIGHT_TIMEOUT;
        }
    }

    return KNIGHT_FAIL;
}

static int IsPositionValidIMP(point_ty pos_)
{
    return (pos_.row >= 0 && pos_.row < BOARD_ROWS &&
            pos_.col >= 0 && pos_.col < BOARD_COLS);
}

static int IsVisitedIMP(point_ty pos_, bit_array_ty visited_)
{
    assert(1 == IsPositionValidIMP(pos_));

    return BitArrayGetBitVal(visited_, (unsigned int)GetPositionIMP(pos_));
}

static int IsAllVisitedIMP(bit_array_ty visited_)
{
    return (BOARD_SIZE == BitArrayCountOn(visited_));
}

static bit_array_ty AddPositionIMP(point_ty pos_, bit_array_ty visited_)
{
    assert(1 == IsPositionValidIMP(pos_));

    return BitArraySetOn(visited_, (unsigned int)GetPositionIMP(pos_));
}

static size_t GetPositionIMP(point_ty pos_)
{
    assert(1 == IsPositionValidIMP(pos_));

    return (size_t)(BOARD_COLS * pos_.row + pos_.col);
}

static point_ty IndexToPointIMP(size_t index_)
{
    point_ty ret = {0, 0};

    assert(index_ < BOARD_SIZE);

    ret.row = (int)(index_ / BOARD_COLS);
    ret.col = (int)(index_ % BOARD_COLS);

    return ret;
}

static void InitLUTIMP(void)
{
    static const int row_delta[MAX_MOVES] = {-2, -2, -1, 1, 2, 2, 1, -1};
    static const int col_delta[MAX_MOVES] = {1, -1, -2, -2, -1, 1, 2, 2};

    size_t row = 0;
    size_t col = 0;
    size_t dir = 0;
    size_t index = 0;
    point_ty current = {0, 0};
    point_ty next = {0, 0};

    if (g_lut_init)
    {
        return;
    }

    for (index = 0; index < BOARD_SIZE; ++index)
    {
        g_lut[index].count = 0;
    }

    for (row = 0; row < BOARD_ROWS; ++row)
    {
        for (col = 0; col < BOARD_COLS; ++col)
        {
            current.row = (int)row;
            current.col = (int)col;
            index = GetPositionIMP(current);

            for (dir = 0; dir < MAX_MOVES; ++dir)
            {
                next.row = current.row + row_delta[dir];
                next.col = current.col + col_delta[dir];

                if (IsPositionValidIMP(next))
                {
                    g_lut[index].moves[g_lut[index].count] = next;
                    ++g_lut[index].count;
                }
            }
        }
    }

    g_lut_init = 1;
}

static void SeedRandIMP(void)
{
    if (!g_rand_seeded)
    {
        srand((unsigned int)time(NULL));
        g_rand_seeded = 1;
    }
}

static int IsTimedOutIMP(time_t start_time_)
{
    time_t now = time(NULL);

    return (difftime(now, start_time_) >= TIMEOUT_SECONDS);
}

static size_t BuildMovesIMP(point_ty current_,
                            bit_array_ty visited_,
                            move_info_ty moves_[MAX_MOVES])
{
    size_t i = 0;
    size_t count = 0;
    size_t index = 0;
    point_ty next = {0, 0};

    assert(1 == IsPositionValidIMP(current_));
    assert(NULL != moves_);

    index = GetPositionIMP(current_);

    for (i = 0; i < g_lut[index].count; ++i)
    {
        next = g_lut[index].moves[i];

        if (!IsVisitedIMP(next, visited_))
        {
            moves_[count].pos = next;
            moves_[count].eval = GetMoveEvalIMP(next, visited_);
            ++count;
        }
    }

    return count;
}

static size_t GetMoveEvalIMP(point_ty pos_, bit_array_ty visited_)
{
    size_t i = 0;
    size_t ret = 0;
    size_t index = 0;
    point_ty next = {0, 0};

    if (!IsPositionValidIMP(pos_) || IsVisitedIMP(pos_, visited_))
    {
        return INVALID_EVAL;
    }

    index = GetPositionIMP(pos_);

    for (i = 0; i < g_lut[index].count; ++i)
    {
        next = g_lut[index].moves[i];

        if (!IsVisitedIMP(next, visited_))
        {
            ++ret;
        }
    }

    return ret;
}

static int CompareMovesIMP(const void *a_, const void *b_)
{
    const move_info_ty *move_a = (const move_info_ty *)a_;
    const move_info_ty *move_b = (const move_info_ty *)b_;

    assert(NULL != a_);
    assert(NULL != b_);

    if (move_a->eval < move_b->eval)
    {
        return -1;
    }

    if (move_a->eval > move_b->eval)
    {
        return 1;
    }

    return 0;
}
