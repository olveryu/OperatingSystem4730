#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

int main (int argc, char *argv[] ) 
{
  int i;
  int totalnumbers=100;
  int numbegin=-1000000;
  int numend=100000000;
  int range=0;
  int number;

  if( argc < 3 ) 
	printf("randomnumers <totalnumbers> <range from> <range to>\n");

  srand( time( NULL) );   /* Seed the random number generator. */

  if( argc < 4 )
	exit(0);

  if( argc > 1 )
	totalnumbers =  atoi( argv[1] ) ; 

  if( argc > 2 )
	numbegin =  atoi( argv[2] ) ; 

  if( argc > 3 )
	numend =  atoi( argv[3] ) ; 

  fprintf( stderr,  "generating random numbers in the range [ %d, %d ] skipping 0s\n", numbegin, numend );
  fprintf( stderr,  "starting to  generated %d numbers : %d to stdout \n", totalnumbers );

  range = numend - numbegin + 1;

  for( i = 0; i < totalnumbers ; i++ )
  {
	number = ((int) ( range * ( rand() / (1.0 + RAND_MAX))) + numbegin ) ;

	if( number != 0 )
		printf("%3d\n", number ); 
   }
  printf("0\n"); /* terminating file with a zero */
  return 0;
}

