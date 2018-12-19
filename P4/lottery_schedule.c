#include "scheduler.h"
#include "utils.h"
#include "process.h"
#include "lottery_schedule.h"

process_t **queue;
int queue_size; 
int queue_index;
int LOTTERY_TIME = 5;
int LOTTERY_TICKET = 5;

void scheduler_admit_lottery( process_t* p)
{
  if(p->status == EXITED){
    return;
  }else if(p->status == NEW){
    p->lottery_tickets = LOTTERY_TICKET;
  }else if(p->status == TIMEOUT){
    p->lottery_tickets --;
  }else if(p->status == WAITING){
    p->lottery_tickets ++;
  }
  if(p->lottery_tickets < 1){
    p->lottery_tickets = 1;
  }
  queue[queue_index] = p;
  queue_index ++;
}

int scheduler_can_preempt_lottery( process_t* p )
{
  UNUSED(p);
  return 0;
}

int scheduler_timeout_lottery( process_t* p )
{
  if(clock - p->timeslice_started >= LOTTERY_TIME){
    return 1;
  }else{
    return 0;
  }
}

int scheduler_count_lottery()
{
  return queue_index;
}

process_t* scheduler_get_job_lottery()
{
  process_t* job = NULL;
  if(queue_index == 0){
    return 0;
  }
  int totalTickets = 0;
  for(int i = 0; i < queue_index; i++){
    totalTickets += queue[i]->lottery_tickets;
  }
  
  int bingo = (mrand() % totalTickets)+1;
  int currentTicket = 0;
  for(int i = 0; i < queue_index; i++){
    currentTicket += queue[i]->lottery_tickets;
    if(currentTicket >= bingo){
      job = queue[i];
      queue_index--;
      queue[i] = queue[queue_index];
      job->timeslice_started = clock;
      break;
    }
  }

  return job;
}

void scheduler_initialize_lottery()
{
  queue_size = 512;
  queue = (process_t **)calloc(queue_size, sizeof(queue[0]));
  
  /* hook them horns of the interface functions */
  scheduler_admit 		= &scheduler_admit_lottery;
  scheduler_can_preempt 	= &scheduler_can_preempt_lottery;
  scheduler_timeout 		= &scheduler_timeout_lottery;
  scheduler_count 		= &scheduler_count_lottery;
  scheduler_get_job 		= &scheduler_get_job_lottery;
}
