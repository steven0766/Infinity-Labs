/*************************************
* Reviewer: Nimrod                   *
* Date: 1.12.25                      *
* Version: 1                         *
**************************************/

#include <stdio.h>   /* printf, fopen, fclose, fwrite, fread */
#include <string.h>  /* strcmp, strcpy, strlen */
#include <stdint.h>  /* uint8_t */
#include <stdlib.h>  /* exit */


typedef struct real_grades rl_grade_ty;
typedef struct humanistic_grades human_grade_ty;
typedef struct grade grade_ty;
typedef struct student student_ty;

#define MAX_NAME_LEN    25
#define EXIT            exit(1)

struct real_grades
{
    float phisics;
    float math;
};

struct humanistic_grades
{
    float sociology;
    float psychology;
};

struct grade
{
    human_grade_ty human_grades;
    rl_grade_ty rl_grades;
    float sport;
};

struct student
{
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    grade_ty grades;
};


static void TestStudent();
void InitStudent(student_ty* s, const char* first,const char* last,
                 float sociology, float psychology, float physics,
                 float math, float sport);
void PrintStudent(const student_ty* s);
int StudentCmp(const student_ty* a, const student_ty* b);
void WriteStudent(student_ty* student, char* filename);
void WriteGrades(grade_ty* grades, FILE* fptr);
void WriteHumanicGrades(human_grade_ty* grades, FILE* fptr);
void WriteRealGrades(rl_grade_ty* grades, FILE* fptr);
void ReadStudent(student_ty* student, char* filename);
void ReadGrades(grade_ty* grades, FILE* fptr);
void ReadRealGrades(rl_grade_ty* grades, FILE* fptr);
void ReadHumanicGrades(human_grade_ty* grades, FILE* fptr);
                 

int main()
{
    TestStudent();
    
    return 0;
}


static void TestStudent()
{
    student_ty s1;
    student_ty s2;
    InitStudent(&s1, "Steve", "Evtushenko",
                90.33,   /* sociology */
                90.2,    /* psychology */
                60.3,    /* physics */
                88.7,    /* math */
                87.23);  /* sport */
    WriteStudent(&s1, "save_student.bin");
    ReadStudent(&s2, "save_student.bin");
    
    PrintStudent(&s1);
    PrintStudent(&s2);
    
    if (StudentCmp(&s1, &s2))
    {
        printf("Students match\n");
    }
    else
    {
        printf("Students don't match\n");
    }
}

void InitStudent(student_ty* s, const char* first,const char* last,
                 float sociology, float psychology, float physics,
                 float math, float sport)
{
    strncpy(s->first_name, first, MAX_NAME_LEN);
    strncpy(s->last_name, last, MAX_NAME_LEN);

    s->grades.human_grades.sociology = sociology;
    s->grades.human_grades.psychology = psychology;
    s->grades.rl_grades.phisics = physics;
    s->grades.rl_grades.math = math;
    s->grades.sport = sport;
}

void PrintStudent(const student_ty* s)
{
    printf("First name: %s\n", s->first_name);
    printf("Last name: %s\n", s->last_name);
    printf("Sociology: %.2f\n", s->grades.human_grades.sociology);
    printf("Psychology: %.2f\n", s->grades.human_grades.psychology);
    printf("Physics: %.2f\n", s->grades.rl_grades.phisics);
    printf("Math: %.2f\n", s->grades.rl_grades.math);
    printf("Sport: %.2f\n", s->grades.sport);
}

