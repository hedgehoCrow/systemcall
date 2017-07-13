#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define P_MAX 5 // 子プロセス数

unsigned long long get_overhead(void)
{
    unsigned long long from, to, diff;
    __asm__ volatile("rdtsc" : "=A" (from));
    __asm__ volatile("rdtsc" : "=A" (to));
    diff = to - from;
    
    return diff;
}

void call_get_sybling_process_structure(pid_t pid, unsigned long long diff)
{
    long ret;
    unsigned long long from, to;
    char str[1024];
    // get counter
    __asm__ volatile("rdtsc" : "=A" (from));
      
      
    // ret = get_sibling_process_structure(pid); 
    // ”351" is the systemcall number of get_sibling_process_structure
    __asm__ volatile("int $0x80" : "=a" (ret) : "0" (351), "b" (pid), "c" (str));
      

    // get counter
    __asm__ volatile("rdtsc" : "=A" (to));
    
    
    printf("Retval: %d, Clock: %u\n", ret, (to - from - diff));
}

int main()
{
    pid_t my_pid, pid[P_MAX];
    long i;
    unsigned long long from, to, diff;

    // get overhead
    diff = get_overhead();
    
    /* 子プロセスの生成 */
    for(i = 0; i < P_MAX && (pid[i] = fork()) > 0; i++);

    if(i != P_MAX && pid[i] == 0){
	/* 子プロセス */
	// 10分存在
	printf("child process id is %d\n", getpid());
	sleep(600);
    } else if(i == P_MAX){
	/* 親プロセス */    
	// 子プロセスの生成待つ
	sleep(1);
	my_pid = getpid();
	
	/* システムコール実行 */
	// 自分
	call_get_sybling_process_structure(my_pid, diff);
    
	// forkで生成したプロセス
	for(i = 0; i < P_MAX; i++){
	    call_get_sybling_process_structure(pid[i], diff);
	}
	

    } else {
	perror("child fork error");
	return -ECHILD;
    }

    return(0);
}
