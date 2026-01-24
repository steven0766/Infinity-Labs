#include <stdlib.h>		/* free, malloc */

#include "scheduler.h"
#include "test_utils.h"

#define BASE_CASES_LEN		3
#define FIRST_TASK_IDX  	0
#define MIDDLE_TASK_IDX 	1
#define LAST_TASK_IDX   	2
#define TASK_INTERVAL   	5
#define SHORT_INTERVAL		1	
#define EXEC_ORDER_TEST_LEN 	15
#define UNUSED(var)		((void)(var))

static void TestSchedulerCreate(void);
static void TestSchedulerAddTask(void);
static void TestSchedulerRemoveTask(void);
static void TestSchedulerRun(void);
static void TestSchedulerRunExecutionOrder(void);
static void TestSchedulerRunOperations(void);
static void TestSchedulerRunEmpty(void);

static sch_op_status_ty IncrementOp(void* param);
static void IncrementCleanup(void* param);

static sch_op_status_ty RemoveOrderOp(void* param);
static void RemoveOrderCleanup(void* param);

static sch_op_status_ty StopSchedulerOp(void* param);
static void NoCleanup(void* param);

static sch_op_status_ty ExecOrderOp(void* param);
static sch_op_status_ty AddTaskOp(void* param);
static sch_op_status_ty RemoveTaskOp(void* param);
static sch_op_status_ty SizeTaskOp(void* param);
static sch_op_status_ty ClearTaskOp(void* param);

typedef struct remove_order_param
{
	uid_ty* uids;
	uid_ty task_id;
	size_t* idx;
	
}remove_order_param_ty;

typedef struct exec_order_param
{
	int* actual;
	size_t* idx;
	int data_to_write;
	scheduler_ty* sch;
}exec_order_param_ty;

typedef struct add_task_param
{
	scheduler_ty* sch;
	unsigned int interval;
	scheduler_op_ty op_func;
	void* op_param;
	scheduler_cleanup_ty clean_func;
	void* clean_param;
	uid_ty* new_task_id;	
}add_task_param_ty;

typedef struct remove_task_param
{
	scheduler_ty* sch;
	int* is_removed;
	uid_ty* to_remove;	
}remove_task_param_ty;

typedef struct is_empty_task_param
{
	scheduler_ty* sch;
	int* is_empty;	
}is_empty_task_param_ty;

typedef struct size_task_param
{
	scheduler_ty* sch;
	size_t* size;	
}size_task_param_ty;

int main()
{
	TestSuiteBegin("Scheduler");
	
	TestPrintSection("Creation Tests");
	TestSchedulerCreate();
	
	TestPrintSection("AddTask Tests");
	TestSchedulerAddTask();
	
	TestPrintSection("RemoveTask Tests");
	TestSchedulerRemoveTask();
	
	TestPrintSection("SchedulerRun Tests");
	TestSchedulerRun();
	
	TestPrintSection("SchedulerRunEmpty Tests");
	TestSchedulerRunEmpty();                                      
	
	TestPrintSection("SchedulerRunExecutionOrder Tests");
	TestSchedulerRunExecutionOrder();
	
	TestPrintSection("SchedulerRunOperations Tests");
	TestSchedulerRunOperations();
	
	return TestSuiteEnd();

}

static void TestSchedulerCreate(void)
{
	scheduler_ty* sch = SchedulerCreate();
	if (!ASSERT_NOT_NULL(sch))
	{
		return;
	}
	
	ASSERT_SIZE(0, SchedulerSize(sch)); 

	SchedulerDestroy(sch);
}

static void TestSchedulerAddTask(void)
{
	uid_ty uid = {0};
	scheduler_ty* sch = NULL;
	
	int* counter_param = (int*)malloc(sizeof(*counter_param));
	if (!ASSERT_NOT_NULL(counter_param))
	{
		return;
	}
	*counter_param = 0;
	
	sch = SchedulerCreate();
	if (!ASSERT_NOT_NULL(sch))
	{
		return;
	}
	
	uid = SchedulerAddTask(sch, TASK_INTERVAL, IncrementOp, counter_param, 
					IncrementCleanup, counter_param);
	
	ASSERT_FALSE(IsMatchId(invalid_uid_g, uid));
	ASSERT_SIZE(1, SchedulerSize(sch));
	
	SchedulerDestroy(sch);
}

