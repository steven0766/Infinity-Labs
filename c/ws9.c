/*************************************
* Reviewer: shir                     *
* Date: 3.12.25                      *
* Version: 1                         *
**************************************/

#include <stdio.h>    /* printf, size_t */
#include <string.h>   /* memset, memcpy, memmove, memcmp */


void* Memset(void* ptr, int x, size_t num_bytes);
static size_t ByteToWord(unsigned char byte_val);
static void TestMemset(void);
void* Memcpy(void* dest, const void* src, size_t num_bytes);
static void TestMemcpy(void);
void *Memmove(void* dest, const void* src, size_t n);
static void TestMemmove(void);
static void PrintMemory(void* ptr, size_t size);


#define WORD_SIZE            8
#define NOT_ALIGNED_TO_8(x) (0 != ((size_t)(x) % WORD_SIZE))


int main()
{
    TestMemset();
    TestMemcpy();
    TestMemmove();
    
    return 0;
}

/* fill block of memory with specific byte value
   return: pointer to the start of ptr */
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

    while (num_bytes >= WORD_SIZE)
    {
        *(size_t*)current = word_val;
        current += WORD_SIZE;
        num_bytes -= WORD_SIZE;
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
    int i = WORD_SIZE;
    
    while (i > 0)
    {
        word <<= WORD_SIZE;
        word |= byte_val;
        --i;
    }
    
    return word;
}

static void TestMemset(void)
{
    int expected[10] = {0};
    int actual[10] = {0};
    int subject[] = {5, 499, 1502, -50, 33};
    size_t num_bytes[] = {8, 23, 39, 30, 2};
    size_t i = 0;
    
    for (i = 0; i < sizeof(subject) / sizeof(subject[0]); ++i)
    {
        
        
        memset(&expected, subject[i], num_bytes[i]);
        Memset(&actual, subject[i], num_bytes[i]);
        
        if (0 != memcmp(&expected, &actual, num_bytes[i]))
        {
           printf("expected:\n");
           PrintMemory(&expected, num_bytes[i]);
           printf("actual:\n");
           PrintMemory(&actual, num_bytes[i]);
        }
    }
    
}

/* copy block of memory from src to dest
   return: pointer to the start of pointer we copied to(dest) */
void* Memcpy(void* dest, const void* src, size_t num_bytes)
{
    unsigned char* current_dest = (unsigned char*)dest;
    const unsigned char* current_src = (const unsigned char*)src;
    
    while (num_bytes > 0 && NOT_ALIGNED_TO_8(current_dest))
    {
        *current_dest = *current_src;
        ++current_dest;
        ++current_src;
        --num_bytes;
    }
    
    while (num_bytes >= WORD_SIZE)
    {
        *(size_t*)current_dest = *(size_t*)current_src;
        current_dest += WORD_SIZE;
        current_src += WORD_SIZE;
        num_bytes -= WORD_SIZE;
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

static void TestMemcpy(void)
{
    size_t expected[] = {0, 0, 0 , 0, 0, 0, 0};
    size_t actual[] = {0, 0, 0 , 0, 0, 0, 0};
    size_t subject[] = {25, 75, 22, 58, 4, 99, 12};
    size_t num_bytes = sizeof(subject);
    size_t i = 0;
    
    for (i = 0; i < num_bytes; ++i)
    {
        Memset(expected, 0, sizeof(expected));
        Memset(actual, 0, sizeof(actual));
        
        memcpy(expected, subject, i);
        Memcpy(actual, subject, i);
        
        if (0 != memcmp(expected, actual, num_bytes))
        {
            printf("expected:\n");
            PrintMemory(expected, num_bytes);
            printf("actual:\n");
            PrintMemory(actual, num_bytes);
        }
    }
}

/* copy block of memory from src to dest.
   note: unlike memcpy, it covers the overlaping strugle.
   return: pointer to the start of pointer we copied to(dest) */
void *Memmove(void* dest, const void* src, size_t num_bytes)
{
    unsigned char* write_ptr = (unsigned char*)dest + num_bytes;
    unsigned char* read_ptr = (unsigned char*)src + num_bytes;
    
    int is_overlap = ((unsigned char*)dest > (unsigned char*)src &&
                      (unsigned char*)dest < read_ptr);

    if (is_overlap)
    {   
        while (num_bytes > 0 && NOT_ALIGNED_TO_8(write_ptr))
        {
            --write_ptr;
            --read_ptr;
            *write_ptr = *read_ptr;
            --num_bytes;
        }
        
        while (num_bytes >= WORD_SIZE)
        {
            write_ptr -= WORD_SIZE;
            read_ptr -= WORD_SIZE;
            *(size_t*)write_ptr = *(size_t*)read_ptr;
            num_bytes -= WORD_SIZE;
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

static void TestMemmove(void)
{
    size_t expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t actual[] =   {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t num_bytes = 3 * WORD_SIZE;
    size_t max_offset = 5;
    size_t i = 0, j = 0;
    
    for (i = 0; i <= max_offset; ++i)
    {
        for (j = 0; j < max_offset; ++j)
        {
            expected[j] = j + 1;
            actual[j] = j + 1;
        }
        
        for (j = 0; j <= max_offset; ++j)
        {
            memmove(expected + i, expected + j, num_bytes);
            Memmove(actual + i, actual + j, num_bytes);
            
            if (0 != memcmp(expected, actual, sizeof(expected)))
            {
                printf("expected:\n");
                PrintMemory(expected, 72);
                printf("actual:\n");
                PrintMemory(actual, 72);
            }
        }
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
