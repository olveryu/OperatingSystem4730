#include <stdbool.h>

//#define NDEBUG

#define ZERO 0

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG(%s:%d) " M, __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define RANDOM 0
#define FIFO 1
#define LRU 2
#define CLOCK 3

#define TLB_ENTRY 4
#define NUM_FRAME 8
#define NUM_PAGE 8
#define NUM_PROC 2

extern int replacementPolicy;
int page_replacement();
int MMU(int pid, int addr, char type);
void disk_print_stats();
void pt_print_stats();
void pt_init();
void tlb_init();