static void TestSchedulerRemoveTask(void)
{
	remove_order_param_ty* params[BASE_CASES_LEN] = {0};
	uid_ty uids[BASE_CASES_LEN] = {0};
	uid_ty actual_order[BASE_CASES_LEN] = {0};
	uid_ty expected_order[BASE_CASES_LEN] = {0};
	
	size_t i = 0;
	size_t shared_idx = 0;
	
	scheduler_ty* sch = SchedulerCreate();
	if (!ASSERT_NOT_NULL(sch))
	{
		return;
	}
	
	/* initialize params */
	for (i = 0; i < BASE_CASES_LEN; ++i)
	{
		params[i] = (remove_order_param_ty*)malloc(sizeof(*params[i]));
		if (!ASSERT_NOT_NULL(params[i]))
        	{
        		while (i > 0)
        		{
        			free(params[--i]);
        		}
        		SchedulerDestroy(sch);
            		return;
		}
		
		params[i]->task_id = invalid_uid_g;
		params[i]->uids = actual_order;
		params[i]->idx = &shared_idx;
		
	}
	
	/* create tasks */
	for (i = 0; i < BASE_CASES_LEN; ++i)
	{
		uids[i] = SchedulerAddTask(sch, TASK_INTERVAL * (i + 1), 
						RemoveOrderOp, NULL, 
						RemoveOrderCleanup, params[i]);
		ASSERT_FALSE(IsMatchId(invalid_uid_g, uids[i]));
		params[i]->task_id = uids[i];
	}
	
	/* remove order: middle , last, first */
	expected_order[0] = uids[MIDDLE_TASK_IDX];
    	expected_order[1] = uids[LAST_TASK_IDX];
    	expected_order[2] = uids[FIRST_TASK_IDX];
	
	ASSERT_SIZE(BASE_CASES_LEN, SchedulerSize(sch));
	
	/* remove task in the middle */
	ASSERT_SUCCESS(SchedulerRemoveTask(sch, uids[MIDDLE_TASK_IDX]));
	ASSERT_SIZE(BASE_CASES_LEN - 1, SchedulerSize(sch));
	ASSERT_FAIL(SchedulerRemoveTask(sch, uids[MIDDLE_TASK_IDX]));
	
	/* remove last task */
	ASSERT_SUCCESS(SchedulerRemoveTask(sch, uids[LAST_TASK_IDX]));
	ASSERT_SIZE(BASE_CASES_LEN - 2, SchedulerSize(sch));
	ASSERT_FAIL(SchedulerRemoveTask(sch, uids[LAST_TASK_IDX]));
	
	/* remove first task */
	ASSERT_SUCCESS(SchedulerRemoveTask(sch, uids[FIRST_TASK_IDX]));
	ASSERT_SIZE(0, SchedulerSize(sch));
	
	/* verify remove order */
	for (i = 0; i < BASE_CASES_LEN; ++i)
	{
		ASSERT_TRUE(IsMatchId(expected_order[i], actual_order[i]));
	}
	
	SchedulerDestroy(sch);
}

static void TestSchedulerRun(void)
{
	scheduler_ty* sch = NULL;
	int counter = 0;
	int* counter_param = &counter;
	
	/* create scheduler with one task to perform and one to stop */
	sch = SchedulerCreate();
	if (!ASSERT_NOT_NULL(sch))
	{
		return;
	}
	
	ASSERT_FALSE(IsMatchId(invalid_uid_g,  SchedulerAddTask(sch, SHORT_INTERVAL, IncrementOp, 
					counter_param, NoCleanup, 
								NULL)));
	ASSERT_FALSE(IsMatchId(invalid_uid_g,  SchedulerAddTask(sch, SHORT_INTERVAL, StopSchedulerOp, 
					sch, NoCleanup, 
								NULL)));
	
	/* run scheduler, validate task executed, incremented once and stopped */
	ASSERT_INT(SCH_STOP, SchedulerRun(sch));
	ASSERT_INT(1, *counter_param);
	/* 1 task left in the scheduler (increment) */
	ASSERT_SIZE(1, SchedulerSize(sch));
	
	SchedulerClear(sch);
	ASSERT_SIZE(0, SchedulerSize(sch));
	
	SchedulerDestroy(sch);
}

/**
* Test running an empty scheduler
* SchedulerRun returns SCH_SUCCESS immediately
*/
static void TestSchedulerRunEmpty(void)
{
	scheduler_ty* sch = SchedulerCreate();
	if (!ASSERT_NOT_NULL(sch))
	{
		return;
	}
	
	ASSERT_SIZE(0, SchedulerSize(sch));
	
	/* run empty scheduler - should return success immediately */
	ASSERT_INT(SCH_SUCCESS, SchedulerRun(sch));
	
	/* should still be empty after run */
	ASSERT_SIZE(0, SchedulerSize(sch));
	
	SchedulerDestroy(sch);
}

