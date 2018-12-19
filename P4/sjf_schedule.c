#include "scheduler.h"
#include "utils.h"
#include "process.h"
#include "sjf_schedule.h"

double BURST_GUESS = 5.0;
process_t **queue;
int queue_size; 
int queue_index;
int cmpfunc (const void * a, const void * b) {
  const process_t *p1 =*(process_t **)a;
  const process_t *p2 =*(process_t **)b;
  if(p2->sjf_burst > p1->sjf_burst){
    return 1;
  }else if(p2->sjf_burst < p1->sjf_burst){
    return -1;
  }else{
    return p2->pid - p1->pid;
  }
}

void scheduler_admit_sjf( process_t* p)
{
  if(p->status == EXITED){
    return;
  }else if(p->status == NEW){
    p->sjf_burst = BURST_GUESS;
  }else if(p->status == WAITING){
    //do nothing because it is waitting
  }else{
    p->sjf_burst = 0.5 * p->sjf_burst + 0.5 * (clock - p->timeslice_started);
  }
  queue[queue_index] = p;
  queue_index++;
  qsort(queue, queue_index, sizeof(queue[0]), cmpfunc);
}

int scheduler_can_preempt_sjf( process_t* p )
{
  if(queue_index == 0){
	return 0;
    }
  double burst =  0.5 * p->sjf_burst + 0.5 * (clock - p->timeslice_started);
  process_t *temp = queue[queue_index - 1];
  return temp->sjf_burst < burst;
}

int scheduler_timeout_sjf( process_t* p )
{
  UNUSED(p);
  return 0;
}

int scheduler_count_sjf()
{
  return queue_index;
}

process_t* scheduler_get_job_sjf()
{
  if(queue_index == 0){
    return 0;
  }
  queue_index--;
  queue[queue_index]->timeslice_started = clock;
  return queue[queue_index];
}

void scheduler_initialize_sjf()
{
  queue_size = 512;
  queue = (process_t **)calloc(queue_size, sizeof(queue[0]));
  
  /* hook them horns of the interface functions */
  scheduler_admit 		= &scheduler_admit_sjf;
  scheduler_can_preempt 	= &scheduler_can_preempt_sjf;
  scheduler_timeout 		= &scheduler_timeout_sjf;
  scheduler_count 		= &scheduler_count_sjf;
  scheduler_get_job 		= &scheduler_get_job_sjf;
}
