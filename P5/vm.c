#include <stdio.h>
#include <time.h>
#include "vm.h"
#include "disk.h"
#include <stdlib.h>

int replacementPolicy;
int main(int argc, char **argv){
    char input[256];
    int pid, addr;
    char type;

    srand(time(NULL));

    if (argc < 2) {
        fprintf(stderr, "usage: ./vm page_replacement_policy "
                "[0 - RANDOM, 1 - FIFO, 2 - LRU, 3 - CLOCK]\n");
        return -1;
    }
    replacementPolicy = atoi(argv[1]);
    printf("Replacement Policy: %d - ", replacementPolicy);
    if (replacementPolicy == RANDOM) {
        printf("RANDOM\n");
    } else if (replacementPolicy == FIFO) {
        printf("FIFO\n");
    } else if (replacementPolicy == LRU) {
        printf("LRU\n");
    } else if (replacementPolicy == CLOCK) {
        printf("CLOCK\n");
    } else {
        printf("UNKNOWN!\n");
        return -1;
    }
    
    pt_init();
    tlb_init();


    while (fgets(input, 256, stdin)) {
        if (input[0] == '#') {
            continue;
        }
        if (sscanf(input, "%d %c 0x%x", &pid, &type, &addr) < 3) {
            printf("Error: invalid format\n");
            return 0;
        }
        int physicalAddr = MMU(pid, addr, type);
    }

    printf("=====================================\n");
    pt_print_stats();
    disk_print_stats();
}
