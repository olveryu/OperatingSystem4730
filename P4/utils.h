#ifndef __UTILS__
#define __UTILS__

#include <stdlib.h>

#include "mrand.h"
#include "process.h"
#include "queue.h"

#define UNUSED(...) (void)(__VA_ARGS__)	

#define MUF_NONAGGRESSIVE 	0
#define MUF_AGGRESSIVE 		1
#define SJF		        2  	
#define LOTTERY		        3  	

int    POLICY;
int    VERBOSE;
int    RAND_FROM_FILE;
int    RAND_SEED;
int    CHANCE_IO_REQUEST;
int    CHANCE_IO_COMPLETE;
int    TIMESLICE[8];

process_t** input_jobs;			// not best place for this one.

int 	input_jobs_count; 

void  	parse_args(int, char**);
void  	os_srand(unsigned int);
int   	os_rand();
void  	parse_stdin();
int   	IO_request();
int   	IO_complete();

#endif
