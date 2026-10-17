
#include <stdio.h>

#define MAX_PAGES 100
#define MAX_FRAMES 20

typedef struct {
    int faults;
    int hits;
} Result;

void displayFrames(int frames[], int capacity) {
    printf("[ ");
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == -1)
            printf("- ");
        else
            printf("%d ", frames[i]);
    }
    printf("]");
}

int findPage(int frames[], int capacity, int page) {
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == page)
            return i;
    }
    return -1;
}

Result simulate(int pages[], int n, int capacity,
                int algorithm, int showSteps) {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES] = {0};
    int nextReplace = 0;
    Result result = {0, 0};

    for (int i = 0; i < capacity; i++)
        frames[i] = -1;

    if (showSteps) {
        if (algorithm == 1)
            printf("\nFIFO PAGE REPLACEMENT\n");
        else if (algorithm == 2)
            printf("\nLRU PAGE REPLACEMENT\n");
        else
            printf("\nOPTIMAL PAGE REPLACEMENT\n");

        printf("%-8s %-24s %-12s\n",
               "Page", "Frames", "Status");
        printf("-----------------------------------------------\n");
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int position = findPage(frames, capacity, page);
        int hit = (position != -1);

        if (hit) {
            result.hits++;
            if (algorithm == 2)
                lastUsed[position] = i;
        } else {
            result.faults++;

            int empty = findPage(frames, capacity, -1);
            int victim;

            if (empty != -1) {
                victim = empty;
            } else if (algorithm == 1) {
                victim = nextReplace;
                nextReplace = (nextReplace + 1) % capacity;
            } else if (algorithm == 2) {
                victim = 0;
                for (int j = 1; j < capacity; j++) {
                    if (lastUsed[j] < lastUsed[victim])
                        victim = j;
                }
            } else {
                int farthest = -1;
                victim = 0;

                for (int j = 0; j < capacity; j++) {
                    int nextUse = n;

                    for (int k = i + 1; k < n; k++) {
                        if (pages[k] == frames[j]) {
                            nextUse = k;
                            break;
                        }
                    }

                    if (nextUse > farthest) {
                        farthest = nextUse;
                        victim = j;
                    }
                }
            }

            frames[victim] = page;

            if (algorithm == 2)
                lastUsed[victim] = i;
        }

        if (showSteps) {
            printf("%-8d ", page);
            displayFrames(frames, capacity);
            printf(" %-12s\n", hit ? "HIT" : "PAGE FAULT");
        }
    }

    if (showSteps) {
        printf("\nTotal page references : %d\n", n);
        printf("Page hits             : %d\n", result.hits);
        printf("Page faults           : %d\n", result.faults);
        printf("Hit ratio             : %.2f%%\n",
               result.hits * 100.0 / n);
        printf("Page fault ratio      : %.2f%%\n",
               result.faults * 100.0 / n);
    }

    return result;
}

int main(void) {
    int pages[MAX_PAGES];
    int n, capacity, choice;

    printf("========================================\n");
    printf(" VIRTUAL MEMORY PAGE REPLACEMENT TOOL\n");
    printf("========================================\n");

    printf("Enter number of page references (1-%d): ",
           MAX_PAGES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PAGES) {
        printf("Invalid number of references.\n");
        return 1;
    }

    printf("Enter the page reference sequence:\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &pages[i]) != 1 || pages[i] < 0) {
            printf("Pages must be non-negative integers.\n");
            return 1;
        }
    }

    printf("Enter number of frames (1-%d): ", MAX_FRAMES);
    if (scanf("%d", &capacity) != 1 ||
        capacity < 1 || capacity > MAX_FRAMES) {
        printf("Invalid frame capacity.\n");
        return 1;
    }

    do {
        printf("\n========== MENU ==========\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Compare all algorithms\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        if (choice >= 1 && choice <= 3) {
            simulate(pages, n, capacity, choice, 1);
        } else if (choice == 4) {
            Result fifo = simulate(pages, n, capacity, 1, 0);
            Result lru = simulate(pages, n, capacity, 2, 0);
            Result optimal = simulate(pages, n, capacity, 3, 0);

            printf("\n======= ALGORITHM COMPARISON =======\n");
            printf("%-12s %-10s %-10s %-12s\n",
                   "Algorithm", "Hits", "Faults", "Fault Rate");

            printf("%-12s %-10d %-10d %-11.2f%%\n",
                   "FIFO", fifo.hits, fifo.faults,
                   fifo.faults * 100.0 / n);

            printf("%-12s %-10d %-10d %-11.2f%%\n",
                   "LRU", lru.hits, lru.faults,
                   lru.faults * 100.0 / n);

            printf("%-12s %-10d %-10d %-11.2f%%\n",
                   "Optimal", optimal.hits, optimal.faults,
                   optimal.faults * 100.0 / n);

            printf("\nLowest page-fault count: ");
            int minimum = fifo.faults;
            if (lru.faults < minimum)
                minimum = lru.faults;
            if (optimal.faults < minimum)
                minimum = optimal.faults;

            if (fifo.faults == minimum)
                printf("FIFO ");
            if (lru.faults == minimum)
                printf("LRU ");
            if (optimal.faults == minimum)
                printf("Optimal ");
            printf("\n");
        } else if (choice != 0) {
            printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    printf("Visualizer closed.\n");
    return 0;
}
