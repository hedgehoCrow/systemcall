#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define P_MAX 5 // 子プロセス数

int main()
{
    pid_t my_pid, pid[P_MAX];
    int status;
    long i, ret;
    unsigned long long from, to, diff;

    /* 子プロセスの生成 */
    for(i = 0; i < P_MAX && (pid[i] = fork()) > 0; i++);

    if(i != P_MAX && pid[i] == 0){
	/* 子プロセス */
	// 10分存在
	printf("child process id is %d\n", getpid());
	sleep(600);
    } else if(i == P_MAX){
	/* 親プロセス */    
	sleep(5);
	my_pid = getpid();
	
	// get overhead
	__asm__ volatile("rdtsc" : "=A" (from)); __asm__ volatile("rdtsc" : "=A" (to)); diff = to - from;
      
	// get counter
	__asm__ volatile("rdtsc" : "=A" (from));
      
      
	// ret = get_sibling_process_structure(pid); 
	// ”351" is the systemcall number of get_sibling_process_structure
	__asm__ volatile("int $0x80" : "=a" (ret) : "0" (351), "b" (my_pid));
      

	// get counter
	__asm__ volatile("rdtsc" : "=A" (to));
	printf("Retval: %d, Clock: %u\n", ret, (to - from - diff));
    } else {
	perror("child fork error");
	return -ECHILD;
    }

    return(0);
}
