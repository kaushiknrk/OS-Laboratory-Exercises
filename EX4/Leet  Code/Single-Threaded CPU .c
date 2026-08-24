#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int enqueueTime;
    int processingTime;
    int originalIndex;
} Task;

typedef struct {
    int processingTime;
    int originalIndex;
} HeapNode;

int compareTasks(const void* a, const void* b) {
    Task* t1 = (Task*)a;
    Task* t2 = (Task*)b;
    if (t1->enqueueTime != t2->enqueueTime) {
        return (t1->enqueueTime > t2->enqueueTime) - (t1->enqueueTime < t2->enqueueTime);
    }
    return (t1->originalIndex > t2->originalIndex) - (t1->originalIndex < t2->originalIndex);
}

void swapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void pushToHeap(HeapNode* heap, int* heapSize, int procTime, int origIdx) {
    heap[*heapSize].processingTime = procTime;
    heap[*heapSize].originalIndex = origIdx;
    int i = *heapSize;
    (*heapSize)++;
    
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[i].processingTime < heap[parent].processingTime || 
           (heap[i].processingTime == heap[parent].processingTime && heap[i].originalIndex < heap[parent].originalIndex)) {
            swapNodes(&heap[i], &heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

HeapNode popFromHeap(HeapNode* heap, int* heapSize) {
    HeapNode top = heap[0];
    (*heapSize)--;
    heap[0] = heap[*heapSize];
    
    int i = 0;
    while (2 * i + 1 < *heapSize) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;
        
        if (right < *heapSize) {
            if (heap[right].processingTime < heap[left].processingTime || 
               (heap[right].processingTime == heap[left].processingTime && heap[right].originalIndex < heap[left].originalIndex)) {
                smallest = right;
            }
        }
        
        if (heap[smallest].processingTime < heap[i].processingTime || 
           (heap[smallest].processingTime == heap[i].processingTime && heap[smallest].originalIndex < heap[i].originalIndex)) {
            swapNodes(&heap[i], &heap[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

int* getOrder(int** tasks, int tasksSize, int* tasksColSize, int* returnSize) {
    Task* sortedTasks = (Task*)malloc(tasksSize * sizeof(Task));
    for (int i = 0; i < tasksSize; i++) {
        sortedTasks[i].enqueueTime = tasks[i][0];
        sortedTasks[i].processingTime = tasks[i][1];
        sortedTasks[i].originalIndex = i;
    }
    
    qsort(sortedTasks, tasksSize, sizeof(Task), compareTasks);
    
    HeapNode* heap = (HeapNode*)malloc(tasksSize * sizeof(HeapNode));
    int heapSize = 0;
    
    int* result = (int*)malloc(tasksSize * sizeof(int));
    *returnSize = tasksSize;
    int resultIdx = 0;
    
    long long currentTime = 0; 
    int taskIdx = 0;
    
    while (taskIdx < tasksSize || heapSize > 0) {
        if (heapSize == 0 && currentTime < sortedTasks[taskIdx].enqueueTime) {
            currentTime = sortedTasks[taskIdx].enqueueTime;
        }
        
        while (taskIdx < tasksSize && sortedTasks[taskIdx].enqueueTime <= currentTime) {
            pushToHeap(heap, &heapSize, sortedTasks[taskIdx].processingTime, sortedTasks[taskIdx].originalIndex);
            taskIdx++;
        }
        
        HeapNode currentTask = popFromHeap(heap, &heapSize);
        result[resultIdx++] = currentTask.originalIndex;
        currentTime += currentTask.processingTime; 
    }
    
    free(sortedTasks);
    free(heap);
    
    return result;
}