/**
* Test case: 
* 3 tasks to test execution order: 
* t1 write '1' in intervals of 1 second
* t2 write '2' in intervals of 3 seconds
* t3 write '3' in intervals of 5 seconds
* expected = [1, 1, 2, 1, 1, 3, 1, 2, 1, 1, 1, 2, 1, 3, 1]
*/
static void TestSchedulerRunExecutionOrder(void)
{
	/* create array for expected that all the tasks can write to */
	exec_order_param_ty* params[BASE_CASES_LEN] = {0};
	int expected[EXEC_ORDER_TEST_LEN] = {1, 1, 2, 1, 1, 3, 1, 2, 1, 1, 1, 2, 1, 3, 1};
	int actual[EXEC_ORDER_TEST_LEN] = {0};
	size_t shared_idx = 0;
	size_t i = 0;
	int interval = 1;
	
	uid_ty curr_id = invalid_uid_g;
	
	scheduler_ty* sch = SchedulerCreate();
	if (!ASSERT_NOT_NULL(sch))
	{
		return;
	}
	
	/* initialize params */
	for (i = 0; i < BASE_CASES_LEN; ++i)
	{
		params[i] = (exec_order_param_ty*)malloc(sizeof(*params[i]));
		if (!ASSERT_NOT_NULL(params[i]))
        	{
        		while (i > 0)
        		{
        			free(params[--i]);
        		}
        		SchedulerDestroy(sch);
            		return;
		}
		
		params[i]->actual = actual;
		params[i]->data_to_write = i + 1;
		params[i]->idx = &shared_idx;
		params[i]->sch = sch;
	}
	
	/* create tasks */
	for (i = 0; i < BASE_CASES_LEN; ++i)
	{
		curr_id = SchedulerAddTask(sch, interval, ExecOrderOp, 
					   params[i], NoCleanup, NULL);
		ASSERT_FALSE(IsMatchId(invalid_uid_g, curr_id));
		interval += 2;
	}
					   
	ASSERT_SIZE(BASE_CASES_LEN, SchedulerSize(sch));
	
	/* run and verify execution order*/
	ASSERT_INT(SCH_STOP, SchedulerRun(sch));
	
	ASSERT_INT_ARRAY(expected, actual, EXEC_ORDER_TEST_LEN);
	
	SchedulerDestroy(sch);	
	
	for (i = 0; i < BASE_CASES_LEN; ++i)
	{
		free(params[i]);
	}
}

/**
* Test scheduler methods while is running. 
* t1 - counter task
* t2 - task using AddTask method of scheduler , adding new counter task
* t3 - remove task - remove t1
* t4 - remove task - remove self (t4)
* t5 - task using IsEmpty - should be false
* t6 - task using Size - should return 7 after removal
* t7 - task using Clear - remove all tasks and return success
* t8 - task using Stop - should be removed before executed 
*/
static void TestSchedulerRunOperations(void)
{
	scheduler_ty* sch = NULL;
	uid_ty uid = invalid_uid_g;
	int counter = 0;
	int* counter_param = &counter;
	
	/* local vars for pointers */
	uid_ty new_task_id = invalid_uid_g;
	int is_removed_flag = 0;
	uid_ty to_remove = invalid_uid_g;
	int remove_self_flag = 0;
	uid_ty remove_self_id = invalid_uid_g;
	int is_empty = 0;
	size_t size = 0;
	
	/* params */
	add_task_param_ty add_task_param = {0};
	remove_task_param_ty remove_task_param = {0};
	remove_task_param_ty remove_self_param = {0};
	is_empty_task_param_ty is_empty_task_param = {0};
	size_task_param_ty size_task_param = {0};
	
	sch = SchedulerCreate();
	if (!ASSERT_NOT_NULL(sch))
	{
		return;
	}
	
	/* add counter task */
	uid = SchedulerAddTask(sch, SHORT_INTERVAL, IncrementOp, 
			       counter_param, NoCleanup, NULL);
	/* add AddTask */
	add_task_param.sch = sch;
	add_task_param.interval = SHORT_INTERVAL;
	add_task_param.op_func = IncrementOp;
	add_task_param.op_param = counter_param;
	add_task_param.clean_func = NoCleanup;
	add_task_param.clean_param = NULL;
	add_task_param.new_task_id = &new_task_id;
	
	SchedulerAddTask(sch, SHORT_INTERVAL, AddTaskOp, 
			 &add_task_param, NoCleanup, NULL);
	
	/* remove task */
	remove_task_param.sch = sch;
	remove_task_param.is_removed = &is_removed_flag;
	to_remove = uid;
	remove_task_param.to_remove = &to_remove;
	
	SchedulerAddTask(sch, SHORT_INTERVAL, RemoveTaskOp, 
			 &remove_task_param, NoCleanup, NULL);
	
	/* remove self */
	remove_self_param.sch = sch;
	remove_self_param.is_removed = &remove_self_flag;
	remove_self_param.to_remove = &remove_self_id;
	
	uid = SchedulerAddTask(sch, SHORT_INTERVAL, RemoveTaskOp, 
			       &remove_self_param, NoCleanup, NULL);	
	remove_self_id = uid;
	
	/* size task */
	size_task_param.sch = sch;
	size_task_param.size = &size;
	SchedulerAddTask(sch, SHORT_INTERVAL, SizeTaskOp, 
			 &size_task_param, NoCleanup, NULL);
			 
	/* clear task */
	SchedulerAddTask(sch, SHORT_INTERVAL, ClearTaskOp, 
			 sch, NoCleanup, NULL);
			 
	/* stop task */
	SchedulerAddTask(sch, SHORT_INTERVAL, StopSchedulerOp, 
			 sch, NoCleanup, NULL);
	
	/* verify operations */
	/* scheduler was cleared and return success */
	ASSERT_INT(SCH_SUCCESS, SchedulerRun(sch));
	/* counter param incremented only once */
	ASSERT_INT(1, counter);
	/* add task should return valid id */
	ASSERT_FALSE(IsMatchId(new_task_id, invalid_uid_g));
	/* remove task - verify success */
	ASSERT_SUCCESS(is_removed_flag);
	/* remove self - verify success */
	ASSERT_SUCCESS(remove_self_flag);
	/* is empty - should be false */
	ASSERT_FALSE(is_empty);		
	/* size - should be 7 */
	ASSERT_SIZE(6, size);		
	
	SchedulerDestroy(sch);
	
}

