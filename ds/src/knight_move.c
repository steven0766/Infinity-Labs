int KnightTour(size_t row_, size_t column_)
{
	/* init bit array */
	/* call CanIVisitAllIMP */
}

static int IsPositionValidIMP(size_t row_, size_t column_);
static void AddPositionIMP(size_t row, size_t column, bitarray bit_arr_);
static void InitDirectionIMP(bitarray bit_arr_);

static int CanIVisitAllIMP(size_t row_, size_t column_, bitarray bit_arr_)
{
	/* declare x, y*/
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
		/* init x as row, y as column */
		/* call GetNewDirIMP(row_ + lut[i][0], column_ + lut[i][1], dir) to get new position */
		/* if call CanIVisitAllIMP(x, t, bit_arr_) return TRUE */
			/* return TRUE */
	
	/* return FALSE */
}













