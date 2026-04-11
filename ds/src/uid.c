/***********************************************************************
*File_name: uid.c
*Reviewer: itay
*Version:  1.0
*Date:	   6.1.26
************************************************************************/

#include <time.h>       /* time, difftime, time_t */
#include <unistd.h>     /* getpid */
#include <arpa/inet.h>  /* inet_ntoa */
#include <ifaddrs.h>    /* struct ifaddrs, getifaddrs, freeifaddrs */
#include <string.h>     /* strcpy, strcmp */
#include <linux/if.h>   /* IFF_LOOPBACK */
#include <sys/types.h>  /* size_t, pid_t */
#include <stdatomic.h>  /* atomic_fetch_add */

#include "uid.h"

static void SetIP(char* ip_buffer);

const uid_ty invalid_uid_g = {0};

uid_ty UidCreate(void)
{
	static size_t counter = 0;

	uid_ty uid = {0};

	uid.time = time(NULL);
	
	if ((time_t)-1 == uid.time)
	{
	    return invalid_uid_g;
	}

	uid.counter = atomic_fetch_add(&counter, 1) + 1;
	uid.pid = getpid();

	SetIP(uid.ip);
	
	if (0 == strcmp(uid.ip, ""))
	{
	    return invalid_uid_g;
	}

	return uid;
}

int IsMatchId(uid_ty id_1, uid_ty id_2)
{
    return (difftime(id_1.time, id_2.time) == 0 &&
	   id_1.counter == id_2.counter &&
	   id_1.pid == id_2.pid &&
           strcmp(id_1.ip, id_2.ip) == 0);
}

static void SetIP(char* ip_buffer)
{
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr = NULL;
    int status_getadd = getifaddrs(&ifap);
    
    if (status_getadd)
    {
      strcpy(ip_buffer, "");
      return;
    }
    
    for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && (AF_INET == ifa->ifa_addr->sa_family))
        {
            sa = (struct sockaddr_in*)ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            strcpy(ip_buffer, addr);
            if (!(ifa->ifa_flags & IFF_LOOPBACK))
            {
                break;
            }
        }
    }
    
    freeifaddrs(ifap);
    ifap = NULL;
}
