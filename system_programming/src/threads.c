#define _POSIX_C_SOURCE 200809L

#include <pthread.h>	/* pthread */
#include <unistd.h>     /* sleep */
#include <stdio.h>      /* printf */
#include <time.h>		/* time, time_t */

static void* WriteIndexIMP(void *arg);

enum
{
    NUM_THREADS = 700000
};

static int g_arr[NUM_THREADS];

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int i = 0;
    int status = 0;
    time_t start, end;
    pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    start = time(NULL);

    for (i = 0; i < NUM_THREADS; ++i)
    {
    	do
    	{
    		status = pthread_create(&threads[i], NULL, WriteIndexIMP, (void*)i);
    	}while (0 != status);
    }
    
	end = time(NULL);
	
	pthread_attr_destroy(&attr);

    sleep(10);

    for (i = 0; i < NUM_THREADS; ++i)
    {
        if (g_arr[i] != i)
        {
        	printf("arr[%d] = %d\n", i, g_arr[i]);
        }
    }

	printf("time: %ld seconds\n", end - start);

    return 0;
}

static void* WriteIndexIMP(void *arg)
{
    int index = (int)arg;
    g_arr[index] = index;

    return NULL;
}
