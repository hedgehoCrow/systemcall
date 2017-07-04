#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    long i, ret;
    unsigned long long from, to, diff;

    pid = getpid();
    for(i = 0; i < 5; i++) {
      // get overhead
      __asm__ volatile("rdtsc" : "=A" (from)); __asm__ volatile("rdtsc" : "=A" (to)); diff = to - from;
      // get counter
      __asm__ volatile("rdtsc" : "=A" (from));
      // ret = get_sibling_process_structure(pid); 
      // ”351" is the systemcall number of get_sibling_process_structure
      __asm__ volatile("int $0x80" : "=a" (ret) : "0" (351), "b" (pid));
      // get counter
      __asm__ volatile("rdtsc" : "=A" (to));
      printf("Retval: %d, Clock: %u¥n", ret, (to - from - diff));
    }

    return(0);
}
