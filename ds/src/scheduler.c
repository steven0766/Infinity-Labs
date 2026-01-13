#include <unistd.h> /* sleep */
#include <assert.h>	/* assert */
#include <stdlib.h> /* malloc, free */

#include "scheduler.h"
#include "task.h"
#include "uid.h"
#include "pqueue.h" 

typedef int (*ismatchid_ty)(uid_ty id_1, uid_ty id_2);

typedef enum which_op_status
{
	PASS = 0,
	REMOVE = 1,
	CLEAN = 2 
	 
}which_op_status_ty;

struct scheduler
{
    pq_ty* p_queue;
    sch_run_status_ty is_run;
    uid_ty curr_uid;
    which_op_status_ty which_op; 
    
};

typedef struct wrapper
{
    scheduler_op_ty op;
    void *op_param;
    scheduler_cleanup_ty clean;
    void *clean_param;
} op_wrapper_ty;


typedef struct match_wrapper
{
    ismatchid_ty match;
    void *param;
} match_wrapper_ty;

static int CompareTask(const void* task1, const void* task2, void* param)
{
    const task_ty* t1 = (const task_ty*)task1;
    const task_ty* t2 = (const task_ty*)task2;
    time_t time1 = 0;
    time_t time2 = 0;

    (void)param;
	
    assert(NULL != t1); 
    assert(NULL != t2); 
	
    time1 = TaskGetTimeToRun(t1);
    time2 = TaskGetTimeToRun(t2);

    if (time1 < time2)
    {
        return -1;
    }
    else if (time1 > time2)
    {
        return 1;
    }

    return 0;
}

scheduler_ty* SchedulerCreate(void)
{
	/* 1: allocate scheduler */
		/* 1.1: check if NULL return NULL */
    scheduler_ty* sch = (scheduler_ty*)malloc(sizeof(scheduler_ty) * sizeof(char));
    if (NULL == sch)
    {
        return NULL;
    }
	/* 2: creaute queue: scheduler->p_queue QueueCreate(CompareTask, NULL) */
		/* 2.1: check if NULL*/
		/* 2.1.1: Destroy scheduler, return NULL */
    sch->p_queue = PQCreate(CompareTask, NULL);
    if (NULL == sch->p_queue)
    {
        free(sch);
        sch = NULL;
        return NULL;
    }
		
	/* 3: Init 
			is_run = STOP,
	 		curr_uid = invalid uid,
	  		which_op = PASS */ 
    sch->is_run = SCH_STOP;
    sch->curr_uid = invalid_uid_g;
    sch->which_op = PASS;
    
    return sch;
    
	/* 4: return scheduler */
}

void SchedulerDestroy(scheduler_ty* sch)
{
    if (NULL == sch)
    {
        return;
    }
	/* 1: if sch is null return */
    SchedulerClear(sch);
    PQDestroy(sch->p_queue);
	
	/* 2:call P_queue destory (p_queue) */
	
    free(sch);
    sch = NULL;
	
	/* 3: free scheduler */
}

static op_status_ty OpAdapter(void *data)
{
    op_wrapper_ty* wrap = (op_wrapper_ty*)data;
    sch_op_status_ty status = wrap->op(wrap->op_param);

    if (status == SCH_REPEAT)
    {
        return REPEAT;
    }

    return NOT_REPEAT;
}

static void SchedulerCleanupWrapper(void *wrapper_param)
{
    op_wrapper_ty* wrap = wrapper_param;

    if (NULL == wrap)
    {
        return;
    }

    if (NULL != wrap->clean)
    {
        wrap->clean(wrap->clean_param);
    }

    free(wrap);
}


uid_ty SchedulerAddTask(scheduler_ty* sch, unsigned int interval,
                         scheduler_op_ty op_func, void* op_param,
                         scheduler_cleanup_ty clean_func, void* clean_param )
{
    op_wrapper_ty* wrap = NULL;
    task_ty* task = NULL;

    assert(sch);
    assert(op_func);
    assert(clean_func);

    wrap = (op_wrapper_ty*)malloc(sizeof(op_wrapper_ty) * sizeof(char));
    if (NULL == wrap)
    {
        return invalid_uid_g;
    }

    wrap->op = op_func;
    wrap->op_param = op_param;
    wrap->clean = clean_func;
    wrap->clean_param = clean_param;

    task = TaskCreate(OpAdapter, wrap,
                  SchedulerCleanupWrapper, wrap,
                  interval);

    if (NULL == task)
    {
        free(wrap);
        return invalid_uid_g;
    }

    if (FAIL == PQEnqueue(sch->p_queue, task))
    {
        TaskDestroy(task);
        return invalid_uid_g;
    }

    return TaskGetUID(task);
}

static int IsMatchWrapper(const void* data, void* param)
{
    const match_wrapper_ty* match_wrap = (const match_wrapper_ty*)param;
    const task_ty* task = (const task_ty*)data;

    uid_ty task_uid = TaskGetUID(task);
    uid_ty target_uid = *(uid_ty*)match_wrap->param;

    return match_wrap->match(task_uid, target_uid);
}

