typedef struct {
  int pid;
  int page;
  bool used;
} FRAME;


typedef struct {
  int frameNo;
  bool valid;
  bool dirty;
} PTE;

typedef struct {
  int hitCount;
  int missCount;
  int tlbHitCount;
  int tlbMissCount;
} STATS;

typedef struct {
  PTE entry[NUM_PROC][NUM_PAGE];
  STATS stats;
} PT;

typedef struct {
  int entry[2]; // virtual pageno
  int frameNo[2];
  bool valid[2];
  int lru;
} TLB;

FRAME frame_table[NUM_FRAME];
int pagefault_handler(int pid, int pageNo, char type);
