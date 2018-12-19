#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "process.h"
#include "utils.h"
#include "scheduler.h"
#include "multilevel_feedback.h"

// gutted code - use at your own risk

int context_switches;

void coordinator()
{
    if( VERBOSE )
    {
        printf("\n\n\n");
        printf("Tick    PID Burst Sleep    IO Queue               IO Completed        Status\n");
        printf("=====+=====+=====+=====+======================+======================+==================\n");
    }

    clock = 0;  /* and so it begins */
 
    context_switches = 0; /* tracks the number of context switches*/

    int arrival_index = 0;

    os_srand(RAND_SEED);

    /* let there be IO queue */
    queue_t* waiting_queue = create_queue(); 
    /* initialize the scheduler of the chosen policy */
    switch(POLICY){
    case LOTTERY:
      scheduler_initialize_lottery();
      break;
    case SJF:
      scheduler_initialize_sjf();
      break;
    case MUF_AGGRESSIVE:
      scheduler_initialize_multilevel(1);
      break;
    case MUF_NONAGGRESSIVE:
      scheduler_initialize_multilevel(0);
      break;
    default:
      break;
    }

    // add jobs

    while(arrival_index < input_jobs_count && input_jobs[arrival_index]->arrival_time <= clock){
      scheduler_admit(input_jobs[arrival_index]);
      arrival_index++;
    }
    /* jobs arriving at 0 admitted now to kickstart the cycle of agony  ---
    	// admit all those jobs and enqueue them to the ready queue
    	 	// find those jobs by looping input_jobs
			// loopedy loopy doo.
    */

    // ---------- while( there are jobs in the system ) // main coordinator loop
    // Keep processing while scheduler is not empty, 
    //	there are processes yet to come, 
    // 	or some processes are on IO

    while(input_jobs_count > arrival_index ||  scheduler_count() != 0 || waiting_queue->count != 0)
    {
      /* call up 'that hooked up scheduler to get that next job */
        process_t* current_job = (*scheduler_get_job)();

        /* if the scheduler did provide a job (not empty) */
        if( current_job != NULL )
        {
            context_switches++;                 /* switching to that process content */
            current_job -> status = RUNNING;    /* marked running */

            if( current_job->first_time )
	      {        
		// you go the chance to attend to response time here.
		current_job->response_time = clock - current_job->arrival_time;
		current_job->first_time = 0;
	      }
	}


        /*spin me round and round*/
        while(1)
        {
            /* admission of processes arriving at this clock tick */
    		// admit all those jobs and enqueue them to the ready queue
    	  		// find those jobs by looping input_jobs
				// loopedy loopy doo.


            /*IO queue checking zone, tread carefully, you are on hallow ground */

	  while(arrival_index < input_jobs_count && input_jobs[arrival_index]->arrival_time <= clock){
	    scheduler_admit(input_jobs[arrival_index]);
	    arrival_index++;
	  }

	  //char none[] = "none";
	  char waiting_list[256]=""; /* list of pids on the IO queue*/
	  char waking_list[256]="";  /* list of pids on leaving the IO queue*/
	  char pid_str[10]; /*buffer to convert pid to string*/
	  /*prepare to iterate on the IO queue*/
	  
	  
	  queue_iterator_t * queueIterator = queue_iterator_create(waiting_queue);
	  while(queue_iterator_has_next(queueIterator) )
	    {
	      process_t *p = queue_iterator_next(queueIterator);
	      snprintf(pid_str,sizeof(pid_str), "%d ",p->pid);
	      /* check if this job's I/O is complete */
	      if( IO_complete()){
		strncat(waking_list,pid_str,sizeof(waking_list)-1);
		queue_iterator_remove(queueIterator);
		scheduler_admit(p);
	      }else{
		strncat(waiting_list,pid_str,sizeof(waiting_list)-1);
		p->io_time++;
	      }
            }
	  /*END OF IO Queue checking, you may now breath */
	  
	  /*no job received from scheduler, let's do nothing*/
	  if( current_job == NULL )
	    {
	      if(VERBOSE) printf("%5d:%5s:%5s:%5s:%-22s:%-22s:%-18s\n",
				 clock, 
				 "*", 
				 "*",
				 "*",
				 waiting_list,
				 waking_list,
				 "idling"
				 ); 
	      clock++; // -- getting out of loop so OK.
	      
	      break;
            }
	  
	  /* so there IS a job */
	  
	  // some bookkeeping.
	  
	  int swap_out = 1; // this may change.
	  
	  char* status = NULL; /* for display purposes */
	  current_job->cpu_time++;
	  // check if current job is done.
	  if(current_job->cpu_time >= current_job->service_time){
	    current_job->status = EXITED;
	    current_job->end_time = clock;
	    status = "* exited";
	  }else if( IO_request()){
	      status = "sleeping";
	      current_job->status = WAITING;
	      enqueue(waiting_queue, current_job);
	      current_job->io_time++;
	  }else if(scheduler_can_preempt(current_job)){
	      status = "preempted";
	      current_job->status = PREEMPTED;
	  }else if(scheduler_timeout(current_job)){
	      status = "slice expired";
	      current_job->status = TIMEOUT;
	  }else{
	    swap_out = 0;
	    status = "still running";
	  }
	    
	    // check if current job is done.
	    // status = "* exited";
	    // else if check if we need to do IO_request()
                // status="sleeping";
		// enqueu to waiting queue.
		
            // else if scheduler_can_preempt current_job)) 
		//Yo, scheduler, anybody tryna preempt?
		// add current job  back to papa
                // status = "preempted";
            // else if -- how about exceeding time slice?
                // current_job->status = TIMEOUT;
		// add current job  back to papa  again.
                // status = "time slice expired";
            // else
                // swap_out = 0; /*do NOT swap out, leave it running */
                // status = "running";

            /*some process stats*/

            if(VERBOSE) printf("%5d:%5d:%5d:%5s:%-22s:%-22s:%-18s\n",
			       clock, 
			       current_job->pid, 
			       current_job->service_time - current_job->cpu_time,
			       current_job->status == WAITING?"True":"False",
			       waiting_list,
			       waking_list,
			       status
			       ); 
            clock++;
	    if (swap_out) {
                if(current_job->status != WAITING && current_job->status !=EXITED ){
		  scheduler_admit(current_job);
                }
                break;
            }
        }
    }
}
void printStats(){
    for (int i = 0; i < input_jobs_count; i++) {
      process_t* p = input_jobs[i];
      p->system_time = (p->end_time) - (p->arrival_time) + 1;
      p->readyToRun_time = p->system_time - p->io_time -  p->service_time;
    }
    int shortest_turn_around = INT_MAX;
    int longest_turn_around = INT_MIN;
    int sum_response_time = 0;
    double sum_penalty_ratio = 0;
    int sum_completion_time = 0;
    int sum_ready_queue_time = 0;
    int sum_io_time = 0;
    
    printf("\n\n\n");
    printf("         |  Response   | Total time  | Total time      | Total time      | Total time    | Penalty     \n");
    printf("Job#     |    time     | on cpu      | in ready to run | in sleeping on  | in system     |  Ratio      \n");
    printf("         |             |             | state           | I/O state       |               |             \n");
    printf("=========+=============+=============+=================+=================+===============+=============\n");

    for(int i =0; i< input_jobs_count; i++)
    {
 
        process_t* p = input_jobs[i];

        printf("%5d    |%5d        |%5d        |%5d            |%4d             |%5d          |%5.2lf\n", 
               p->pid,
               p->response_time,
               p->cpu_time, 
               p->readyToRun_time,
               p->io_time,
               p->system_time,
               (double)(p->system_time) / (p->service_time)
	       );
	
	if(p->system_time < shortest_turn_around){
            shortest_turn_around = p->system_time;
        }
        if(p->system_time > longest_turn_around){
            longest_turn_around = p->system_time;
        }
        sum_response_time += p->response_time;
        sum_penalty_ratio += (double)(p->system_time) / (p->service_time);
        sum_completion_time += p->system_time;
        sum_ready_queue_time += p->readyToRun_time;
        sum_io_time += p->io_time;
    }

    char *policy = NULL;
    switch (POLICY)
      {
      case MUF_NONAGGRESSIVE:
	policy="Non-aggressive Preemptive Multilevel Feedback";
	break;
	
      case MUF_AGGRESSIVE:
	policy="Aggressive Preemptive Multilevel Feedback";
	break;
	
      case LOTTERY:
	policy = "Lottery"; break;
	
      case SJF:
        policy = "Preemptive Shortest Job First" ;
	break;
      }
    
    printf("\n\n  %s\n", policy);
    printf("%30s:%8d\n", "Total simulation time", clock);
    printf("%30s:%8d\n", "Total number of jobs", input_jobs_count );
    printf("%30s:%8d\n", "Shortest job turn-around time", shortest_turn_around);
    printf("%30s:%8d\n", "Longest job turn-around time", longest_turn_around);
    printf("%30s:%8d\n", "Number of context switches", context_switches);
    printf("%30s:%8.2f\n", "Average response time",(double)sum_response_time/input_jobs_count);
    printf("%30s:%8.2f\n", "Average penalty ratio",(double)sum_penalty_ratio/input_jobs_count);
    printf("%30s:%8.2f\n", "Average completion time",(double)sum_completion_time/input_jobs_count);
    printf("%30s:%8.2f\n", "Average time in ready queue",(double)sum_ready_queue_time/input_jobs_count);
    printf("%30s:%8.2f\n", "Average time sleeping on I/O",(double)sum_io_time/input_jobs_count);
    printf("\n");
}
int main(int argc, char** argv)
{
    parse_args( argc, argv );
    parse_stdin();
    coordinator();
    printStats();
    return 0;
}
