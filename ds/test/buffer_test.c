/***********************************************************************
*File_name: queue.c
*Reviewer: 
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <stdio.h>
#include <string.h>

#include "buffer.h"


#define BUFFERSIZE          10


static void TestCircularBufferCreate();
static void TestCircularBufferWriteAndRead();

static void PrintError(char* func, char* error);


int main()
{
    TestCircularBufferCreate();
    TestCircularBufferWriteAndRead();
    
    return 0;
}

static void TestCircularBufferCreate()
{
    buffer_ty* buffer = CircularBufferCreate(BUFFERSIZE);
    
    if (NULL == buffer)
    {
        PrintError("TestCircularBufferCreate", "NULL");
    }
    
    if (BUFFERSIZE != CircularBufferCapacity(buffer))
    {
        PrintError("TestCircularBufferCreate", "capacity");
    }
    
    if (1 != CircularBufferIsEmpty(buffer))
    {
        PrintError("TestCircularBufferCreate", "isempty");
    }
    
    CircularBufferDestroy(buffer);
}

static void TestCircularBufferWriteAndRead()
{
    buffer_ty* buffer = CircularBufferCreate(BUFFERSIZE);
    char read_buf[BUFFERSIZE + 1] = {0};
    size_t ret = 0;

    /* write full buffer */
    ret = CircularBufferWrite(buffer, 10, "ABCDEFGHIJ");
    if (ret != 10)
    {
        PrintError("WriteFull", "expected 10");
    }

    /* read part */
    ret = CircularBufferRead(buffer, 7, read_buf);
    if (ret != 7 || strncmp(read_buf, "ABCDEFG", 7) != 0)
    {
        PrintError("ReadPartial", "wrong data");
    }

    /* write causing wrap-around */
    memset(read_buf, 0, sizeof(read_buf));
    ret = CircularBufferWrite(buffer, 4, "1234");
    if (ret != 4)
    {
        PrintError("WrapWrite1", "expected 4");
    }

    /* write more than available space */
    ret = CircularBufferWrite(buffer, 5, "XXXXX");
    if (ret != 3)
    {
        PrintError("WriteOverflow", "expected partial write");
    }

    /* read everything */
    memset(read_buf, 0, sizeof(read_buf));
    ret = CircularBufferRead(buffer, BUFFERSIZE, read_buf);
    if (ret != 10 || strncmp(read_buf, "HIJ1234XXX", 10) != 0)
    {
        PrintError("FinalRead", "wrong data after wrap");
    }

    if (!CircularBufferIsEmpty(buffer))
    {
        PrintError("IsEmpty", "buffer should be empty");
    }

    CircularBufferDestroy(buffer);
}


static void PrintError(char* func, char* error)
{
    printf("error in %s function, %s\n", func, error);
}
