#include <stdio.h>
#include <string.h>
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

int is_num(char c)
{
    if('0' <= c && c <= '9'){
	return 1;
    } else {
	return 0;
    }
}

void print_tree(char *str, int *index, int depth)
{
    int i;
    char leaf[128], str_pid[8], space[2], next[2];
    
    leaf[0] = '\0';
    str_pid[0] = '\0';
    space[0] = ' ';
    space[1] = '\0';
    next[0] = '+';
    next[1] = '\0';

    for(i = 0; i < depth; i++){
        strcat(leaf, space);
    }
    if(depth != 0){
        strcat(leaf, next);
    }
    leaf[strlen(leaf)] = '\0';


    while(*index < strlen(str) && str[*index] != ']'){
        /* pidまで移動 */
        for(; *index < strlen(str) && !is_num(str[*index]); (*index)++);
        /* pid取得分 */
        for(i = 0; *index < strlen(str) && is_num(str[*index]); i++, (*index)++){
            str_pid[i] = str[*index];
        }
        str_pid[i] = '\0';

        strcat(leaf, str_pid);
        printf("%s\n", leaf);

	/* pid reset */
	leaf[strlen(leaf)-strlen(str_pid)] = '\0';

        /* 子供要素まで移動 */
        for(; *index < strlen(str) && str[*index] != '['; (*index)++);
        (*index)++;

        print_tree(str, index, depth+1);

    }
    (*index)++;
}

void call_get_sybling_process_structure(pid_t pid, unsigned long long diff)
{
    int *index;
    long ret;
    unsigned long long from, to;
    char str[512];
    *index = 0;

    // get counter
    __asm__ volatile("rdtsc" : "=A" (from));
      
      
    // ret = get_sibling_process_structure(pid); 
    // ”351" is the systemcall number of get_sibling_process_structure
    __asm__ volatile("int $0x80" : "=a" (ret) : "0" (351), "b" (pid), "c" (str));
    errno = ret
    
    printf("tree struct:  %s\n", str);
    /* ツリー構造出力 */
    print_tree(str, index, 0);  

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
