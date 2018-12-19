#include "mrand.h"

/*
 *  mrand.c
 *
 *  Created by Joshua Cook 
 *  Copyright (c) Joshua Cook. All rights reserved.
 *
 *  Publicly licensed under the BSD License.
 *  http://www.opensource.org/licenses/bsd-license.html
 */

Generator gen;

Generator
newGenerator( const char* input_file ) 
{
   /* create a new mrandom number generator.
    * argument ``input_file'' specifies the path of the random number file.
    *          defaults to "random.txt" */
   Generator g;

   if (!( g = malloc( sizeof(generator) ) )) {
      return NULL;
   }

   memset( g, 0, sizeof(generator) );

   if (!( g->int_list = malloc(LIST_SIZE*sizeof(g->int_list)) )) {
      return NULL;
   }

   if ( !input_file ) {
      input_file = RANDOM_TXT;
   }

   g->rand_file = fopen( input_file, "r" );

   if (!(g->rand_file)) {
      return NULL;
   }

   return g;
}

void
mrseed( unsigned int seed ) {
   int i, list_size = LIST_SIZE;
   //size_t tmp;
   char* buf;

   if (!( buf = malloc(BUF_SIZE) )) {
      return;
   }

   /* create a new mrandom number generator and seed it */
   gen = newGenerator(NULL);
   gen->current_position = seed;

   while (1) {
      /* step through the input file and populate the array of random numbers */
      for ( i=gen->list_len;  i<list_size;  i++ ) {
         if (!( fgets( buf, BUF_SIZE, gen->rand_file ) )) {
            return;
         }
         gen->int_list[i] = atoi(buf);
         ++(gen->list_len);
      }
      /* resize the array by 100% when necessary. */
      list_size*=2;
      gen->int_list = realloc( gen->int_list, list_size*sizeof(gen->int_list) );
   }
   fclose( gen->rand_file );
}

unsigned int
mrand() {
   /* return the next mrandom number. */
   unsigned int r;

   r = gen->int_list[gen->current_position++];

   /* wrap around the end of the array. */
   if ( gen->current_position >= gen->list_len ) {
      gen->current_position = 0;
   }

   return r;
}
