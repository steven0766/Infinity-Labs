#include "knight_move.h"
#include "bit_array.h"
#include "utils.h"

int KnightTour(size_t row_, size_t column_)
{
	/* init bit array */
	bit_array_ty bit_arr = 0;
	/* return CanIVisitAllIMP */
	return CanIVisitAllIMP(row_, column_, bit_arr);
}

static int IsPositionValidIMP(size_t row_, size_t column_);
static void AddPositionIMP(size_t row, size_t column, bitarray bit_arr_);
static void InitDirectionIMP(bitarray bit_arr_);

static int CanIVisitAllIMP(size_t row_, size_t column_, bitarray bit_arr_)
{
	/* declare static int lut[8][2] */

	/* if BitArrayCountOn return 64 */

		/* return TRUE */
	
	/* if knight visit out of board */

		/* return FALSE */

	/* if knight visited here (BitArrayGetBitVal is TRUE) */

		/* return FALSE */

		
	/* AddPositionIMP */

	
	/* if lut doesn't initialized */
		/* InitDirectionIMP lut */
		
	/* loop through all directions (dir) */
		/* call GetNewDirIMP(row_ + lut[i][0], column_ + lut[i][1], dir) to get new position */
		/* if call CanIVisitAllIMP(x, t, bit_arr_) return TRUE */
			/* return TRUE */
	
	/* return FALSE */
}

static int IsPositionValidIMP(size_t row_, size_t column_)
{
	/* if row_ under 8 and column_ under 8 */
	return ((row_ < 8) && column_ < 8);
}

static bitarray AddPositionIMP(size_t row, size_t column, bitarray bit_arr_)
{
	/* call BitArraySetOn with 8 * row + column index */
	return BitArraySetOn(bit_arr_, GetPosition(row, column));
}

static size_t GetPosition(size_t row, size_t column)
{
	/* return 8 * row + column */
	return 8 * row + column;
}

static void InitDirectionIMP(bitarray bit_arr_)
{
	/* bit_arr_ = 2 */
	/* bit_arr_ = 1 */
	
	/* bit_arr_ = 2 */
	/* bit_arr_ = -1 */
	
	/* bit_arr_ = 1 */
	/* bit_arr_ = -2 */
	
	/* bit_arr_ = -1 */
	/* bit_arr_ = -2 */
	
	/* bit_arr_ = -2 */
	/* bit_arr_ = -1 */
	
	/* bit_arr_ = -2 */
	/* bit_arr_ = 1 */
	
	/* bit_arr_ = -1 */
	/* bit_arr_ = 2 */
	
	/* bit_arr_ = 1 */
	/* bit_arr_ = 2 */
}











