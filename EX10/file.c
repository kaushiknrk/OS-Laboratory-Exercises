#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 50
#define INDEX_PTRS 4   // Maximum data pointers in one index block

// Core File Meta Information
typedef struct {
    char file_name[20];
    int file_size;    // Size in bytes (user input)
    int length;       // Number of blocks needed (calculated)
    int start;
    int allocated;
} FileMeta;

// --- LINKED ALLOCATION STRUCTURES ---
typedef struct Node {
    int block_no;
    struct Node* next;
} Node;

typedef struct {
    char file_name[20];
    Node* head;
} LinkedFile;

// --- SINGLE INDEXED ALLOCATION STRUCTURES ---
typedef struct {
    int block_no;        // Disk block number of the index block
    int ptr[INDEX_PTRS]; // Pointers to data blocks
    int used;            // Number of valid data pointers
} IndexBlock;

typedef struct {
    char file_name[20];
    int file_size;
    int num_data_blocks;
    int index_block;
    IndexBlock index;
} IndexedFile;

int BLOCK_SIZE = 1024; // Will be set by user

/* Helper to get a random free block */
int getRandomFreeBlock(int freeBlock[]) {
    int count = 0;
    int available[MAX];
    for (int i = 0; i < MAX; i++) {
        if (freeBlock[i] == 1) {
            available[count++] = i;
        }
    }
    if (count == 0) return -1;
    int index = rand() % count;
    int allocated_block = available[index];
    freeBlock[allocated_block] = 0;
    return allocated_block;
}

/* Helper to find a contiguous range of free blocks (First-Fit) */
int findContiguousFreeBlocks(int freeBlock[], int length) {
    int count = 0;
    int start_candidate = -1;
    for (int i = 0; i < MAX; i++) {
        if (freeBlock[i] == 1) {
            if (count == 0) start_candidate = i;
            count++;
            if (count == length) return start_candidate;
        } else {
            count = 0;
            start_candidate = -1;
        }
    }
    return -1;
}

void resetFreeBlocks(int freeBlock[]) {
    for (int i = 0; i < MAX; i++) freeBlock[i] = 1;
}

void displayFreeBlocks(int freeBlock[]) {
    printf("\nFree Block List:\n");
    for (int i = 0; i < MAX; i++) {
        if (freeBlock[i] == 1) printf("%d ", i);
    }
    printf("\n");
}

/* 1. Contiguous Allocation */
void contiguous(FileMeta f[], int n, int freeBlock[]) {
    printf("\n--- CONTIGUOUS ALLOCATION ---\n");
    printf("Block Size: %d bytes\n\n", BLOCK_SIZE);
    printf("File\tSize(Bytes)\tBlocks\tStart\tEnd\tStatus\t\tAllocated Blocks\n");

    for (int i = 0; i < n; i++) {
        if (f[i].length == 0) {
            printf("%s\t%d\t\t0\t-\t-\tNo blocks needed\t-\n", f[i].file_name, f[i].file_size);
            continue;
        }
        int auto_start = findContiguousFreeBlocks(freeBlock, f[i].length);
        if (auto_start != -1) {
            f[i].start = auto_start;
            f[i].allocated = 1;
            int end_block = auto_start + f[i].length - 1;
            for (int j = 0; j < f[i].length; j++) {
                freeBlock[auto_start + j] = 0;
            }
            printf("%s\t%d\t\t%d\t%d\t%d\tAllocated\t",
                   f[i].file_name, f[i].file_size, f[i].length, f[i].start, end_block);
            for (int j = 0; j < f[i].length; j++) {
                printf("%d ", f[i].start + j);
            }
            printf("\n");
        } else {
            f[i].start = -1;
            f[i].allocated = 0;
            printf("%s\t%d\t\t%d\t-\t-\tFailed (No space)\n",
                   f[i].file_name, f[i].file_size, f[i].length);
        }
    }
}

