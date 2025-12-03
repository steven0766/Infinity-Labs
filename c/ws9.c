/*************************************
* Reviewer: shir                     *
* Date: 3.12.25                      *
* Version: 1                         *
**************************************/

#include <stdio.h>
#include <string.h>


void* Memset(void* ptr, int x, size_t num_bytes);
static size_t ByteToWord(unsigned char byte_val);
static void TestMemset();
void* Memcpy(void* dest, const void* src, size_t num_bytes);
static void TestMemcpy();
void *Memmove(void* dest, const void* src, size_t n);
static void TestMemmove();
static void PrintMemory(void* ptr, size_t size);


#define NOT_ALIGNED_TO_8(x) (0 != ((size_t)(x) % 8))


int main()
{
    TestMemset();
    TestMemcpy();
    TestMemmove();
    
    return 0;
}


void* Memset(void* ptr, int num, size_t num_bytes)
{
    unsigned char byte_val = (unsigned char)num;
    unsigned char* current = (unsigned char*)ptr;
    size_t word_val = ByteToWord(byte_val);

    while (num_bytes > 0 && NOT_ALIGNED_TO_8(current))
    {
        *current = byte_val;
        ++current;
        --num_bytes;
    }

    while (num_bytes >= 8)
    {
        *(size_t*)current = word_val;
        current += 8;
        num_bytes -= 8;
    }
    
    while (num_bytes > 0)
    {
        *current = byte_val;
        ++current;
        --num_bytes;
    }
    return ptr;   
}

static size_t ByteToWord(unsigned char byte_val)
{
    size_t word = 0;
    int i = 8;
    
    while (i > 0)
    {
        word <<= 8;
        word |= byte_val;
        --i;
    }
    
    return word;
}

static void TestMemset()
{
    int expected = 0;
    int actual = 0;
    
    memset(&expected, 20, 4);
    Memset(&actual, 20, 4);
    
    if (0 != memcmp(&expected, &actual, 4))
    {
       printf("expdcted:\n");
       PrintMemory(&expected, 4);
       printf("actual:\n");
       PrintMemory(&actual, 4);
    }
    
}


void* Memcpy(void* dest, const void* src, size_t num_bytes)
{
    unsigned char* current_dest = (unsigned char*)dest;
    const unsigned char* current_src = (const unsigned char*)src;
    
    while(num_bytes > 0 && NOT_ALIGNED_TO_8(current_dest))
    {
        *current_dest = *current_src;
        ++current_dest;
        ++current_src;
        --num_bytes;
    }
    
    while(num_bytes >= 8)
    {
        *(size_t*)current_dest = *(size_t*)current_src;
        current_dest += 8;
        current_src += 8;
        num_bytes -= 8;
    }
    
    while (num_bytes > 0)
    {
        *current_dest = *current_src;
        ++current_dest;
        ++current_src;
        --num_bytes;
    }
    
    return dest;
}

static void TestMemcpy()
{
    size_t expected[] = {0, 0, 0 , 0, 0, 0, 0};
    size_t actual[] = {0, 0, 0 , 0, 0, 0, 0};
    size_t subject[] = {25, 75, 22, 58, 4,  99, 12};

    memcpy(expected, subject, 36);
    Memcpy(actual, subject, 36);
    
    if (0 != memcmp(expected, actual, 36))
    {
        printf("expected:\n");
        PrintMemory(expected, 36);
        printf("actual:\n");
        PrintMemory(actual, 36);
    }
}


void *Memmove(void* dest, const void* src, size_t num_bytes)
{
    unsigned char* write_ptr = (unsigned char*)dest;
    unsigned char* read_ptr = (unsigned char*)src;
    
    int is_overlap = (write_ptr > read_ptr &&
                      write_ptr < read_ptr + num_bytes);

    if (is_overlap)
    {
        write_ptr += num_bytes;
        read_ptr += num_bytes;
        
        while (num_bytes > 0 && NOT_ALIGNED_TO_8(write_ptr))
        {
            --write_ptr;
            --read_ptr;
            *write_ptr = *read_ptr;
            --num_bytes;
        }
        
        while (num_bytes >= 8)
        {
            write_ptr -= 8;
            read_ptr -= 8;
            *(size_t*)write_ptr = *(size_t*)read_ptr;
            num_bytes -= 8;
        }
        
        while (num_bytes > 0)
        {
            --write_ptr;
            --read_ptr;
            *write_ptr = *read_ptr;
            --num_bytes;
        }
        
    }
    else
    {
        Memcpy(dest, src, num_bytes);
    }
    
    
    return dest;
}

static void TestMemmove()
{
    size_t expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t actual[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    memmove(expected + 3, expected + 2, 32);
    Memmove(actual + 3, actual + 2, 32);
    
    if (0 != memcmp(expected, actual, 72))
    {
        printf("expected:\n");
        PrintMemory(expected, 72);
        printf("actual:\n");
        PrintMemory(actual, 72);
    }
}

static void PrintMemory(void* ptr, size_t size)
{
  unsigned char* curr = (unsigned char*)ptr;
  while (curr < ((unsigned char*)ptr + size))
  {
    printf("%d ", *curr);
    ++curr;
  }
  printf("\n");
}
