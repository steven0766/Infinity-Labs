/***************************************
* Reviewer: Itay
* Developer: Steve
* Date: 09.2.26
* Version: 1
****************************************/
#include <stdlib.h>			/* strtod */
#include <string.h>			/* strlen */
#include <ctype.h>			/* isspace */
#include <assert.h>			/* assert */

#include "calculator.h"		/* calc_status_ty */
#include "stack.h"			/* full stack api */

#define ASCII		256

typedef enum state
{
	WAIT_OPD = 0,
	WAIT_OPT = 1,
	STATE_SIZE = 2
}state_ty;

typedef enum input
{
	OPD = 0,
	OPT = 1,
	OPEN_PAR = 2,
	CLOSE_PAR = 3,
	INVALID = 4,
	END = 5,
	INPUT_SIZE = 6
}input_type_ty;

typedef struct token
{
	input_type_ty type;
	size_t len;
	union {
		double number;
		char operator;
	}value;
}token_ty;

typedef calc_status_ty (*opt_handler_ty)(token_ty* token, 
					stack_ty* opd_stk, stack_ty* opt_stk);
typedef calc_status_ty (*op_func_ty)(double* arg1, double* arg2);

typedef struct transition
{
	opt_handler_ty handler;
	state_ty next_state;
}transition_ty;

typedef struct opt
{
	int precedence;
	op_func_ty op_func;
}opt_ty;

/*************************************HANDLERS*********************************/
static calc_status_ty HandleOpd(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk);
static calc_status_ty HandleOpt(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk);
static calc_status_ty HandleOpenPar(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk);
static calc_status_ty HandleClosePar(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk);
static calc_status_ty HandleError(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk);
static calc_status_ty HandleEnd(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk);
static calc_status_ty HandleUnary(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk);
/***********************************OPERATIONS*********************************/
static calc_status_ty AddOp(double* arg1, double* arg2);
static calc_status_ty SubOp(double* arg1, double* arg2);
static calc_status_ty MulOp(double* arg1, double* arg2);
static calc_status_ty DivOp(double* arg1, double* arg2);
/***********************************HELPERS************************************/
static void InitStates(void);
static void InitOperators(void);
static token_ty GetNextToken(const char* s);
static calc_status_ty EvaluateTopOperator(stack_ty* opd_stk, stack_ty* opt_stk);
static void HandleUnaryInParentheses(stack_ty* opd_stk, stack_ty* opt_stk);
static int IsDigit(const char c);
static int IsOpt(const char c);
static int IsUnaryOpt(const char s);
static int IsOpenPar(const char c);
static int IsClosePar(const char c);


static transition_ty states_g[STATE_SIZE][INPUT_SIZE] = {0};
static opt_ty opt_g[ASCII] = {0};
static int sign = 1;


calc_status_ty Calculator(const char* s, double* res)
{
	static int is_initialized = 0;
	calc_status_ty status = SUCCESS;
	token_ty token = {0};
	transition_ty transition = {0};
	state_ty state = WAIT_OPD;
	stack_ty* opd_stk = NULL;
	stack_ty* opt_stk = NULL;
	
	assert(s);
	assert(res);
	 
	opd_stk = StackCreate(strlen(s), sizeof(double));
	if (!opd_stk)
	{
		return OUT_OF_MEMORY;
	}
	
	opt_stk = StackCreate(strlen(s), sizeof(char));
	if (!opt_stk)
	{
		StackDestroy(opd_stk);
		opd_stk = NULL;
		
		return OUT_OF_MEMORY;
	}
	
	if (!is_initialized)
	{
		InitStates();
		InitOperators();
		is_initialized = 1;
	}
	
	do
	{
		token = GetNextToken(s);

		transition = states_g[state][token.type];
		status = transition.handler(&token, opd_stk, opt_stk);
		state = transition.next_state;

		s += token.len;
	} while (END != token.type&& SUCCESS == status);
	
	if (StackIsEmpty(opd_stk))
	{
		status = SYNTAX_ERROR;
	}
	else
	{
		*res = *(double*)StackPeek(opd_stk);
	}
	
	StackDestroy(opd_stk);
	StackDestroy(opt_stk);
	opd_stk = NULL;
	opt_stk = NULL;
	
	return status;
}