static sch_op_status_ty IncrementOp(void* param)
{
	int* counter = (int*)param;
	*counter += 1;

	return SCH_REPEAT;
}


static void IncrementCleanup(void* param)
{
	free(param);
}

static sch_op_status_ty RemoveOrderOp(void* param)
{
	UNUSED(param);
	
	return SCH_REPEAT;
}
static void RemoveOrderCleanup(void* param)
{
	remove_order_param_ty* removed_order_param = 
						(remove_order_param_ty*)param;
	
	*(removed_order_param->uids + *removed_order_param->idx) = 
						removed_order_param->task_id;
	*removed_order_param->idx += 1;
	
	free(removed_order_param);
}

static sch_op_status_ty StopSchedulerOp(void* param)
{
	SchedulerStop((scheduler_ty*)param);
	
	return SCH_NOT_REPEAT;
}

static void NoCleanup(void* param)
{
	UNUSED(param);
}

static sch_op_status_ty ExecOrderOp(void* param)
{
	exec_order_param_ty* exec_order = (exec_order_param_ty*)param;
	
	*(exec_order->actual + *exec_order->idx) = exec_order->data_to_write;
	*exec_order->idx += 1;

	if (*exec_order->idx >= EXEC_ORDER_TEST_LEN)
	{
		SchedulerStop(exec_order->sch);
		return SCH_NOT_REPEAT;
	}
		
	
	return SCH_REPEAT;
}

static sch_op_status_ty AddTaskOp(void* param)
{
	uid_ty new_task_id = invalid_uid_g;
	add_task_param_ty* add_tsk_prm = (add_task_param_ty*)param;

	new_task_id = SchedulerAddTask(add_tsk_prm->sch, add_tsk_prm->interval, 
			 add_tsk_prm->op_func, add_tsk_prm->op_param, 
			 add_tsk_prm->clean_func, add_tsk_prm->clean_param);
	*add_tsk_prm->new_task_id = new_task_id;
	
	return SCH_REPEAT;
}

static sch_op_status_ty RemoveTaskOp(void* param)
{
	remove_task_param_ty* rm_tsk_prm = (remove_task_param_ty*)param;
	
	int success = SchedulerRemoveTask(rm_tsk_prm->sch, 
					  *rm_tsk_prm->to_remove);
	*rm_tsk_prm->is_removed = success;
	
	return SCH_REPEAT;
}


static sch_op_status_ty SizeTaskOp(void* param)
{
	size_task_param_ty* s_tsk_prm = (size_task_param_ty*)param;
	
	size_t size = SchedulerSize(s_tsk_prm->sch);
	*s_tsk_prm->size = size;
	
	return SCH_REPEAT;
}

static sch_op_status_ty ClearTaskOp(void* param)
{
	SchedulerClear((scheduler_ty*)param);
	
	return SCH_REPEAT;
}
	

#if 0
/***********************************************************************
*File_name: fsa.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   14.01.26
************************************************************************/

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
#endif