int SchedulerRemoveTask(scheduler_ty* sch, uid_ty task_id)
{
    match_wrapper_ty* match_wrap = NULL;
    task_ty* task = NULL;
	
    assert(NULL != sch);
    assert(!IsMatchId(invalid_uid_g, task_id));
    
    if (IsMatchId(sch->curr_uid,task_id))
    {
        sch->which_op = REMOVE;
        return 0;
    }
    
    match_wrap = (match_wrapper_ty*)malloc(sizeof(match_wrapper_ty) * sizeof(char));
    
    if (NULL == match_wrap)
    {
        return 1;
    }
    
    match_wrap->match = IsMatchId;
    match_wrap->param = &task_id;
    task = PQRemove(sch->p_queue, IsMatchWrapper, match_wrap);
    if (NULL == task)
    {
        free(match_wrap);
        return 1;
    }	
    
    TaskDestroy(task);
    free(match_wrap);
    return 0;
        /* 1: assert sch, task_id is invalid 
	 2: if "uid.h"(is_match) of task_id and sch->uid
	    2.1: if true which_op = 1 return 
	 3: call PQremove(sch->PQ, "uid.h" is_match from , task_uid)
	 4: return if PQRmove is NULL return 1 else 0 destroy task*/
}

size_t SchedulerSize(const scheduler_ty* sch)
{
    assert(NULL != sch);
    return PQCount(sch->p_queue);
	/* assert sch is NULL */ 
	/* 1: return PQCount(sch->p_queue) */
}

sch_run_status_ty SchedulerRun(scheduler_ty* sch)
{
    task_ty* curr_task = NULL;
    time_t curr_time = 0;
    op_status_ty task_status = 0;
    time_t diff = 0;

    assert(NULL != sch);

    sch->is_run = SCH_SUCCESS;
    sch->which_op = PASS;
    sch->curr_uid = invalid_uid_g;

    while (!PQIsEmpty(sch->p_queue) && SCH_STOP != sch->is_run)
    {
        curr_task = PQDequeue(sch->p_queue);
        sch->curr_uid = TaskGetUID(curr_task);

        curr_time = time(NULL);
        if ((time_t)(-1) == curr_time)
        {
            TaskDestroy(curr_task);
            sch->is_run = SCH_FAIL;
            break;
        }
        
        diff = TaskGetTimeToRun(curr_task) - curr_time;
        
        if (diff > 0)
        {
            while (diff > 0)
            {
                diff = sleep(diff);
            }
        }
        task_status = TaskExecute(curr_task);

        switch (sch->which_op)
        {
            case PASS:
                if (REPEAT == task_status)
                {
                    TaskUpdateTimeToRun(curr_task);
                    if (FAIL == PQEnqueue(sch->p_queue, curr_task))
                    {
                        TaskDestroy(curr_task);
                        sch->is_run = SCH_FAIL;
                    }
                }
                else
                {
                    TaskDestroy(curr_task);
                }
                break;

            case REMOVE:
                TaskDestroy(curr_task);
                sch->which_op = PASS;
                break;

            case CLEAN:
                sch->is_run = SCH_STOP;
                TaskDestroy(curr_task);
                while (!PQIsEmpty(sch->p_queue))
                {
                    task_ty* t = PQDequeue(sch->p_queue);
                    TaskDestroy(t);
                }
                sch->which_op = PASS;
                break;
        }

        sch->curr_uid = invalid_uid_g;
    }

    return sch->is_run;
	/* 
		0.1: assert NULL != sch
		0.2 create pointer curr task
		0.3 creat curr time
			0.3.1 if time is fail return status to run = FAIL 
		0.4 create task status
		
		1. set scheduler status to run = SCH_SUCCESS
		
		2. while queue is not empty and status is run:
		
			2.2 curr task = dequeue
			2.3 get task time to run 
			2.4 if time to run > curr time: sleep(time to run - curr time)
			
			2.4 update sch->uid to curr task uid
			2.5 call to task operate and save task status
			 
			2.6 switch on sch->which_op:
				case PASS:
					2.6.1: if task status = repeat, TaskUpdateTimeToRun and PQEnqueu
						2.6.1.1 if PQEnqueue is fail, status to run = FAIL
					2.6.2: else, if not repeat destroy task
				case REMOVE:
					2.6.3: destroy task
					2.6.3 sch->which_op = PASS
				case CLEAN:
					2.6.5: while !PQIsEmpty -> PQDequeue and DestroyTask
					2.6.6: sch->which_op = PASS
					2.6.7: status to run = STOP
		3. return scheduler status
	*/

}

void SchedulerStop(scheduler_ty* sch)
{
    assert(NULL != sch);
    sch->is_run = SCH_STOP;
	/*
		assert sch 
		1: update scheduler status to stop
	*/
}

void SchedulerClear(scheduler_ty* sch)
{
	/*
		assert sch
		if status sch is run 
		sch->which_op = CLEAN return 
		1: while queue is not empty dequeue and call destroy task 	
	*/
    task_ty* t = NULL;
    assert(NULL != sch);
    
    if (SCH_SUCCESS == sch->is_run)
    {
        sch->which_op = CLEAN;
        return;
    }
    
    while (!PQIsEmpty(sch->p_queue))
    {
        t = PQDequeue(sch->p_queue);
        TaskDestroy(t);
    }
}