/*************************************HANDLERS*********************************/
static calc_status_ty HandleOpd(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk)
{	
	(void)opt_stk;
	
	StackPush(opd_stk, &token->value.number);
	
	return SUCCESS;
}

static calc_status_ty HandleOpt(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk)
{
	char top_opt = 0;
	int top_prec = 0;
	char new_opt = token->value.operator;
    	int new_prec = opt_g[(unsigned char)new_opt].precedence;
    	calc_status_ty status = SUCCESS;
	
	while (SUCCESS == status && !StackIsEmpty(opt_stk))
	{
		top_opt = *(char*)StackPeek(opt_stk);
		top_prec = opt_g[(unsigned char)top_opt].precedence;
		
		if (top_prec < new_prec)
		{
			break;
		}
		
		status = EvaluateTopOperator(opd_stk, opt_stk);
	}
	
	StackPush(opt_stk, &new_opt);
	
	return status;
}

static calc_status_ty HandleError(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk)
{
	(void)token;
	(void)opd_stk;
	(void)opt_stk;
	
	return SYNTAX_ERROR;
}

static calc_status_ty HandleEnd(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk)
{
	calc_status_ty status = SUCCESS;
	(void)token;
	
	while (SUCCESS == status && !StackIsEmpty(opt_stk))
	{
		status = EvaluateTopOperator(opd_stk, opt_stk);
	}
	
	return status;
}

static calc_status_ty HandleUnary(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk)
{
	(void)opd_stk;
	(void)opt_stk;
	
	if (!IsUnaryOpt(token->value.operator))
	{
		return SYNTAX_ERROR;
	}
	
	if (token->value.operator == '-')
	{
		sign *= -1;
	}
	
	return SUCCESS;
}


static calc_status_ty HandleOpenPar(token_ty* token, stack_ty* opd_stk, 
							stack_ty* opt_stk)
{
	if (-1 == sign)
	{
		HandleUnaryInParentheses(opd_stk, opt_stk);
	}
	
	StackPush(opt_stk, &token->value.operator);

	return SUCCESS;
}

static calc_status_ty HandleClosePar(token_ty* token, 
					stack_ty* opd_stk, stack_ty* opt_stk)
{
	calc_status_ty status = SUCCESS;

	(void)token;
	
	while (SUCCESS == status && !StackIsEmpty(opt_stk) && 
	       !IsOpenPar(*(char*)StackPeek(opt_stk)))
	{
		status = EvaluateTopOperator(opd_stk, opt_stk);

	}
	
	/* in case we didn't stop at open par */
	if (StackIsEmpty(opt_stk))
	{
		return SYNTAX_ERROR;
	}
	
	StackPop(opt_stk);

	return status;
}

/***********************************OPERATIONS*********************************/
static calc_status_ty AddOp(double* arg1, double* arg2)
{
	*arg2 = *arg2 + *arg1;
	
	return SUCCESS;
}

static calc_status_ty SubOp(double* arg1, double* arg2)
{
	*arg2 = *arg2 - *arg1;
	
	return SUCCESS;
}

static calc_status_ty MulOp(double* arg1, double* arg2)
{
	*arg2 = *arg2 * *arg1;
	
	return SUCCESS;
}

