#include "scheduler.h"
#include "utils.h"
#include "process.h"
#include "multilevel_feedback.h"


void scheduler_admit_multilevel( process_t* p )
{
  // TODO: add code here. hints below.
  
  //set priority based on its status 
  //	NEW, timed out, waiting; priority may depends whether is aggressive or not.
  
  // it is ready to run indicate this.
  
  // assign the appropriate time slice based on its priority 
  
  // then enqueu it to the ready queue.
  if(p->status == EXITED){
    return;
  }else if(p->status == TIMEOUT){
    p->priority ++;
  }else if(p->status == WAITING){
    if(SCHEDULER_AGRESSIVE){
      p->priority --;
    }else{
      p->priority = 0;
    }
  }else if(p->status == NEW){
    p->priority = 0; 
  }else if(p->status == PREEMPTED){
    if(SCHEDULER_AGRESSIVE){
      p->priority --;
    }
  }
  // set -1 priority back to 0
  p->priority = (p->priority < 0) ? 0 : p->priority;
  enqueue(ready_queue[p->priority], p);
}

int scheduler_can_preempt_multilevel( process_t* p )
  {
    // TODO: add code here. hints below.
    // returns 0 if preemptive returns 1 it it doesnt.
  if(SCHEDULER_AGRESSIVE){
    for(int i=0; i < p->priority; i++){
      if(ready_queue[i]->count != 0){
	return 1;
      }
    }
  }
  return 0;
}

int scheduler_timeout_multilevel( process_t* p )
{
    /*always swapout when a process exceed its timeslice*/

    // return 0 or 1  whether it exceeds its time slice */
  int quantum;
  int timeslice = clock - p->timeslice_started;
  if(p->priority != 0){
    quantum = p->priority*2;
  }else{
    quantum = 1;
  }
  return timeslice >= quantum - 1;
}

int scheduler_count_multilevel()
{
    /*count jobs on all 8 ready queues*/

    // iterate through all ready queue and get the final count.
  int count = 0;
  for(int i = 0; i < 8; ++i){
    count += ready_queue[i]->count;
  }
  
  return count;
}

process_t* scheduler_get_job_multilevel()
{
    process_t* job = NULL;

    /*check queues from high priority till low*/

    // then dequeue job and return it.
    for(int i = 0; i < 8; ++i){
	if(ready_queue[i]->count != 0){
	    job = dequeue(ready_queue[i]);
	    job->timeslice_started = clock;
	    break;
	}
    }
    return job;
}

void scheduler_initialize_multilevel(int agressive)
{
  SCHEDULER_AGRESSIVE = agressive;
  
  // 8 is ugly below, but it depends what the definition of is is.
  ready_queue = (queue_t**) malloc( sizeof(queue_t*)*8 ); 
  
  /* initialize all 8 queues*/
  int i;
  for( i = 0 ; i < 8 ; i++ )
    ready_queue[i] = create_queue();
  
  /* hook them horns of the interface functions */
  scheduler_admit 		= &scheduler_admit_multilevel;
  scheduler_can_preempt 	= &scheduler_can_preempt_multilevel;
  scheduler_timeout 		= &scheduler_timeout_multilevel;
  scheduler_count 		= &scheduler_count_multilevel;
  scheduler_get_job 		= &scheduler_get_job_multilevel;
  
}
