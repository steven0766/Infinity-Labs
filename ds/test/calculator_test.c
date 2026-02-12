#include "calculator.h"
#include "test_utils.h"

static void TestAddOp(void);
static void TestSubOp(void);
static void TestMulOp(void);
static void TestDivOp(void);
static void TestUnaryOpt(void);
static void TestParentheses(void);
static void TestSpaces(void);
static void TestMultipleOperations(void);
static void TestSyntaxError(void);
static void TestEdgeCases(void);

int main()
{
	TestSuiteBegin("Calculator");
	
	TestPrintSection("Single Operations");
	
	TestPrintSubSection("Add Operation");
	TestAddOp();
	
	TestPrintSubSection("Substract Operation");
	TestSubOp();
	
	TestPrintSubSection("Multiply Operation");
	TestMulOp();
	
	TestPrintSubSection("Divide Operation");
	TestDivOp();
	
	TestPrintSubSection("Parentheses Operator");
	TestParentheses();
	
	TestPrintSubSection("Unary Operator");
	TestUnaryOpt();
	
	TestPrintSection("Spaces");
	TestSpaces();
	
	TestPrintSection("Multiple Operations");
	TestMultipleOperations();
	
	TestPrintSection("Syntax Error");
	TestSyntaxError();
	
	TestPrintSection("Edge Cases");
	TestEdgeCases();
	
	return TestSuiteEnd();
}

