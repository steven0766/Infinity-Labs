#include <stdio.h>

#include "uid.h"

int main()
{
    uid_ty id1 = UidCreate();
    uid_ty id2 = UidCreate();
    
    printf("id1:\n");
    printf("time: %ld\n", id1.time);
    printf("counter: %ld\n", id1.counter);
    printf("pid: %d\n", id1.pid);
    printf("ip: %s\n", id1.ip);
    printf("_________________\n");
    printf("id2:\n");
    printf("time: %ld\n", id2.time);
    printf("counter: %ld\n", id2.counter);
    printf("pid: %d\n", id2.pid);
    printf("ip: %s\n", id2.ip);
    
    return 0;
}
