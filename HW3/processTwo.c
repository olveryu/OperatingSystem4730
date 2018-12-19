
/*
 * shm-client - client program to demonstrate shared memory.
 * shm-client - not sure of the origin of these code segments.
 * possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHMSZ     27

int
main()
{
  int shmid;
  key_t key;
  char *shm, *s;

  /*
   * We need to get the segment named
   * "5678", created by the server.
   */
  key = 5678;

  /*
   * Locate the segment.
   */
  if( (shmid = shmget(key, SHMSZ, 0666)) < 0 )
    {
      perror("shmget");
      exit(1);
    }

  /*
   * Now we attach the segment to our data space.
   */
  if( (shm = shmat(shmid, NULL, 0)) == (char *) -1 )
    {
      perror("shmat");
      exit(1);
    }

  /*
   * Now read what the server put in the memory.
   */
  while(*shm != '1'){
    sleep(1);
  }
  memcpy(shm+1,"shared",6);
  s = shm;
  s += 7;
  *s = 0;
  *shm = '2';

  return 0;
}
