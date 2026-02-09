#ifndef __ILRD_TEST_UTILS_H__
#define __ILRD_TEST_UTILS_H__

#include <stddef.h>

/*===========================================================================
 * Test Suite Management
 *===========================================================================*/

/* Initialize a new test suite - resets counters */
void TestSuiteBegin(const char* suite_name);

/* Print summary and return 0 if all passed, 1 if any failed */
int TestSuiteEnd(void);

/*===========================================================================
 * Assertion Macros - automatically track line numbers
 *===========================================================================*/

#define ASSERT_INT(expected, actual) \
    AssertInt((expected), (actual), __FILE__, __LINE__)

#define ASSERT_DOUBLE(expected, actual) \
    AssertDouble((expected), (actual), __FILE__, __LINE__)
    
#define ASSERT_SIZE(expected, actual) \
    AssertSize((expected), (actual), __FILE__, __LINE__)

#define ASSERT_PTR(expected, actual) \
    AssertPtr((expected), (actual), __FILE__, __LINE__)

#define ASSERT_TRUE(condition) \
    AssertTrue((condition), #condition, __FILE__, __LINE__)

#define ASSERT_FALSE(condition) \
    AssertFalse((condition), #condition, __FILE__, __LINE__)

#define ASSERT_SUCCESS(condition) \
    AssertSuccess((condition), #condition, __FILE__, __LINE__)

#define ASSERT_FAIL(condition) \
    AssertFail((condition), #condition, __FILE__, __LINE__)
    
#define ASSERT_NOT_NULL(ptr) \
    AssertNotNull((ptr), #ptr, __FILE__, __LINE__)

#define ASSERT_NULL(ptr) \
    AssertNull((ptr), #ptr, __FILE__, __LINE__)

#define ASSERT_STR_EQUAL(expected, actual) \
    AssertStrEqual((expected), (actual), __FILE__, __LINE__)

#define ASSERT_INT_ARRAY(expected, actual, length) \
    AssertIntArray((expected), (actual), (length), __FILE__, __LINE__)

/*===========================================================================
 * Assertion Functions - called by macros - don't call directly
 *===========================================================================*/

int AssertInt(int expected, int actual, const char* file, int line);
int AssertDouble(double expected, double actual, const char* file, int line);
int AssertSize(size_t expected, size_t actual, const char* file, int line);
int AssertPtr(const void* expected, const void* actual, const char* file, int line);
int AssertTrue(int condition, const char* condition_str, const char* file, int line);
int AssertFalse(int condition, const char* condition_str, const char* file, int line);
int AssertSuccess(int condition, const char* condition_str, const char* file, int line);
int AssertFail(int condition, const char* condition_str, const char* file, int line);
int AssertNotNull(const void* ptr, const char* ptr_str, const char* file, int line);
int AssertNull(const void* ptr, const char* ptr_str, const char* file, int line);
int AssertStrEqual(const char* expected, const char* actual, const char* file, int line);
int AssertIntArray(const int* expected, const int* actual, size_t length,
                   const char* file, int line);

/*===========================================================================
 * Manual Reporting - for custom checks
 *===========================================================================*/

void TestPass(const char* test_name);
void TestFail(const char* test_name, const char* message, const char* file, int line);

/*===========================================================================
 * Utility Functions
 *===========================================================================*/

/* Enable/disable colored output (enabled by default) */
void TestUtilsSetColorOutput(int enable);

/* Print a section header */
void TestPrintSection(const char* section_name);
/* Print a section sub-header */
void TestPrintSubSection(const char* subsection_name);

#endif /* __ILRD_TEST_UTILS_H__ */

