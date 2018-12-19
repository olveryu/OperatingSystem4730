#include <stdio.h>
#include "vm.h"
#include "disk.h"
#include "pagetable.h"

PT pageTable;
FRAME frame_table[NUM_FRAME];
TLB tlb[NUM_PROC][TLB_ENTRY/2];
int currentPage;

char* bool2char(bool b)
{
  if(b) return "hit";
  else return "miss";
}

void print_result(int pid, char type, bool tlb_hit, bool page_hit, int virtual_addr, int physical_addr)
{
  printf("[pid %d, %c] TLB:%s,\t Page:%s,\t 0x%x\t -> 0x%x\n", pid, type, bool2char(tlb_hit), bool2char(page_hit), virtual_addr, physical_addr);
}

int MMU(int pid, int addr, char type)
{
  int frameNo;
  int pageNo = (addr >> 8);
  int offset = addr - (pageNo << 8);
  int physicalAddr;
  bool tlb_hit = 0;
  bool page_hit = 0;

  if(pageNo > NUM_PAGE) {
    printf("invalid page number (NUM_PAGE = 0x%x): pid %d, addr %x\n", NUM_PAGE, pid, addr);
    return -1;
  }
  if(pid > NUM_PROC-1) {
    printf("invalid pid (NUM_PROC = %d): pid %d, addr %x\n", NUM_PROC, pid, addr);
    return -1;
  }

  
  int tlbIndex = pageNo % (TLB_ENTRY/2);
  //page entry
  PTE *entry = &pageTable.entry[pid][pageNo];

  //page
  if(entry->valid){			// hit
    frameNo = entry->frameNo;
    page_hit = 1;
    pageTable.stats.hitCount++;
  }else{				// miss
    frameNo = currentPage;
    if(frameNo >= NUM_FRAME){
      frameNo = page_replacement();
      int victimPid = frame_table[frameNo].pid;
      int victimPageNo = frame_table[frameNo].page;
      PTE *victimEntry = &pageTable.entry[victimPid][victimPageNo];
      if(victimEntry -> dirty){
	//write
	disk_write(frameNo,victimPid, victimPageNo);
	frame_table[frameNo].used = 0;
	victimEntry->dirty = 0;
      }
      victimEntry->valid = 0;
      victimEntry->frameNo = -1;
    }else{
      currentPage++;
    }
    //read
    disk_read(frameNo,pid,pageNo);
    entry->frameNo = frameNo;
    entry->valid = 1;
    entry->dirty = 0;
    pageTable.stats.missCount++;
  }

  //tlb
  for(int i = 0; i < 2; i++){		//hit
    if(tlb[pid][tlbIndex].valid[i] && tlb[pid][tlbIndex].entry[i] == pageNo){
      tlb[pid][tlbIndex].lru = 1-i;
      if(page_hit == 1){
	tlb_hit = 1;
	pageTable.stats.tlbHitCount++;
	frame_table[frameNo].used = 1;
      }
    }
  }
  if(tlb_hit == 0){			//miss
    pageTable.stats.tlbMissCount++;
  }

  //make it dirty if W
  if(type=='W'){
    entry->dirty = 1;
  }

  // add to frame table
  int lru = tlb[pid][tlbIndex].lru;
  tlb[pid][tlbIndex].frameNo[lru] = frameNo;
  tlb[pid][tlbIndex].entry[lru] = pageNo;
  tlb[pid][tlbIndex].valid[lru] = 1;
  tlb[pid][tlbIndex].lru = (1-lru);
  frame_table[frameNo].pid = pid;
  frame_table[frameNo].page = pageNo;  
  physicalAddr = (frameNo<<8)+offset;

  // print result
  print_result(pid,type,tlb_hit, page_hit, addr, physicalAddr);
  return physicalAddr;
}

void pt_print_stats()
{
  int req = pageTable.stats.hitCount + pageTable.stats.missCount;
  int hit = pageTable.stats.hitCount;
  int miss = pageTable.stats.missCount;
  int tlbHit = pageTable.stats.tlbHitCount;
  int tlbMiss = pageTable.stats.tlbMissCount;

  printf("Request: %d\n", req);
  printf("Page Hit: %d (%.2f%%)\n", hit, (float) hit*100 / (float)req);
  printf("Page Miss: %d (%.2f%%)\n", miss, (float)miss*100 / (float)req);
  printf("TLB Hit: %d (%.2f%%)\n", tlbHit, (float) tlbHit*100 / (float)req);
  printf("TLB Miss: %d (%.2f%%)\n", tlbMiss, (float) tlbMiss*100 / (float)req);

}

void pt_init()
{
  int i,j;

  pageTable.stats.hitCount = 0;
  pageTable.stats.missCount = 0;
  pageTable.stats.tlbHitCount = 0;
  pageTable.stats.tlbMissCount = 0;

  for(i = 0; i < NUM_PROC; i++) {
    for(j = 0; j < NUM_PAGE; j++) {
      pageTable.entry[i][j].valid = false;
    }
  }
}

void tlb_init()
{
  int i, j;
  for (i = 0; i < NUM_PROC; i++)
    {
      for(j = 0; j < TLB_ENTRY / 2; j++)
	{
	  tlb[i][j].valid[0] = tlb[i][j].valid[1] = false;
	  tlb[i][j].lru = 0;
	}
    }
}

