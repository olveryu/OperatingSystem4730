
#include "avg.h"	
#include <stdlib.h>


void echo_prog_1( char* host, int argc, char *argv[] )
{
   CLIENT *clnt; /* client handle, rpc.h included in avg.h from rpcgen */
   input_data input;
   input_data_echo *echo_1_arg; /* input_data rpc struct */

   clnt = clnt_create( host, ECHO_PROGS, ECHO_VERS, "udp" );

   /* check if error */
   if (clnt == NULL) 
   {
      /* 
       * rpc error library routine
       *  print a more descriptive error 
       */   
      clnt_pcreateerror( host );
      exit(1);
   }

   strncpy(input.text, argv[2], strlen(argv[2]));
   input.text[strlen(argv[2])] = 0;
   echo_1_arg = echo_1( &input, clnt );
   if (echo_1_arg == NULL) 
      {
      clnt_perror(clnt, "call failed:");
      }

   clnt_destroy( clnt );

   printf( "echo: %s\n",echo_1_arg->text_echo );
}


/* here is main */
int main( int argc, char* argv[] )
{
   char *host;

   /* check correct syntax */
   if( argc != 3 )
   {
     	printf( "usage: %s server_host value ...\n", argv[0]); 
     	exit(1);
   }

   /* host name is in first parameter (after program name) */
   host = argv[1];
   echo_prog_1( host, argc, argv);
   exit(0);
}
