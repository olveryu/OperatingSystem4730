#ifndef __MUFs__
#define __MUFs__
#include "process.h"
#include "utils.h"

int SCHEDULER_AGRESSIVE;

void scheduler_initialize_multilevel(int);

void scheduler_admit_multilevel(process_t*);

process_t* scheduler_get_job_multilevel();

int scheduler_can_preempt_multilevel(process_t*);

int scheduler_timeout_multilevel(process_t*);

int scheduler_count_multilevel();

#endif
