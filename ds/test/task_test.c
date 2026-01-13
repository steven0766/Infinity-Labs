/***********************************************************************
*File_name: task_test.c
*Reviewer: Rozalin
*Version:  1.0
*Date:	   6.1.26
************************************************************************/

#include <stdio.h>   /* printf */
#include <stdlib.h>  /* malloc, free */

#include "task.h"

typedef struct
{
    int repeat;
    int success;
    char* msg;
} op_param;

static void TaskCreateTest(void);
static void TaskExecuteTest(void);
static void TaskRepeatLogicTest(void);
static void TaskUpdateTimeTest(void);
static void TaskIsMatchTest(void);

static task_ty* CreateDefaultTask(op_param** out_param);
static void Print(char* func, char* problem);

static op_status_ty Task(void* param);
static void Cleanup(void* param);

int main()
{
    TaskCreateTest();
    TaskExecuteTest();
    TaskRepeatLogicTest();
    TaskUpdateTimeTest();
    TaskIsMatchTest();

    return 0;
}

static void TaskCreateTest(void)
{
    task_ty* task = NULL;
    op_param* param = NULL;

    task = CreateDefaultTask(&param);
    if (NULL == task)
    {
        Print("TaskCreateTest", "CreateDefaultTask");
        return;
    }

    TaskDestroy(task);
}

static void TaskExecuteTest(void)
{
    task_ty* task = NULL;
    op_param* param = NULL;

    task = CreateDefaultTask(&param);
    if (NULL == task)
    {
        Print("TaskExecuteTest", "CreateDefaultTask");
        return;
    }

    TaskExecute(task);

    if (param->success != 1)
    {
        Print("TaskExecuteTest", "Task execution");
    }

    TaskDestroy(task);
}

static void TaskRepeatLogicTest(void)
{
    task_ty* task = NULL;
    op_param* param = NULL;

    task = CreateDefaultTask(&param);
    if (NULL == task)
    {
        Print("TaskRepeatLogicTest", "CreateDefaultTask");
        return;
    }

    if (TaskExecute(task) != REPEAT)
    {
        Print("TaskRepeatLogicTest", "expected REPEAT");
    }

    if (TaskExecute(task) != REPEAT)
    {
        Print("TaskRepeatLogicTest", "expected REPEAT second time");
    }

    if (TaskExecute(task) != NOT_REPEAT)
    {
        Print("TaskRepeatLogicTest", "expected NOT_REPEAT");
    }

    TaskDestroy(task);
}

static void TaskUpdateTimeTest(void)
{
    task_ty* task = NULL;
    op_param* param = NULL;
    time_t t1 = 0;
    time_t t2 = 0;

    task = CreateDefaultTask(&param);
    if (NULL == task)
    {
        Print("TaskUpdateTimeTest", "CreateDefaultTask");
        return;
    }

    t1 = TaskGetTimeToRun(task);
    TaskUpdateTimeToRun(task);
    t2 = TaskGetTimeToRun(task);

    if (t2 <= t1)
    {
        Print("TaskUpdateTimeTest", "time_to_run");
    }

    TaskDestroy(task);
}

static void TaskIsMatchTest(void)
{
    task_ty* task1 = NULL;
    task_ty* task2 = NULL;
    op_param* param1 = NULL;
    op_param* param2 = NULL;

    task1 = CreateDefaultTask(&param1);
    if (NULL == task1)
    {
        Print("TaskIsMatchTest", "CreateDefaultTask task1");
        return;
    }

    task2 = CreateDefaultTask(&param2);
    if (NULL == task2)
    {
        TaskDestroy(task1);
        Print("TaskIsMatchTest", "CreateDefaultTask task2");
        return;
    }

    if (FALSE == TaskIsMatch(task1, task1))
    {
        Print("TaskIsMatchTest", "same task should match");
    }

    if (TRUE == TaskIsMatch(task1, task2))
    {
        Print("TaskIsMatchTest", "different tasks shouldn't match");
    }

    TaskDestroy(task1);
    TaskDestroy(task2);
}

static task_ty* CreateDefaultTask(op_param** out_param)
{
    unsigned int interval = 5;
    op_param* param = NULL;
    task_ty* task = NULL;

    param = malloc(sizeof(op_param));
    if (NULL == param)
    {
        Print("CreateDefaultTask", "malloc failed");
        return NULL;
    }

    param->repeat = 2;
    param->success = 0;
    param->msg = "hello\n";

    task = TaskCreate(Task, param, Cleanup, param, interval);
    if (NULL == task)
    {
        free(param);
        Print("CreateDefaultTask", "TaskCreate failed");
        return NULL;
    }

    *out_param = param;
    return task;
}

static op_status_ty Task(void* param)
{
    op_param* par = (op_param*)param;

    par->success++;

    if (par->repeat > 0)
    {
        --par->repeat;
        return REPEAT;
    }

    return NOT_REPEAT;
}

static void Cleanup(void* param)
{
    free(param);
}

static void Print(char* func, char* problem)
{
    printf("houston we have a problem in \"%s\" function!\n"
           "problem in:  %s\n", func, problem);
}

