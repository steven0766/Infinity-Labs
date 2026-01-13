#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free */

#include "scheduler.h"

typedef struct
{
    uid_ty remove_task;
} op_param;

typedef struct
{
    int* runs;
    int repeat;
    scheduler_ty* sch;
    uid_ty victim;
} task_s_ty;


static void TestSchedulerCreate(void);
static void TestSchedulerAddAndRemoveTask(void);
static void TestSchedulerRun(void);

static sch_op_status_ty DummyTask(void* param);
static void Cleanup(void* param);
static sch_op_status_ty Task1(void* param);
static sch_op_status_ty Task2(void* param);
static sch_op_status_ty Task3(void* param);
static sch_op_status_ty Task4(void* param);
static sch_op_status_ty Task5(void* param);
static void Print(char* func, char* problem);

int main()
{
    TestSchedulerCreate();
    TestSchedulerAddAndRemoveTask();
    TestSchedulerRun();
    
    return 0;
}

static void TestSchedulerCreate(void)
{
    scheduler_ty* sch = SchedulerCreate();
    
    if (NULL == sch)
    {
        Print("TestSchedulerCreate", "SchedulerCreate");
    }
    
    if (0 != SchedulerSize(sch))
    {
        Print("TestSchedulerCreate", "SchedulerSize");
    }
    
    SchedulerDestroy(sch);
}

static void TestSchedulerAddAndRemoveTask(void)
{
    scheduler_ty* sch = SchedulerCreate();
    uid_ty t1, t2;
    task_s_ty* ctx1 = malloc(sizeof(task_s_ty) * sizeof(char));
    task_s_ty* ctx2 = malloc(sizeof(task_s_ty) * sizeof(char));

    ctx1->runs = 0;
    ctx1->repeat = 0;
    ctx1->sch = sch;

    ctx2->runs = 0;
    ctx2->repeat = 0;
    ctx2->sch = sch;

    t1 = SchedulerAddTask(sch, 1, DummyTask, ctx1, Cleanup, ctx1);
    t2 = SchedulerAddTask(sch, 1, DummyTask, ctx2, Cleanup, ctx2);

    if (2 != SchedulerSize(sch))
    {
        Print("AddTask", "size != 2");
    }

    if (0 != SchedulerRemoveTask(sch, t1))
    {
        Print("RemoveTask", "failed remove");
    }

    if (1 != SchedulerSize(sch))
    {
        Print("RemoveTask", "size != 1 after remove");
    }

    if (0 != SchedulerRemoveTask(sch, t2))
    {
        Print("RemoveTask", "failed remove second");
    }

    if (0 != SchedulerSize(sch))
    {
        Print("RemoveTask", "size != 0 after second remove");
    }

    SchedulerDestroy(sch);
}

