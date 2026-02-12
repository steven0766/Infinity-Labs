#ifndef __ILRD_CALC_H__
#define __ILRD_CALC_H__

typedef enum status
{
    SUCCESS = 0,
    OUT_OF_MEMORY = 1,
    LOGIC_ERROR = 2,
    SYNTAX_ERROR = 3
} calc_status_ty;

/****************************************************************************** 
* 	Description:
* 		Calculates the expression and assignes res to be the result on SUCCESS
* 	Return Value:
* 		SUCCESS
* 		OUT_OF_MEMORY
* 		LOGIC_ERROR
* 		SYNTAX_ERROR
*   Undefined Behavior:
*       expression is NULL or res is NULL
******************************************************************************/
calc_status_ty Calculator(const char* s, double* res);

#endif /* __ILRD_CALC_H__ */

