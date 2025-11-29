#include <stdio.h>   /* printf, sprintf */
#include <string.h>  /* strlen, strcpy, strcat, memcpy */
#include <stdlib.h>  /* malloc, realloc, free, exit */

typedef struct element element_ty;
typedef void(*add_value_func_ty)(element_ty*, int);
typedef void(*print_value_func_ty)(element_ty*);
typedef void(*cleanup_func_ty)(element_ty*);

struct element
{
    void* val;
    add_value_func_ty add;
    print_value_func_ty print;
    cleanup_func_ty cleanup;
};

#define MAX2(a,b) (((a) > (b)) ? (a) : (b))
#define MAX3(a,b,c) (MAX2((MAX2((a),(b))),(c)))
#define SIZEOF_VAR(var) ((char*)(&(var)+1) - (char*)(&(var)))
#define SIZEOF_TYPE(type) ((size_t)((char*)(((type*)0) + 1) - (char*)((type*)0)))

#define SIZE_OF(elements) sizeof(elements) / sizeof(elements[0])
#define MAX_INT           11


static void TestElements();
void InitInt(element_ty* e, int value);
void PrintInt(element_ty* data);
void AddInt(element_ty* data, int value);
void CleanupInt(element_ty* data);
void InitFloat(element_ty* e, float value);
void AddFloat(element_ty* data, int value);
void PrintFloat(element_ty* e);
void CleanupFloat(element_ty* data);
void InitString(element_ty* e, char* value);
void AddString(element_ty* e, int value);
void PrintString(element_ty* data);
void CleanupString(element_ty* data);
static void TestMax2();
static void TestMax3();
static void TestSizeofVar();
static void TestSizeofType();

int main()
{
    TestElements();
    TestMax2();
    TestMax3();
    TestSizeofVar();
    TestSizeofType();
    return 0;
}

static void TestElements()
{
    size_t i = 0;

    element_ty elements[5];

    InitFloat(&elements[0], 4.2);
    InitFloat(&elements[1], 6.7);
    InitString(&elements[2], "chapter");
    InitInt(&elements[3], 12);
    InitFloat(&elements[4], 56.32);
    
    for (i = 0; i < SIZE_OF(elements); ++i)
    {
        printf("Element: %ld\n", i);
        elements[i].print(&elements[i]);
        elements[i].add(&elements[i], 10);
        elements[i].print(&elements[i]);
        elements[i].cleanup(&elements[i]);
        printf("\n");
    }
}

void InitInt(element_ty* data, int value)
{   
    data->val = (void*)(long)value;
    data->add = AddInt;
    data->print = PrintInt; 
    data->cleanup = CleanupInt;
}

void AddInt(element_ty* data, int value)
{
    long new_val = (long)data->val;
    new_val += value;
    data->val = (void*)new_val;
}

void PrintInt(element_ty* data)
{
    printf("%d\n", (int)(long)data->val);
}

void CleanupInt(element_ty* data)
{
    (void)data;
}


void InitFloat(element_ty* data, float value)
{
    long bits = 0;
    memcpy(&bits, &value, sizeof(float));
    data->val = (void*)bits;
    data->add = AddFloat;
    data->print = PrintFloat;
    data->cleanup = CleanupFloat;
}

void AddFloat(element_ty* data, int value)
{
    long bits = (long)data->val;
    float f = 0;
    
    memcpy(&f, &bits, sizeof(float));
    f += value;
    memcpy(&bits, &f, sizeof(float));
    
    data->val = (void*)bits;
}

void PrintFloat(element_ty* data)
{
    long bits = (long)data->val;
    float f = 0;
    memcpy(&f, &bits, sizeof(float));
    printf("%f\n", f);
}

void CleanupFloat(element_ty* data)
{
    (void)data;
}


void InitString(element_ty* data, char* value)
{
    data->val = (char*)malloc((strlen(value)+1) * sizeof(char));
    if (NULL == data->val)
    {
        printf("string malloc failed\n");
        exit(1);
    }
    strcpy(data->val, value);
    
    data->add = AddString;
    data->print = PrintString;
    data->cleanup = CleanupString;
}

void AddString(element_ty* data, int value)
{
    char buffer[MAX_INT];
    size_t new_size = 0;
    
    sprintf(buffer, "%d", value);
    new_size = strlen((char*)data->val) + strlen(buffer) + 1;
    
    data->val = (char*)realloc(data->val, new_size * sizeof(char));
    if (NULL == data->val)
    {
        printf("string realloc failed");
        exit(1);
    }
    
    strcat(data->val, (const char*)buffer);
}

void PrintString(element_ty* data)
{
    printf("%s\n", (char*)data->val);
}

void CleanupString(element_ty* data)
{
    free(data->val);
}


static void TestMax2()
{
    int a = 5, b = 10;
    int result = MAX2(a,b);
    printf("the greater number between %d and %d is %d\n", a, b, result);
}

static void TestMax3()
{
    int a = 5, b = 10, c = 7;
    int result = MAX3(a,b,c);
    printf("the greater number between %d, %d and %d is %d\n", a, b, c, result);
}

static void TestSizeofVar()
{
    int    a = 0;
    double b = 0.0;
    char   c = 'A';
    size_t result[3] = {SIZEOF_VAR(a), SIZEOF_VAR(b), SIZEOF_VAR(c)};
    
    printf("SIZEOF_VAR(a): %lu, sizeof(a): %lu\n", result[0], sizeof(a));
    printf("SIZEOF_VAR(b): %lu, sizeof(b): %lu\n", result[1], sizeof(b));
    printf("SIZEOF_VAR(c): %lu, sizeof(c): %lu\n", result[2], sizeof(c));
}

static void TestSizeofType()
{
    printf("SIZEOF_TYPE(int): %lu, sizeof(int): %lu\n", SIZEOF_TYPE(int), sizeof(int));
    printf("SIZEOF_TYPE(double): %lu, sizeof(double): %lu\n", SIZEOF_TYPE(double), sizeof(double));
    printf("SIZEOF_TYPE(char): %lu, sizeof(char): %lu\n", SIZEOF_TYPE(char), sizeof(char));
}

