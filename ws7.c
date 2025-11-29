#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct element element_ty;
typedef void(*add_value_func_ty)(element_ty*, int);
typedef void(*print_value_func_ty)(element_ty*);
typedef void(*cleanup_func_ty)(element_ty*);

struct element
{
    int     int_val;
    float   float_val;
    char*   str_val;

    add_value_func_ty add;
    print_value_func_ty print;
    cleanup_func_ty cleanup;
};

#define MAX2(a,b) (((a) > (b)) ? (a) : (b))
#define MAX3(a,b,c) (((a) > (b)) ? (((a) > (c)) ? (a) : (c)) : ((b) > (c)) ? (b) : (c))
#define SIZEOF_VAR(var) ((char*)(&(var)+1) - (char*)(&(var)))
#define SIZEOF_TYPE(type) ((size_t)((char*)(((type*)0) + 1) - (char*)((type*)0)))


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
    
    for (i = 0; i < sizeof(elements) / sizeof(elements[0]); ++i)
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
    data->int_val = value;
    data->add = AddInt;
    data->print = PrintInt; 
    data->cleanup = CleanupInt;
}

void AddInt(element_ty* data, int value)
{
    data->int_val += value;
}

void PrintInt(element_ty* data)
{
    printf("%d\n", data->int_val);
}

void CleanupInt(element_ty* data)
{
    (void)data;
}


void InitFloat(element_ty* data, float value)
{
    data->float_val = value;
    data->add = AddFloat;
    data->print = PrintFloat;
    data->cleanup = CleanupFloat;
}

void AddFloat(element_ty* data, int value)
{
    data->float_val += value;
}

void PrintFloat(element_ty* data)
{
    printf("%f\n", data->float_val);
}

void CleanupFloat(element_ty* data)
{
    (void)data;
}


void InitString(element_ty* data, char* value)
{
    data->str_val = (char*)malloc((strlen(value)+1) * sizeof(char));
    if (NULL == data->str_val)
    {
        printf("string malloc failed\n");
        exit(1);
    }
    strcpy(data->str_val, value);
    
    data->add = AddString;
    data->print = PrintString;
    data->cleanup = CleanupString;
}

void AddString(element_ty* data, int value)
{
    char buffer[32];
    size_t new_size = 0;
    
    sprintf(buffer, "%d", value);
    new_size = strlen(data->str_val) + strlen(buffer) + 1;
    data->str_val = (char*)realloc(data->str_val, new_size * sizeof(char));
    
    if (NULL == data->str_val)
    {
        printf("string realloc failed");
        exit(1);
    }
    
    strcat(data->str_val, buffer);
}

void PrintString(element_ty* data)
{
    printf("%s\n", (char*)data->str_val);
}

void CleanupString(element_ty* data)
{
    free(data->str_val);
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
    
    printf("the size of variable a (int) is %lu\n", result[0]);
    printf("the size of variable b (double) is %lu\n", result[1]);
    printf("the size of variable c (char) is %lu\n", result[2]);
}

static void TestSizeofType()
{
    printf("the size of int is %lu\n", SIZEOF_TYPE(int));
    printf("the size of double is %lu\n", SIZEOF_TYPE(double));
    printf("the size of char is %lu\n", SIZEOF_TYPE(char));
}