static void TestAddOp(void)
{
	double result = 0;
	double expected = 5;
	
	ASSERT_SUCCESS(Calculator("2 + 3", &result));
	ASSERT_DOUBLE(expected, result);

	expected = 0;
	ASSERT_SUCCESS(Calculator("0 + 0", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 1;
	ASSERT_SUCCESS(Calculator("-2 + 3", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -1;
	ASSERT_SUCCESS(Calculator("-2 + 1", &result));
	ASSERT_DOUBLE(expected, result);

	expected = -1.2;
	ASSERT_SUCCESS(Calculator("-2.3 + 1.1", &result));
	ASSERT_DOUBLE(expected, result);
}

static void TestSubOp(void)
{
	double result = 0;
	double expected = 1;
	
	ASSERT_SUCCESS(Calculator("3 - 2", &result));
	ASSERT_DOUBLE(expected, result);

	expected = 0;
	ASSERT_SUCCESS(Calculator("0 - 0", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 0;
	ASSERT_SUCCESS(Calculator("2 - 2", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -1;
	ASSERT_SUCCESS(Calculator("3 - 4", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -1.2;
	ASSERT_SUCCESS(Calculator("0 - 1.2", &result));
	ASSERT_DOUBLE(expected, result);
}

static void TestMulOp(void)
{
	double result = 0;
	double expected = 6;
	
	ASSERT_SUCCESS(Calculator("2 * 3", &result));
	ASSERT_DOUBLE(expected, result);

	expected = 0;
	ASSERT_SUCCESS(Calculator("0 * 5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 5;
	ASSERT_SUCCESS(Calculator("2.5 * 2", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -3;
	ASSERT_SUCCESS(Calculator("3 * -1", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 3;
	ASSERT_SUCCESS(Calculator("9 * 0.333333333", &result));
	ASSERT_DOUBLE(expected, result);
}

static void TestDivOp(void)
{
	double result = 0;
	double expected = 6;
	
	ASSERT_SUCCESS(Calculator("12 / 2", &result));
	ASSERT_DOUBLE(expected, result);

	expected = 0;
	ASSERT_SUCCESS(Calculator("0 / 5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 2.5;
	ASSERT_SUCCESS(Calculator("5 / 2", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -3;
	ASSERT_SUCCESS(Calculator("3 / -1", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -0.5;
	ASSERT_SUCCESS(Calculator("-4 / 8", &result));
	ASSERT_DOUBLE(expected, result);
}

static void TestParentheses(void)
{
	double result = 0;
	double expected = 6;
	
	ASSERT_SUCCESS(Calculator("(12 / 2)", &result));
	ASSERT_DOUBLE(expected, result);

	expected = 5;
	ASSERT_SUCCESS(Calculator("(0 + 5)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 10;
	ASSERT_SUCCESS(Calculator("(5 * 2)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 0;
	ASSERT_SUCCESS(Calculator("(3 - 3)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -0.5;
	ASSERT_SUCCESS(Calculator("(-4 / 8)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 36;
	ASSERT_SUCCESS(Calculator("2 * (3 * (5 + 1) - (2 - 2))", &result));
	ASSERT_DOUBLE(expected, result);
}

static void TestSpaces(void)
{
	double result = 0;
	double expected = 6;
	
	ASSERT_SUCCESS(Calculator("      2 * 3", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 36;
	ASSERT_SUCCESS(Calculator("2 * (3 *   (5 + 1) -   (2 - 2))", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 0;
	ASSERT_SUCCESS(Calculator("(     3    -    3    )   ", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 0;
	ASSERT_SUCCESS(Calculator("---  ---5 + -+-   +-5", &result));
	ASSERT_DOUBLE(expected, result);
}

static void TestUnaryOpt(void)
{
	double result = 0;
	double expected = 0;
	
	ASSERT_SUCCESS(Calculator("5 + -5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 10;
	ASSERT_SUCCESS(Calculator("5 + --5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 0;
	ASSERT_SUCCESS(Calculator("5 + -+-+-5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 0;
	ASSERT_SUCCESS(Calculator("------5 + -+-+-5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 10;
	ASSERT_SUCCESS(Calculator("------5 - -+-+-5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 0;
	ASSERT_SUCCESS(Calculator("(--5 - +5)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 1;
	ASSERT_SUCCESS(Calculator("1 -(--5 - +5)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -5;
	ASSERT_SUCCESS(Calculator("-----(10 - 5)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -4;
	ASSERT_SUCCESS(Calculator("-----(10 - 5) +-(2 - 3)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 5;
	ASSERT_SUCCESS(Calculator("---(---(10 - 5))", &result));
	ASSERT_DOUBLE(expected, result);
}

static void TestMultipleOperations(void)
{
	double result = 0;
	double expected = 1;
	
	ASSERT_SUCCESS(Calculator("2 * 3 - 5", &result));
	ASSERT_DOUBLE(expected, result);

	expected = 17;
	ASSERT_SUCCESS(Calculator("2 + 3 * 5", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 8;
	ASSERT_SUCCESS(Calculator("5 / 2 * 2 + 3", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -6;
	ASSERT_SUCCESS(Calculator("-1 - 3 * 2 + 1", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 4;
	ASSERT_SUCCESS(Calculator("3.333333333 * 1 + 2 / 3", &result));
	ASSERT_DOUBLE(expected, result);	
	
	expected = 16;
	ASSERT_SUCCESS(Calculator("(5 + 3) * 2", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 6;
	ASSERT_SUCCESS(Calculator("2 * (3 + 6) / 3", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 18;
	ASSERT_SUCCESS(Calculator("3 * (5 + 1)", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = 43;
	ASSERT_SUCCESS(Calculator("-3 * (-5 -4) + (6 - 2) * 4", &result));
	ASSERT_DOUBLE(expected, result);
	
	expected = -6;
	ASSERT_SUCCESS(Calculator("(2 - 8) * ( 3 - 1) / (4 - 2)", &result));
	ASSERT_DOUBLE(expected, result);	
}

static void TestSyntaxError(void)
{
	double result = 0;
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("a", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("*", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("/", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("-", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("+", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("1 + a", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("a + 1", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("1 * * 2", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("1 * + * 2", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("()", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator(")", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("(", &result));
		
	ASSERT_INT(SYNTAX_ERROR, Calculator("1 * (2 ", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("1 * ( 2 * 3 ", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("1 * ( * ", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("1 * ) * ", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("7 / a", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("2 + * 3", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("2 - - -- *3", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("7 % 2", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("(------)", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("2 +", &result));
	
	ASSERT_INT(SYNTAX_ERROR, Calculator("2 // 3", &result));
}


static void TestEdgeCases(void)
{
	double result = 0;
	double expected = 5;
	
	ASSERT_SUCCESS(Calculator("5", &result));
	ASSERT_DOUBLE(expected, result);
	
	ASSERT_INT(LOGIC_ERROR, Calculator("5 / 0", &result));	
}



