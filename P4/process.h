#ifndef __PROCESS__
#define __PROCESS__

#define NEW 		0
#define READY_TO_RUN 	1
#define RUNNING 	2
#define TIMEOUT 	3
#define PREEMPTED 	4
#define WAITING 	5
#define EXITED 		6

int clock;
typedef struct process_t
    { // process control block - add fields if needed.
     // Process Id : Arrival time : Service time : Priority
     // 123:0:10:1

      int pid;
      int arrival_time;
      int service_time;
      int priority;
      int status;      
      int first_time;
      
      // added
      int end_time;
      int timeslice_started;
      int response_time;
      int cpu_time;
      int readyToRun_time;
      int io_time;
      int system_time;

      // sjf
      int sjf_burst;

      // lottery
      int lottery_tickets;
      
} process_t;

#endif
