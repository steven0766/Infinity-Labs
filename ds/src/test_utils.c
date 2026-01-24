#include "test_utils.h"
#include <stdio.h>
#include <string.h>

/*============================================================================
 * Global State
 *============================================================================*/

static size_t g_tests_passed = 0;
static size_t g_tests_total = 0;
static int g_color_enabled = 1;  /* Enabled by default */
static const char* g_suite_name = NULL;

/*============================================================================
 * ANSI Color Codes
 *============================================================================*/

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"

/*============================================================================
 * Helper Functions
 *============================================================================*/

static const char* GetFileName(const char* path)
{
    const char* filename = strrchr(path, '/');
    if (filename)
    {
        return filename + 1;
    }
    
    filename = strrchr(path, '\\');
    if (filename)
    {
        return filename + 1;
    }
    
    return path;
}

static void PrintPass(const char* file, int line)
{
    if (g_color_enabled)
    {
        printf("%s✓ PASS%s [%s:%d]\n", 
               COLOR_GREEN, COLOR_RESET, GetFileName(file), line);
    }
    else
    {
        printf("✓ PASS [%s:%d]\n", GetFileName(file), line);
    }
}

static void PrintFail(const char* file, int line, const char* message)
{
    if (g_color_enabled)
    {
        printf("%s✗ FAIL%s [%s:%d] - %s\n", 
               COLOR_RED, COLOR_RESET, GetFileName(file), line, message);
    }
    else
    {
        printf("✗ FAIL [%s:%d] - %s\n", GetFileName(file), line, message);
    }
}

/*============================================================================
 * Test Suite Management
 *============================================================================*/

void TestSuiteBegin(const char* suite_name)
{
    g_tests_passed = 0;
    g_tests_total = 0;
    g_suite_name = suite_name;
    
    printf("\n");
    if (g_color_enabled)
    {
        printf("%s", COLOR_BOLD);
    }
    printf("================================================================================\n");
    printf("Test Suite: %s\n", suite_name);
    printf("================================================================================\n");
    if (g_color_enabled)
    {
        printf("%s", COLOR_RESET);
    }
    printf("\n");
}

int TestSuiteEnd(void)
{
    int all_passed = (g_tests_passed == g_tests_total);
    
    printf("\n");
    if (g_color_enabled)
    {
        printf("%s", COLOR_BOLD);
    }
    printf("================================================================================\n");
    printf("Test Summary: %lu/%lu tests passed\n", 
           g_tests_passed, g_tests_total);
    printf("================================================================================\n");
    if (g_color_enabled)
    {
        printf("%s", COLOR_RESET);
    }
    
    if (all_passed)
    {
        if (g_color_enabled)
        {
            printf("%sPASSED%s\n", COLOR_GREEN, COLOR_RESET);
        }
        else
        {
            printf("PASSED\n");
        }
        return 0;
    }
    else
    {
        if (g_color_enabled)
        {
            printf("%sFAILED%s\n", COLOR_RED, COLOR_RESET);
        }
        else
        {
            printf("FAILED\n");
        }
        return 1;
    }
}

/*============================================================================
 * Assertion Functions
 *============================================================================*/

int AssertInt(int expected, int actual, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (expected == actual)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "expected %d, got %d", expected, actual);
    PrintFail(file, line, message);
    return 0;
}

int AssertSize(size_t expected, size_t actual, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (expected == actual)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "expected %lu, got %lu", expected, actual);
    PrintFail(file, line, message);
    return 0;
}

int AssertPtr(const void* expected, const void* actual, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (expected == actual)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "expected %p, got %p", expected, actual);
    PrintFail(file, line, message);
    return 0;
}

int AssertTrue(int condition, const char* condition_str, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (condition)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "condition '%s' is false", condition_str);
    PrintFail(file, line, message);
    return 0;
}

int AssertFalse(int condition, const char* condition_str, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (!condition)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "condition '%s' is true", condition_str);
    PrintFail(file, line, message);
    return 0;
}

int AssertSuccess(int condition, const char* condition_str, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (!condition)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "condition '%s' is fail", condition_str);
    PrintFail(file, line, message);
    return 0;
}

int AssertFail(int condition, const char* condition_str, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (condition)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "condition '%s' is success", condition_str);
    PrintFail(file, line, message);
    return 0;
}

int AssertNotNull(const void* ptr, const char* ptr_str, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (ptr != NULL)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "'%s' is NULL", ptr_str);
    PrintFail(file, line, message);
    return 0;
}

int AssertNull(const void* ptr, const char* ptr_str, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (ptr == NULL)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "'%s' is not NULL (value: %p)", ptr_str, ptr);
    PrintFail(file, line, message);
    return 0;
}

int AssertStrEqual(const char* expected, const char* actual, const char* file, int line)
{
    char message[256];
    
    g_tests_total++;
    
    if (expected == NULL && actual == NULL)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    if (expected == NULL || actual == NULL)
    {
        sprintf(message, "one string is NULL (expected: %p, actual: %p)", 
                (void*)expected, (void*)actual);
        PrintFail(file, line, message);
        return 0;
    }
    
    if (strcmp(expected, actual) == 0)
    {
        g_tests_passed++;
        PrintPass(file, line);
        return 1;
    }
    
    sprintf(message, "expected \"%s\", got \"%s\"", expected, actual);
    PrintFail(file, line, message);
    return 0;
}

int AssertIntArray(const int* expected, const int* actual, size_t length,
                   const char* file, int line)
{
    char message[256];
    size_t i = 0;
    
    g_tests_total++;
    
    if (expected == NULL || actual == NULL)
    {
        sprintf(message, "array is NULL (expected: %p, actual: %p)", 
                (void*)expected, (void*)actual);
        PrintFail(file, line, message);
        return 0;
    }
    
    for (i = 0; i < length; ++i)
    {
        if (expected[i] != actual[i])
        {
            sprintf(message, "arrays differ at index %lu: expected %d, got %d", 
                    i, expected[i], actual[i]);
            PrintFail(file, line, message);
            return 0;
        }
    }
    
    g_tests_passed++;
    PrintPass(file, line);
    return 1;
}

/*============================================================================
 * Manual Reporting
 *============================================================================*/

void TestPass(const char* test_name)
{
    g_tests_total++;
    g_tests_passed++;
    
    if (g_color_enabled)
    {
        printf("%s✓ PASS%s - %s\n", COLOR_GREEN, COLOR_RESET, test_name);
    }
    else
    {
        printf("✓ PASS - %s\n", test_name);
    }
}

void TestFail(const char* test_name, const char* message, const char* file, int line)
{
    g_tests_total++;
    
    if (g_color_enabled)
    {
        printf("%s✗ FAIL%s [%s:%d] - %s: %s\n", 
               COLOR_RED, COLOR_RESET, GetFileName(file), line, test_name, message);
    }
    else
    {
        printf("✗ FAIL [%s:%d] - %s: %s\n", 
               GetFileName(file), line, test_name, message);
    }
}

/*============================================================================
 * Utility Functions
 *============================================================================*/

void TestUtilsSetColorOutput(int enable)
{
    g_color_enabled = enable;
}

void TestPrintSection(const char* section_name)
{
    printf("\n");
    printf("--- %s ---\n", section_name);
    printf("\n");
}
