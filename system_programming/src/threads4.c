#include <pthread.h>    /* pthread_create, pthread_join */
#include <stdio.h>      /* printf */

#include "utils.h"

enum
{
    TARGET = 1000000000,
    NUM_THREADS = 7,
    RANGE = TARGET / NUM_THREADS
};

static void* GetSumOfDivisorsIMP(void* param_);
static size_t SumDividersIMP(size_t from_, size_t to_, size_t num_);

int main(void)
{
    pthread_t threads[NUM_THREADS] = {0};
    pthread_t* curr_thread = threads;
    pthread_t* end_threads = threads + NUM_THREADS;

    size_t sum_of_divisors = 0;

    int status = 0;
    void* ret_val = NULL;
    size_t i = 0;
    
    time_t start = 0;
    time_t end = 0;

    start = time(NULL);

    for (i = 0; i < NUM_THREADS; ++i)
    {
        status = pthread_create(curr_thread, NULL, GetSumOfDivisorsIMP, (void*)i);
        EXIT_IF_BAD(0 == status, 1, "Failed to create thread");

        ++curr_thread;
    }
    
    end = time(NULL);

    curr_thread = threads;

    while (curr_thread != end_threads)
    {
        status = pthread_join(*curr_thread, &ret_val);
        EXIT_IF_BAD(0 == status, 1, "Failed to join thread");

        sum_of_divisors += (size_t)ret_val;

        ++curr_thread;
    }

    printf("Sum of divisors: %lu\n", sum_of_divisors);

    return 0;
}

static size_t SumDividersIMP(size_t from_, size_t to_, size_t num_)
{
    size_t i = 0;
    size_t sum = 0;

    for (i = from_; i <= to_; ++i)
    {
        if (0 == num_ % i)
        {
            sum += i;
        }
    }

    return sum;
}

static void* GetSumOfDivisorsIMP(void* param_)
{
    size_t i = (size_t)param_;
    size_t from = i * RANGE + 1;
    size_t to = from + RANGE - 1;

    if (NUM_THREADS - 1 == i)
    {
        to = TARGET;
    }

    return (void*)SumDividersIMP(from, to, TARGET);
}
