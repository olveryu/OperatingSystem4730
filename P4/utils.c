#include <unistd.h>
#include "utils.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


void os_srand( unsigned int seed )
{
    if( RAND_FROM_FILE )
        mrseed( seed );
    else
        srand( seed );
}


int os_rand()
{
    if( RAND_FROM_FILE )
	return mrand();
    else
        return rand();
}


int IO_request()
{
    return os_rand() % CHANCE_IO_REQUEST == 0 ? 1:0;
}

int IO_complete()
{
    return os_rand() % CHANCE_IO_COMPLETE == 0 ? 1:0;
}

void parse_args(int argc, char** argv)
{
    POLICY	  =0;
    RAND_FROM_FILE=0;
    RAND_SEED 	= 0;
    CHANCE_IO_REQUEST=10;
    CHANCE_IO_COMPLETE=4;
    VERBOSE=0;

    int i;

    for( i = 0; i < 8; i++ )
        TIMESLICE[i] = 1<<i;

    int c;
    opterr = 0;

    while( (c = getopt( argc, argv,"vfANSL0:1:2:3:4:5:6:7:s:r:c:" ) ) != -1 ) 
    {
            switch(c)
            {
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7': TIMESLICE[c - 48] 	= atoi(optarg);      break;
	    case 's': RAND_SEED 		= atoi(optarg);      break;
	    case 'r': CHANCE_IO_REQUEST	= atoi(optarg);      break;
	    case 'c': CHANCE_IO_COMPLETE	= atoi(optarg);      break;
	    case 'v': VERBOSE		= 1; 		     break;
	    case 'f': RAND_FROM_FILE	= 1; 		     break;
	    case 'A': POLICY		= MUF_AGGRESSIVE;    break;
	    case 'N': POLICY		= MUF_NONAGGRESSIVE; break;
	    case 'S':
	      POLICY = SJF;
	      break;
	    case 'L':
	      POLICY = LOTTERY;
	      break;
            }
    }

}
/*        1         2         3         4         5         6         7         8
012345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

void parse_stdin()
{
    queue_t* ps = create_queue();

    char delim[] = ":";
    char line[256]; /* buffer for the line*/

    /* loop over from stdin */
    while( fgets( line, sizeof(line), stdin ) )
    {
        /*parse each process and create its struct*/

	// Process Id : Arrival time : Service time : Priority
	// 123:0:10:1

        process_t* p = malloc( sizeof( process_t ) );

        p->pid 			= atoi( strtok( line, delim ) );
        p->arrival_time 	= atoi( strtok( NULL, delim ) );
        p->service_time 	= atoi( strtok( NULL, delim ) );
        p->priority 		= atoi( strtok( NULL, delim ) );
        p->status 		= NEW;
	
        p->first_time 		= 1;
	// TODO: initialize other fields.

        /*push it to a queue */
        enqueue( ps, (void*) p);
    }

    input_jobs_count = ps->count;

    /*a dynamic array to store all the incoming processes*/
    input_jobs = (process_t**) malloc(sizeof(process_t*)*ps->count);
    int i=0;

    /*empty the input queue into the array*/
    while( ps->count > 0 )
        input_jobs[i++] = (process_t*) dequeue(ps);
    destroy_queue(ps);
    
    /*sorting the processes by arrival time, tie broken by PID*/
    int j;
    for( i = 0; i< input_jobs_count; i++ )
    {
        for( j = i+1; j< input_jobs_count; j++ )
	{
            if( input_jobs[j]->arrival_time < input_jobs[i]->arrival_time 
		|| ( input_jobs[j]->arrival_time == input_jobs[i]->arrival_time 
		     && input_jobs[j]->pid < input_jobs[i]->pid
	           ) 
	      ) // ends if(
	    {
		// lets swap the order.
       		process_t* temp = input_jobs[i];
                input_jobs[i]   = input_jobs[j];
                input_jobs[j]   = temp;
            } // if
        } // inner for
    } // outer for
}

