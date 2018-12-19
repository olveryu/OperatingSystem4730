#ifndef __MRAND_H__
   #define __MRAND_H__

/*
 *  mrand.h
 *
 *  Created by Joshua Cook 
 *  Copyright (c) Joshua Cook. All rights reserved.
 *
 *  Publicly licensed under the BSD License
 *  http://www.opensource.org/licenses/bsd-license.html
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define RANDOM_TXT "random.txt"
#define LIST_SIZE 16
#define BUF_SIZE 128

typedef struct _mrand 
{
   FILE* rand_file;
   int current_position;
   int list_len;
   int* int_list;
} generator;

typedef generator* Generator;

Generator
newGenerator( const char* input_file );

void
mrseed( unsigned int seed );

unsigned int
mrand();

#endif
