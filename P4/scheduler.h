#ifndef __SCHED__
#define __SCHED__
#include "process.h"
#include "queue.h"
#include "sjf_schedule.h"
#include "lottery_schedule.h"

queue_t** ready_queue;
process_t* (*scheduler_get_job)();
void (*scheduler_admit)(process_t*);
int (*scheduler_count)();
int (*scheduler_can_preempt)(process_t*);
int (*scheduler_timeout)(process_t*);

#endif
