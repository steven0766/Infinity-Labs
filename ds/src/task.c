/***********************************************************************
*File_name: task_test.c
*Reviewer: Rozalin
*Version:  1.0
*Date:	   6.1.26
************************************************************************/

#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */

#include "uid.h"
#include "task.h"

struct task
{
    uid_ty uid;
    task_op_ty op;
    void* op_param;
    task_cleanup_ty cl;
    void* cl_param;
    unsigned int interval;
    time_t time_to_run;
};


task_ty* TaskCreate( task_op_ty op, void* op_param,
                     task_cleanup_ty cl, void* cl_param,
                     unsigned int interval          )
{
    task_ty* task = {0};
    time_t cur_time = 0;

    assert(NULL != op);
    assert(NULL != cl);
    
    task = (task_ty*)malloc(sizeof(task_ty) * sizeof(char));
    
    if (NULL == task)
    {
        return NULL;
    }
    
    cur_time = time(NULL);
    
    if ((time_t)(-1) == cur_time)
    {
        free(task);
        task = NULL;
        
        return NULL;
    }

    task->uid = UidCreate();
    
    if (IsMatchId(task->uid, invalid_uid_g))
    {
        free(task);
        task = NULL;
        
        return NULL;
    }
    
    task->op = op;
    task->op_param = op_param;
    task->cl = cl;
    task->cl_param = cl_param;
    task->interval = interval;
    task->time_to_run = cur_time + interval;

    return task;
}

void TaskDestroy(task_ty* tsk)
{
    if (NULL == tsk)
    {
        return;
    }

    tsk->cl(tsk->cl_param);
    free(tsk);
}

uid_ty TaskGetUID(const task_ty* tsk)
{
    assert(NULL != tsk);
    return tsk->uid;
}


op_status_ty TaskExecute(task_ty* tsk)
{
    assert(NULL != tsk);
    return tsk->op(tsk->op_param);
}


time_t TaskGetTimeToRun(const task_ty* tsk)
{
    assert(NULL != tsk);
    return tsk->time_to_run;
}

void TaskUpdateTimeToRun(task_ty* tsk)
{
    assert(NULL != tsk);
    tsk->time_to_run += tsk->interval;
}

bool_ty TaskIsMatch(const task_ty* tsk1, const task_ty* tsk2)
{
    assert(NULL != tsk1);
    assert(NULL != tsk2);
    return IsMatchId(tsk1->uid, tsk2->uid);
}