/* 2. Linked Allocation */
void linked(FileMeta f[], int n, int freeBlock[]) {
    LinkedFile lf[MAX];
    printf("\n--- LINKED ALLOCATION ---\n");
    printf("Block Size: %d bytes\n\n", BLOCK_SIZE);
    printf("File\tSize(Bytes)\tBlocks\tLinked Blocks\n");

    for (int i = 0; i < n; i++) {
        strcpy(lf[i].file_name, f[i].file_name);
        lf[i].head = NULL;
        Node* tail = NULL;

        if (f[i].length == 0) {
            printf("%s\t%d\t\t0\t(No blocks allocated)\n", lf[i].file_name, f[i].file_size);
            continue;
        }

        for (int j = 0; j < f[i].length; j++) {
            int block = getRandomFreeBlock(freeBlock);
            if (block == -1) {
                printf("\nDisk full! Cannot allocate remaining blocks for %s\n", lf[i].file_name);
                break;
            }
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->block_no = block;
            newNode->next = NULL;
            if (lf[i].head == NULL) {
                lf[i].head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }

        printf("%s\t%d\t\t%d\t", lf[i].file_name, f[i].file_size, f[i].length);
        Node* temp = lf[i].head;
        while (temp != NULL) {
            printf("%d", temp->block_no);
            Node* nextNode = temp->next;
            free(temp);
            temp = nextNode;
            if (temp != NULL) printf(" -> ");
        }
        printf(" -> END\n");
    }
}

/* 3. Single Indexed Allocation */
void singleIndexed(FileMeta f[], int n, int freeBlock[]) {
    printf("\n--- SINGLE INDEXED ALLOCATION ---\n");
    printf("Block Size: %d bytes\n", BLOCK_SIZE);
    printf("Pointers per index block: %d\n\n", INDEX_PTRS);
    printf("Each file uses one index block that points to its data blocks.\n");
    printf("Maximum data blocks per file: %d\n\n", INDEX_PTRS);

    for (int i = 0; i < n; i++) {
        IndexedFile file;
        strcpy(file.file_name, f[i].file_name);
        file.file_size = f[i].file_size;
        file.num_data_blocks = f[i].length;
        file.index_block = -1;
        file.index.block_no = -1;
        file.index.used = 0;

        printf("File: %s | Size: %d bytes | Data Blocks Needed: %d\n",
               file.file_name, file.file_size, file.num_data_blocks);

        if (file.num_data_blocks == 0) {
            printf("  Empty file - no blocks allocated.\n\n");
            continue;
        }

        if (file.num_data_blocks > INDEX_PTRS) {
            printf("  Allocation failed: file needs %d data blocks, but a single index block can point to only %d.\n\n",
                   file.num_data_blocks, INDEX_PTRS);
            continue;
        }

        /* Allocate one index block first */
        int ib = getRandomFreeBlock(freeBlock);
        if (ib == -1) {
            printf("  Allocation failed: no free block for index block.\n\n");
            continue;
        }

        file.index_block = ib;
        file.index.block_no = ib;

        /* Allocate data blocks and store their numbers in the index block */
        int failed = 0;
        for (int j = 0; j < file.num_data_blocks; j++) {
            int b = getRandomFreeBlock(freeBlock);
            if (b == -1) {
                failed = 1;
                break;
            }
            file.index.ptr[file.index.used++] = b;
        }

        printf("  Index Block: %d\n", file.index_block);
        printf("  Data Blocks: ");
        for (int j = 0; j < file.index.used; j++) {
            printf("%d ", file.index.ptr[j]);
        }
        printf("\n");

        if (failed) {
            printf("  Status: PARTIALLY ALLOCATED (disk full)\n\n");
        } else {
            printf("  Status: FULLY ALLOCATED\n\n");
        }
    }
}

int main() {
    FileMeta f[MAX];
    int freeBlock[MAX];
    int n, choice;
    srand(time(NULL));
    resetFreeBlocks(freeBlock);

    printf("===== FILE ALLOCATION TECHNIQUES =====\n");
    printf("\nEnter Disk Block Size (in bytes): ");
    scanf("%d", &BLOCK_SIZE);
    if (BLOCK_SIZE <= 0) BLOCK_SIZE = 1024;

    printf("\nEnter number of files: ");
    scanf("%d", &n);

    printf("\nEnter File Name and File Size in bytes (Block size = %d bytes):\n", BLOCK_SIZE);
    for (int i = 0; i < n; i++) {
        printf("File %d [Name SizeInBytes]: ", i + 1);
        scanf("%s %d", f[i].file_name, &f[i].file_size);

        if (f[i].file_size <= 0) {
            f[i].length = 0;
        } else {
            f[i].length = (f[i].file_size + BLOCK_SIZE - 1) / BLOCK_SIZE; // Ceiling division
        }
        f[i].start = -1;
        f[i].allocated = 0;
    }

    do {
        printf("\n===== MENU =====\n");
        printf("1. Contiguous Allocation\n");
        printf("2. Linked Allocation\n");
        printf("3. Single Indexed Allocation\n");
        printf("4. Display Free Block List\n");
        printf("5. Reset Free Blocks\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                resetFreeBlocks(freeBlock);
                contiguous(f, n, freeBlock);
                break;
            case 2:
                resetFreeBlocks(freeBlock);
                linked(f, n, freeBlock);
                break;
            case 3:
                resetFreeBlocks(freeBlock);
                singleIndexed(f, n, freeBlock);
                break;
            case 4:
                displayFreeBlocks(freeBlock);
                break;
            case 5:
                resetFreeBlocks(freeBlock);
                printf("\nFree block list reset completely.\n");
                break;
            case 6:
                printf("\nProgram Ended.\n");
                break;
            default:
                printf("\nInvalid Choice!\n");
        }
    } while (choice != 6);

    return 0;
}
