#include <stdio.h>

int main()
{
    long i, ret;
    unsigned long long from, to, diff;

    for(i = 0; i < 5; i++) {
      // get overhead
      __asm__ volatile("rdtsc" : "=A" (from)); __asm__ volatile("rdtsc" : "=A" (to)); diff = to - from;
      // get counter
      __asm__ volatile("rdtsc" : "=A" (from));
      // ret = newsyscall(NULL); ”351" is the systemcall number of newsyscall()
      __asm__ volatile("int $0x80" : "=a" (ret) : "0" (351));
      // get counter
      __asm__ volatile("rdtsc" : "=A" (to));
      printf("Retval: %d, Clock: %u¥n", ret, (to - from - diff));
    }

    return(0);
}
