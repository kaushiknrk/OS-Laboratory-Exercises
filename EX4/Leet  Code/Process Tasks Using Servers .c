#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int weight;
    int index;
} FreeServer;

typedef struct {
    long long freeTime;
    int weight;
    int index;
} BusyServer;

void swapFree(FreeServer* a, FreeServer* b) {
    FreeServer temp = *a;
    *a = *b;
    *b = temp;
}

void pushFree(FreeServer* heap, int* heapSize, int weight, int index) {
    heap[*heapSize].weight = weight;
    heap[*heapSize].index = index;
    int i = *heapSize;
    (*heapSize)++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[i].weight < heap[parent].weight || 
           (heap[i].weight == heap[parent].weight && heap[i].index < heap[parent].index)) {
            swapFree(&heap[i], &heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

FreeServer popFree(FreeServer* heap, int* heapSize) {
    FreeServer top = heap[0];
    (*heapSize)--;
    heap[0] = heap[*heapSize];
    int i = 0;
    while (2 * i + 1 < *heapSize) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;
        if (right < *heapSize) {
            if (heap[right].weight < heap[left].weight || 
               (heap[right].weight == heap[left].weight && heap[right].index < heap[left].index)) {
                smallest = right;
            }
        }
        if (heap[smallest].weight < heap[i].weight || 
           (heap[smallest].weight == heap[i].weight && heap[smallest].index < heap[i].index)) {
            swapFree(&heap[i], &heap[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

void swapBusy(BusyServer* a, BusyServer* b) {
    BusyServer temp = *a;
    *a = *b;
    *b = temp;
}

void pushBusy(BusyServer* heap, int* heapSize, long long freeTime, int weight, int index) {
    heap[*heapSize].freeTime = freeTime;
    heap[*heapSize].weight = weight;
    heap[*heapSize].index = index;
    int i = *heapSize;
    (*heapSize)++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[i].freeTime < heap[parent].freeTime || 
           (heap[i].freeTime == heap[parent].freeTime && heap[i].weight < heap[parent].weight) ||
           (heap[i].freeTime == heap[parent].freeTime && heap[i].weight == heap[parent].weight && heap[i].index < heap[parent].index)) {
            swapBusy(&heap[i], &heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

BusyServer popBusy(BusyServer* heap, int* heapSize) {
    BusyServer top = heap[0];
    (*heapSize)--;
    heap[0] = heap[*heapSize];
    int i = 0;
    while (2 * i + 1 < *heapSize) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;
        if (right < *heapSize) {
            if (heap[right].freeTime < heap[left].freeTime || 
               (heap[right].freeTime == heap[left].freeTime && heap[right].weight < heap[left].weight) ||
               (heap[right].freeTime == heap[left].freeTime && heap[right].weight == heap[left].weight && heap[right].index < heap[left].index)) {
                smallest = right;
            }
        }
        if (heap[smallest].freeTime < heap[i].freeTime || 
           (heap[smallest].freeTime == heap[i].freeTime && heap[smallest].weight < heap[i].weight) ||
           (heap[smallest].freeTime == heap[i].freeTime && heap[smallest].weight == heap[i].weight && heap[smallest].index < heap[i].index)) {
            swapBusy(&heap[i], &heap[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

int* assignTasks(int* servers, int serversSize, int* tasks, int tasksSize, int* returnSize) {
    FreeServer* freeHeap = (FreeServer*)malloc(serversSize * sizeof(FreeServer));
    int freeSize = 0;
    for (int i = 0; i < serversSize; i++) {
        pushFree(freeHeap, &freeSize, servers[i], i);
    }
    
    BusyServer* busyHeap = (BusyServer*)malloc(serversSize * sizeof(BusyServer));
    int busySize = 0;
    
    int* ans = (int*)malloc(tasksSize * sizeof(int));
    *returnSize = tasksSize;
    
    long long currentTime = 0;
    for (int j = 0; j < tasksSize; j++) {
        if (currentTime < j) {
            currentTime = j;
        }
        
        while (busySize > 0 && busyHeap[0].freeTime <= currentTime) {
            BusyServer b = popBusy(busyHeap, &busySize);
            pushFree(freeHeap, &freeSize, b.weight, b.index);
        }
        
        if (freeSize == 0 && busySize > 0) {
            currentTime = busyHeap[0].freeTime;
            while (busySize > 0 && busyHeap[0].freeTime <= currentTime) {
                BusyServer b = popBusy(busyHeap, &busySize);
                pushFree(freeHeap, &freeSize, b.weight, b.index);
            }
        }
        
        FreeServer chosen = popFree(freeHeap, &freeSize);
        ans[j] = chosen.index;
        pushBusy(busyHeap, &busySize, currentTime + tasks[j], chosen.weight, chosen.index);
    }
    
    free(freeHeap);
    free(busyHeap);
    return ans;
}
