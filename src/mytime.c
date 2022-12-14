// $Id: mytime.c,v 1.2 2008/07/08 11:19:32 jmh Exp $
#ifndef _WIN32
 #line 26 "mytime.nw"
#endif

#ifdef SGI
 #include <fcntl.h>
 #include <sys/mman.h>
 #include <sys/syssgi.h>
 #include <sys/immu.h>
 #include <stdio.h>
 #include <errno.h>
 #include <assert.h>
#else
 #include <stdlib.h>
 #include <stdio.h>
 #ifndef _WIN32
  // Linux
  #include <sys/time.h>
  #include <sys/resource.h>
 #else
  // Windows crap
  #include <windows.h>
 #endif
#endif

#ifndef _WIN32
 typedef unsigned long long iotimer_t;
#endif

#ifdef SGI
#line 71 "mytime.nw"
__psunsigned_t phys_addr, raddr;
unsigned int cycleval;
double clock_res_in_secs;
volatile iotimer_t *iotimer_addr,counter_value;
static int fd;
#line 85 "mytime.nw"
void InitTimer() {

  /* unsigned low, high; */
  extern int errno;
  int poffmask;

  phys_addr = syssgi(SGI_QUERY_CYCLECNTR, &cycleval);
  clock_res_in_secs = 1e-12 * cycleval;

  assert(phys_addr != ENODEV);

  poffmask = getpagesize() - 1;

  raddr = (phys_addr & ~poffmask);

  fd = open("/dev/mmem", O_RDONLY);

  iotimer_addr = (volatile iotimer_t *)mmap(0, poffmask, PROT_READ,
                                         MAP_PRIVATE, fd, (__psint_t)raddr);
  iotimer_addr = (iotimer_t *)((__psunsigned_t) iotimer_addr + (poffmask & phys_addr));

}
#line 114 "mytime.nw"
static void read_time(volatile iotimer_t * iotimer_addr, long long* high) {

    *high = (long long) *iotimer_addr;

}
#line 132 "mytime.nw"
double mytime (void) {

    double out;
    static int init_called = 0;
    long long strobe;

    if (init_called == 0) {
        InitTimer();
        init_called = 1;
    }

    read_time(iotimer_addr, &strobe);
    out = ((double)(strobe)) * clock_res_in_secs;
    return(out);

}
#line 45 "mytime.nw"
#else
 #ifndef _WIN32
  #line 156 "mytime.nw"
  double mytime (void) {
    struct rusage foo;
    register double sec;
    register double usec;

    getrusage (RUSAGE_SELF, &foo);
    sec = (double) foo.ru_utime.tv_sec;
    usec = ((double) foo.ru_utime.tv_usec) / 1000000.0;
    return (sec + usec);
  }
  #line 47 "mytime.nw"
 #else
  // Windows crap
  double mytime(void) {
    static int init = 0;
    static LARGE_INTEGER pFreq;
    static LARGE_INTEGER pCounterOffset;

    if (!init) {
      QueryPerformanceFrequency(&pFreq);
      QueryPerformanceCounter(&pCounterOffset);
      init = 1;
      return 0;
    }
    else {
      LARGE_INTEGER pCount;
      QueryPerformanceCounter(&pCount);
      return ((double)(pCount.QuadPart - pCounterOffset.QuadPart)) / pFreq.QuadPart;
    }
  }
 #endif
#endif
