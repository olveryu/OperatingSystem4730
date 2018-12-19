#include <stdio.h>
#include "vm.h"
#include "disk.h"
#include <stdlib.h>
#include "pagetable.h"
struct Node *head = NULL;
int next_page;
int clockTime;
int random1()
{
  int next = rand() % NUM_PAGE;
  return next;
}

int fifo()
{

  return next_page ++%NUM_PAGE;

}	

int lru()
{
  int used = -1;
  for(int i = 0; i < NUM_FRAME; i++){
    if(frame_table[i].used == 1){
      used = i;
      break;
    }
  }
  if(used == -1){
    used = 0;
  }
  
  return used;
}

int clock()
{
  while(frame_table[clockTime].used){
    frame_table[clockTime].used = 0;
    clockTime = clockTime++%NUM_FRAME;
  }
  return clockTime;
}

int page_replacement()
{
  int frame;
  if(replacementPolicy == RANDOM)  frame = random1(); 
  else if(replacementPolicy == FIFO)  frame = fifo();
  else if(replacementPolicy == LRU) frame = lru();
  else if(replacementPolicy == CLOCK) frame = clock();

  return frame;
}

