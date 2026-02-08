#include <stdio.h>		/* printf */
#include "test_utils.h" /* ASSERT_TRUE */
#include "fsm.h"		/* IsInLanguage */

static void TestFsm(void);

int main()
{
	TestSuiteBegin("fsm");
	printf("========================================\n");
	printf("test for fsm:\n");
	printf("________________________________________\n");
	TestFsm();
	
	return TestSuiteEnd();
}

static void TestFsm(void)
{
	const char* str1 = "01001110";
	const char* str2 = "10010";
	const char* str3 = "101";
	const char* str4 = "00";
	
	ASSERT_TRUE(IsInLanguage(str1));
	ASSERT_FALSE(IsInLanguage(str2));
	ASSERT_FALSE(IsInLanguage(str3));
	ASSERT_TRUE(IsInLanguage(str4));
}