static void TestSchedulerRun(void)
{
    scheduler_ty* scheduler = SchedulerCreate();

    int runs_remove_last = 0;
    int runs_simple_once = 0;
    int runs_remove_first = 0;
    int runs_repeating_twice = 0;
    int runs_should_be_removed = 0;
    
    sch_run_status_ty result = 0;
    
    uid_ty task_remove_last_id = invalid_uid_g;
    uid_ty task_simple_once_id = invalid_uid_g;
    uid_ty task_remove_first_id = invalid_uid_g;
    uid_ty task_repeating_twice_id = invalid_uid_g;
    uid_ty task_should_be_removed_id = invalid_uid_g;

    task_s_ty* task_remove_last = (task_s_ty*)malloc(sizeof(task_s_ty) * sizeof(char));
    task_s_ty* task_simple_once = (task_s_ty*)malloc(sizeof(task_s_ty) * sizeof(char));
    task_s_ty* task_remove_first = (task_s_ty*)malloc(sizeof(task_s_ty) * sizeof(char));
    task_s_ty* task_repeating_twice = (task_s_ty*)malloc(sizeof(task_s_ty) * sizeof(char));
    task_s_ty* task_should_be_removed = (task_s_ty*)malloc(sizeof(task_s_ty) * sizeof(char));

    task_remove_last->runs = &runs_remove_last;
    task_remove_last->repeat = 0;
    task_remove_last->sch = scheduler;

    task_simple_once->runs = &runs_simple_once;
    task_simple_once->repeat = 0;
    task_simple_once->sch = scheduler;

    task_remove_first->runs = &runs_remove_first;
    task_remove_first->repeat = 0;
    task_remove_first->sch = scheduler;

    task_repeating_twice->runs = &runs_repeating_twice;
    task_repeating_twice->repeat = 1;
    task_repeating_twice->sch = scheduler;

    task_should_be_removed->runs = &runs_should_be_removed;
    task_should_be_removed->repeat = 0;
    task_should_be_removed->sch = scheduler;

    task_remove_last_id =
        SchedulerAddTask(scheduler, 1, Task1, task_remove_last, Cleanup, task_remove_last);

    task_simple_once_id =
        SchedulerAddTask(scheduler, 3, Task2, task_simple_once, Cleanup, task_simple_once);

    task_remove_first_id =
        SchedulerAddTask(scheduler, 5, Task3, task_remove_first, Cleanup, task_remove_first);

    task_repeating_twice_id =
        SchedulerAddTask(scheduler, 7, Task4, task_repeating_twice, Cleanup, task_repeating_twice);

    task_should_be_removed_id =
        SchedulerAddTask(scheduler, 9, Task5, task_should_be_removed, Cleanup, task_should_be_removed);

    task_remove_last->victim = task_should_be_removed_id;
    task_remove_first->victim = task_remove_last_id;
    result = SchedulerRun(scheduler);

    if (SCH_SUCCESS != result)
    {
        Print("SchedulerRun", "did not finish with SCH_SUCCESS");
    }

    if (1 != runs_remove_last)
    {
        Print("Task1", "run count mismatch (expected 1)");
    }

    if (1 != runs_simple_once)
    {
        Print("Task2", "run count mismatch (expected 1)");
    }

    if (1 != runs_remove_first)
    {
        Print("Task3", "run count mismatch (expected 1)");
    }

    if (2 != runs_repeating_twice)
    {
        Print("Task4", "run count mismatch (expected 2)");
    }

    if (1 < runs_should_be_removed)
    {
        Print("Task5", "was not removed in time");
    }

    if (0 != SchedulerSize(scheduler))
    {
        Print("SchedulerRun", "queue should be empty after run");
    }

    SchedulerDestroy(scheduler);
}


static void Cleanup(void* param)
{
    free(param);
}

static sch_op_status_ty DummyTask(void* param)
{
    task_s_ty* c = param;
    (*c->runs)++;
    return SCH_NOT_REPEAT;
}

static sch_op_status_ty Task1(void* param)
{
    task_s_ty* c = param;
    (*c->runs)++;
    SchedulerRemoveTask(c->sch, c->victim);
    return SCH_NOT_REPEAT;
}

static sch_op_status_ty Task2(void* param)
{
    task_s_ty* c = param;
    (*c->runs)++;
    return SCH_NOT_REPEAT;
}

static sch_op_status_ty Task3(void* param)
{
    task_s_ty* c = param;
    (*c->runs)++;
    SchedulerRemoveTask(c->sch, c->victim);
    return SCH_NOT_REPEAT;
}

static sch_op_status_ty Task4(void* param)
{
    task_s_ty* c = param;
    (*c->runs)++;

    if (c->repeat-- > 0)
        return SCH_REPEAT;

    return SCH_NOT_REPEAT;
}

static sch_op_status_ty Task5(void* param)
{
    task_s_ty* c = param;
    c->runs++;
    return SCH_NOT_REPEAT;
}

static void Print(char* func, char* problem)
{
    printf("houston we have a problem in \"%s\" function!\n"
           "problem in:  %s\n", func, problem);
}