static calc_status_ty DivOp(double* arg1, double* arg2)
{
	if (0 == *arg1)
	{
		return LOGIC_ERROR;
	}
	
	*arg2 = *arg2 / *arg1;
	
	return SUCCESS;
}
/***********************************HELPERS************************************/
static void InitStates(void)
{
	/* WAIT_OPD state */
	states_g[WAIT_OPD][OPD].handler = HandleOpd;
	states_g[WAIT_OPD][OPD].next_state = WAIT_OPT;

	states_g[WAIT_OPD][OPT].handler = HandleUnary;
	states_g[WAIT_OPD][OPT].next_state = WAIT_OPD;
	
	states_g[WAIT_OPD][OPEN_PAR].handler = HandleOpenPar;
	states_g[WAIT_OPD][OPEN_PAR].next_state = WAIT_OPD;

	states_g[WAIT_OPD][CLOSE_PAR].handler = HandleError;
	states_g[WAIT_OPD][CLOSE_PAR].next_state = WAIT_OPD;
	
	states_g[WAIT_OPD][INVALID].handler = HandleError;
	states_g[WAIT_OPD][INVALID].next_state = WAIT_OPD;
	
	states_g[WAIT_OPD][END].handler = HandleEnd;
	states_g[WAIT_OPD][END].next_state = WAIT_OPD;

	/* WAIT_OPT state */
	states_g[WAIT_OPT][OPD].handler = HandleError;
	states_g[WAIT_OPT][OPD].next_state = WAIT_OPT;

	states_g[WAIT_OPT][OPT].handler = HandleOpt;
	states_g[WAIT_OPT][OPT].next_state = WAIT_OPD;
	
	states_g[WAIT_OPT][OPEN_PAR].handler = HandleError;
	states_g[WAIT_OPT][OPEN_PAR].next_state = WAIT_OPT;

	states_g[WAIT_OPT][CLOSE_PAR].handler = HandleClosePar;
	states_g[WAIT_OPT][CLOSE_PAR].next_state = WAIT_OPT;
	
	states_g[WAIT_OPT][INVALID].handler = HandleError;
	states_g[WAIT_OPT][INVALID].next_state = WAIT_OPT;

	states_g[WAIT_OPT][END].handler = HandleEnd;
	states_g[WAIT_OPT][END].next_state = WAIT_OPT;
}

static void InitOperators(void)
{
	opt_g['+'].precedence = 1;
	opt_g['+'].op_func = AddOp;
	
	opt_g['-'].precedence = 1;
	opt_g['-'].op_func = SubOp;
	
	opt_g['*'].precedence = 2;
	opt_g['*'].op_func = MulOp;
	
	opt_g['/'].precedence = 2;
	opt_g['/'].op_func = DivOp;
}

static void HandleUnaryInParentheses(stack_ty* opd_stk, stack_ty* opt_stk)
{
	double paren_sign = -1;
	char mul = '*';
	
	StackPush(opd_stk, &paren_sign);
	StackPush(opt_stk, &mul);
	sign *= -1;
}

static token_ty GetNextToken(const char* s)
{
	token_ty token = {0};
	const char* start = s;
	char* end = NULL;
	
	while (*s && isspace(*s))
	{
		++s;
	}
	
	token.len = s - start + 1;
	
	if (!*s)
	{
		token.type = END;
		token.len = s - start;
	}
	else if (IsDigit(*s))
	{
		token.type = OPD;
		token.value.number = strtod(s, &end) * sign;
		sign = 1;
		token.len = end - start; 
	}
	else if (IsOpt(*s))
	{
		token.type = OPT;
		token.value.operator = *s;
	}
	else if (IsOpenPar(*s))
	{
		token.type = OPEN_PAR;
		token.value.operator = *s;
	}
	else if (IsClosePar(*s))
	{
		token.type = CLOSE_PAR;
	}
	else
	{
		token.type = INVALID;
	}
	
	return token;
}

static calc_status_ty EvaluateTopOperator(stack_ty* opd_stk, stack_ty* opt_stk)
{
	double arg1 = 0;
	double arg2 = 0;
	opt_ty opt_data = {0};
	calc_status_ty status = SUCCESS;
	char opt = *(char*)StackPeek(opt_stk);

	if (!IsOpt(opt))
	{
		return SYNTAX_ERROR;
	}

	StackPop(opt_stk);
	
	opt_data = opt_g[(unsigned char)opt];
	
	arg1 = *(double*)StackPeek(opd_stk);
	StackPop(opd_stk);
	
	if (StackIsEmpty(opd_stk))
	{
		return SYNTAX_ERROR;
	}
	
	arg2 = *(double*)StackPeek(opd_stk);
	StackPop(opd_stk);
	
	status = opt_data.op_func(&arg1, &arg2);
	StackPush(opd_stk, &arg2);
	
	return status;
}

static int IsUnaryOpt(const char c)
{
	return ('+' == c || '-' == c);
}
static int IsDigit(const char c)
{
	return ('0' <= c && '9' >= c);
}

static int IsOpt(const char c)
{
	return ('+' == c || '-' == c || '*' == c || '/' == c);
}

static int IsOpenPar(const char c)
{
	return ('(' == c);
}

static int IsClosePar(const char c)
{
	return (')' == c);
}