size_t StudentCmp(const student_ty* a, const student_ty* b)
{
    if (0 == strcmp(a->first_name, b->first_name)
    {
        if (0 == strcmp(a->last_name, b->last_name)
        {
            if(GradeCmp(&a->grade, &b->grade)
            {
                return 1;
            }
        }
    }
    
    return 0;
}

size_t GradeCmp(const grade_ty a, const grade_ty b)
{
    if (HumanGradesCmp(&a->human_grades, &b->human_grades)
    {
        if (RealGradesCmp(&a->rl_grades, &b->rl_grades)
        {
            if (a->sport == b->sport)
            {
                return 1;
            }
        }
    }
    
    return 0;
}

size_t HumanGradesCmp(const human_grade_ty a, const human_grade_ty b)
{
    if (a->sociology == b->sociology)
    {
        if (a->psychology == b->psychology)
        {
            return 1;
        }
    }
    
    return 0;
}

size_t RealGradesCmp(const rl_grade_ty a, const rl_grade_ty b)
{
    if (a->phisics == b->phisics)
    {
        (a->math == b->math)
        {
            return 1;
        }
    }
    
    return 0;
}

void WriteStudent(student_ty* student, char* filename)
{
    FILE* fptr = fopen(filename, "wb");
    uint8_t name_len = strlen(student->first_name) + 1;
    uint8_t last_name_len = strlen(student->last_name) + 1;
    
    if (1 != fwrite(&name_len, sizeof(uint8_t), 1, fptr))
    {
        printf("set first_name size in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set first_name size in file failed");
        }
        EXIT;
    }
    if (name_len != fwrite(&student->first_name, sizeof(char), name_len, fptr))
    {
        printf("set first_name in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set first_name in file failed");
        }
        EXIT;
    }
    if (1 != fwrite(&last_name_len, sizeof(uint8_t), 1, fptr))
    {
        printf("set last_name size in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set last_name size in file failed");
        }
        EXIT;
    }
    if (last_name_len != fwrite(&student->last_name, sizeof(char), last_name_len, fptr))
    {
        printf("set last_name in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set last_name in file failed");
        }
        EXIT;
    }
    WriteGrades(&student->grades, fptr);
    if (fclose(fptr))
    {
        printf("closing file after set all grades failed");
        EXIT;
    }
}

void WriteGrades(grade_ty* grades, FILE* fptr)
{
    WriteHumanicGrades(&grades->human_grades, fptr);
    WriteRealGrades(&grades->rl_grades, fptr);
    if (1 != fwrite(&grades->sport, sizeof(float), 1, fptr))
    {
        printf("set sport grade in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set sport grade in file failed");
        }
        EXIT;
    }
}

void WriteHumanicGrades(human_grade_ty* grades, FILE* fptr)
{
    if (1 != fwrite(&grades->sociology, sizeof(float), 1, fptr))
    {
        printf("set sociology grade in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set sociology grade in file failed");
        }
        EXIT;
    }
    if (1 != fwrite(&grades->psychology, sizeof(float), 1, fptr))
    {
        printf("set psychology grade in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set psychology grade in file failed");
        }
        EXIT;
    }
}

void WriteRealGrades(rl_grade_ty* grades, FILE* fptr)
{
    if (1 != fwrite(&grades->phisics, sizeof(float), 1, fptr))
    {
        printf("set phisics grade in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set phisics grade in file failed");
        }
        EXIT;
    }
    if (1 != fwrite(&grades->math, sizeof(float), 1, fptr))
    {
        printf("set math grade in file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after set math grade in file failed");
        }
        EXIT;
    }
}

void ReadStudent(student_ty* student, char* filename)
{
    uint8_t name_len = 0;
    uint8_t last_name_len = 0;
    FILE* fptr = fopen(filename, "rb");
    if (NULL == fptr)
    {
        printf("failed to open file");
        EXIT;
    }
    
    if (1 != fread(&name_len, sizeof(uint8_t), 1, fptr))
    {
        printf("get first_name size from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get first_name size failed");
        }
        EXIT;
    }
    if (name_len != fread(&student->first_name, sizeof(char), name_len, fptr))
    {
        printf("get first_name from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get first_name failed");
        }
        EXIT;
    }
    if (1 != fread(&last_name_len, sizeof(uint8_t), 1, fptr))
    {
        printf("get last_name size from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get last_name size failed");
        }
        EXIT;
    }
    if (last_name_len != fread(&student->last_name, sizeof(char), last_name_len, fptr))
    {
        printf("get last_name from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get last_name failed");
        }
        EXIT;
    }
    ReadGrades(&student->grades, fptr);
    if (fclose(fptr))
    {
        printf("closing file after get all grades failed");
        EXIT;
    }
}

void ReadGrades(grade_ty* grades, FILE* fptr)
{
    ReadHumanicGrades(&grades->human_grades, fptr);
    ReadRealGrades(&grades->rl_grades, fptr);
    if (1 != fread(&grades->sport, sizeof(float), 1, fptr))
    {
        printf("get sport grade from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get sport grade failed");
        }
        EXIT;
    }
}

void ReadHumanicGrades(human_grade_ty* grades, FILE* fptr)
{
    if (1 != fread(&grades->sociology, sizeof(float), 1, fptr))
    {
        printf("get sociology grade from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get sociology grade failed");
        }
        EXIT;
    }
    if (1 != fread(&grades->psychology, sizeof(float), 1, fptr))
    {
        printf("get psychology grade from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get psychology grade failed");
        }
        EXIT;
    }
}

void ReadRealGrades(rl_grade_ty* grades, FILE* fptr)
{
    if (1 != fread(&grades->phisics, sizeof(float), 1, fptr))
    {
        printf("get phisics grade from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get phisics grade failed");
        }
        EXIT;
    }
    if (1 != fread(&grades->math, sizeof(float), 1, fptr))
    {
        printf("get math grade from file failed");
        if (fclose(fptr))
        {
            printf("closing file failed after get math grade failed");
        }
        EXIT;
    }
}
