/***********************************************************************
*File_name: buf_test.c
*Reviewer: Itay
*Version:  1.0
*Date:	   22.12.25
************************************************************************/

#include <stdio.h> /* printf */
#include <string.h>/* strncmp, memset */

#include "buf.h"


#define BUFFERSIZE          10


static void TestBufCreate();
static void TestBufWriteAndRead();

static void PrintError(char* func, char* error);


int main()
{
    TestBufCreate();
    TestBufWriteAndRead();
    
    return 0;
}

static void TestBufCreate()
{
    buf_ty* buffer = BufCreate(BUFFERSIZE);
    
    if (NULL == buffer)
    {
        PrintError("TestBufCreate", "NULL");
    }
    
    if (BUFFERSIZE != BufCap(buffer))
    {
        PrintError("TestBufCreate", "capacity");
    }
    
    if (1 != BufIsEmpty(buffer))
    {
        PrintError("TestBufCreate", "isempty");
    }
    
    BufDestroy(buffer);
}

static void TestBufWriteAndRead()
{
    buf_ty* buffer = BufCreate(BUFFERSIZE);
    char read_buf[BUFFERSIZE + 1] = {0};
    size_t ret = 0;

    /* write full buffer */
    ret = BufWrite(buffer, 10, "ABCDEFGHIJ");
    if (ret != 10)
    {
        PrintError("WriteFull", "expected 10");
    }

    /* read part */
    ret = BufRead(buffer, 7, read_buf);
    if (ret != 7 || strncmp(read_buf, "ABCDEFG", 7) != 0)
    {
        PrintError("ReadPartial", "wrong data");
    }

    /* write causing wrap-around */
    memset(read_buf, 0, sizeof(read_buf));
    ret = BufWrite(buffer, 4, "1234");
    if (ret != 4)
    {
        PrintError("WrapWrite1", "expected 4");
    }

    /* write more than available space */
    ret = BufWrite(buffer, 5, "XXXXX");
    if (ret != 3)
    {
        PrintError("WriteOverflow", "expected partial write");
    }

    /* read everything */
    memset(read_buf, 0, sizeof(read_buf));
    ret = BufRead(buffer, BUFFERSIZE, read_buf);
    if (ret != 10 || strncmp(read_buf, "HIJ1234XXX", 10) != 0)
    {
        PrintError("FinalRead", "wrong data after wrap");
    }

    if (!BufIsEmpty(buffer))
    {
        PrintError("IsEmpty", "buffer should be empty");
    }

    BufDestroy(buffer);
}


static void PrintError(char* func, char* error)
{
    printf("error in %s function, %s\n", func, error);
}
