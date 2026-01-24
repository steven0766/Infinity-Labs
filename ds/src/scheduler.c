/***********************************************************************
*File_name: scheduler.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   14.01.26
************************************************************************/

#include <unistd.h> /* sleep */
#include <assert.h>	/* assert */
#include <stdlib.h> /* malloc, free */

#include "scheduler.h"
#include "task.h"
#include "uid.h"
#include "pqueue.h" 

typedef int (*ismatchid_ty)(uid_ty id_1, uid_ty id_2);


struct scheduler
{
    pq_ty* p_queue;
    sch_run_status_ty run_status;
    uid_ty curr_uid;
    int skip_reschedule;
    int is_running;
};

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

    return (int)difftime(time1, time2);
}

scheduler_ty* SchedulerCreate(void)
{
    scheduler_ty* sch = (scheduler_ty*)malloc(sizeof(scheduler_ty) * sizeof(char));
    if (NULL == sch)
    {
        return NULL;
    }
    
    sch->p_queue = PQCreate(CompareTask, NULL);
    if (NULL == sch->p_queue)
    {
        free(sch);
        sch = NULL;
        return NULL;
    }
    
    sch->run_status = SCH_STOP;
    sch->curr_uid = invalid_uid_g;
    sch->skip_reschedule = 0;
    sch->is_running = 0;
    
    return sch;
}

void SchedulerDestroy(scheduler_ty* sch)
{
    if (NULL == sch)
    {
        return;
    }
    SchedulerClear(sch);
    PQDestroy(sch->p_queue);
	
    free(sch);
    sch = NULL;
}

uid_ty SchedulerAddTask(scheduler_ty* sch, unsigned int interval,
                         scheduler_op_ty op_func, void* op_param,
                         scheduler_cleanup_ty clean_func, void* clean_param )
{
    task_ty* task = NULL;

    assert(sch);
    assert(op_func);

    task = TaskCreate((task_op_ty)op_func, op_param,
                      clean_func, clean_param, interval);

    if (NULL == task)
    {
        return invalid_uid_g;
    }

    if (FAIL == PQEnqueue(sch->p_queue, task))
    {
        TaskDestroy(task);
        return invalid_uid_g;
    }

    return TaskGetUID(task);
}

static int IsMatchTask(const void* task, void* task_uid)
{
	return IsMatchId(TaskGetUID((task_ty*)task), *(uid_ty*)task_uid);
}

int SchedulerRemoveTask(scheduler_ty* sch, uid_ty task_id)
{
    task_ty* task = NULL;
	
    assert(NULL != sch);
    
    if (IsMatchId(sch->curr_uid,task_id))
    {
        sch->skip_reschedule = 1;
        return 0;
    }

    task = PQRemove(sch->p_queue, IsMatchTask, &task_id);
    if (NULL == task)
    {
        return 1;
    }	
    
    TaskDestroy(task);
    return 0;
}

size_t SchedulerSize(const scheduler_ty* sch)
{
    assert(NULL != sch);
    return PQCount(sch->p_queue) + sch->is_running;
}

sch_run_status_ty SchedulerRun(scheduler_ty* sch)
{
    task_ty* curr_task = NULL;
    time_t curr_time = 0;
    op_status_ty task_status = 0;
    time_t diff = 0;

    assert(NULL != sch);
	
	sch->run_status = SCH_SUCCESS;
    sch->is_running = 1;
    sch->curr_uid = invalid_uid_g;

    while (sch->is_running && !PQIsEmpty(sch->p_queue))
    {
        curr_task = PQDequeue(sch->p_queue);
        sch->curr_uid = TaskGetUID(curr_task);

        curr_time = time(NULL);
        if ((time_t)(-1) == curr_time)
        {
            TaskDestroy(curr_task);
            sch->run_status = SCH_FAIL;
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
        
        if (SCH_REPEAT == task_status && !sch->skip_reschedule)
        {
        	TaskUpdateTimeToRun(curr_task);
            if (FAIL == PQEnqueue(sch->p_queue, curr_task))
            {
                TaskDestroy(curr_task);
                sch->run_status = SCH_FAIL;
                break;
            }
        }
        else
        {
            TaskDestroy(curr_task);
        }

        sch->curr_uid = invalid_uid_g;
        sch->skip_reschedule = 0;
    }
	
	sch->is_running = 0;
    return sch->run_status;
}

void SchedulerStop(scheduler_ty* sch)
{
    assert(NULL != sch);
    sch->is_running = 0;
    sch->run_status = SCH_STOP;
}

void SchedulerClear(scheduler_ty* sch)
{
    task_ty* t = NULL;
    assert(NULL != sch);
    
    while (!PQIsEmpty(sch->p_queue))
    {
        t = PQDequeue(sch->p_queue);
        TaskDestroy(t);
    }
    
    if (1 == sch->is_running)
    {
        sch->skip_reschedule = 1;
    }
}
