#include <stdio.h>
#include <math.h>

int main()
{
    int processSize, pageSize, frameSize, numPages, numFrames;
    int totalMemory, internalFrag;
    int pageTable[100];
    int frameList[100];      
    int option;
    char choice;
    int i;

    printf("Enter total physical memory size (in bytes): ");
    scanf("%d", &totalMemory);

    printf("Enter process size (in bytes): ");
    scanf("%d", &processSize);

    printf("Enter page size (in bytes): ");
    scanf("%d", &pageSize);

    frameSize = pageSize;

    numPages = processSize / pageSize;
    if (processSize % pageSize != 0)
        numPages++;

    numFrames = totalMemory / frameSize;

    internalFrag = (numPages * pageSize) - processSize;

    if (numPages > numFrames)
    {
        printf("\nError: Process requires %d pages but only %d frames available.\n",
               numPages, numFrames);
        return 1;
    }

    for (i = 0; i < numFrames; i++)
    {
        frameList[i] = -1;
    }

    printf("\nEnter frame number for each page:\n");
    for (i = 0; i < numPages; i++)
    {
        printf("Page %d -> Frame: ", i);
        scanf("%d", &pageTable[i]);

        if (pageTable[i] < 0 || pageTable[i] >= numFrames)
        {
            printf("Invalid frame number! Must be between 0 and %d\n", numFrames - 1);
            i--;
        }
        else if (frameList[pageTable[i]] != -1)
        {
            printf("Error: Frame %d is already allocated to Page %d! Choose another frame.\n",
                   pageTable[i], frameList[pageTable[i]]);
            i--;
        }
        else
        {
            frameList[pageTable[i]] = i;
        }
    }

    do
    {
        printf("\nPAGING TECHNIQUE\n");
        printf("1. Display Page Table\n");
        printf("2. Display Number of Pages\n");
        printf("3. Display Internal Fragmentation\n");
        printf("4. Logical to Physical Address Translation\n");
        printf("5. Display All Details\n");
        printf("6. Display Frame List\n");  
        printf("7. Exit\n");                

        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                printf("\nPAGE TABLE\n");
                printf("Page No\t\tFrame No\n");
                for (i = 0; i < numPages; i++)
                {
                    printf("%d\t\t%d\n", i, pageTable[i]);
                }
                break;

            case 2:
                printf("\nTotal pages required: %d\n", numPages);
                printf("Total frames in memory: %d\n", numFrames);
                break;

            case 3:
                printf("\nTotal memory allocated: %d bytes (%d pages x %d bytes)\n",
                       numPages * pageSize, numPages, pageSize);
                printf("Actual process size: %d bytes\n", processSize);
                printf("Internal Fragmentation: %d bytes\n", internalFrag);

                if (internalFrag == 0)
                    printf("No internal fragmentation (process fits exactly)\n");
                else
                    printf("Wasted space in last page: %d out of %d bytes\n",
                           internalFrag, pageSize);
                break;

            case 4:
            {
                int logicalAddr, pageNum, offset;
                int physicalAddr, frameNum;

                printf("\nEnter logical address: ");
                scanf("%d", &logicalAddr);

                if (logicalAddr < 0 || logicalAddr >= numPages * pageSize)
                {
                    printf("Invalid logical address! Must be between 0 and %d\n",
                           (numPages * pageSize) - 1);
                    break;
                }

                pageNum = logicalAddr / pageSize;
                offset = logicalAddr % pageSize;

                frameNum = pageTable[pageNum];

                physicalAddr = (frameNum * frameSize) + offset;

                printf("\nADDRESS TRANSLATION\n");
                printf("Logical Address  : %d\n", logicalAddr);
                printf("Page Number      : %d\n", pageNum);
                printf("Offset           : %d\n", offset);
                printf("Frame Number     : %d (from page table)\n", frameNum);
                printf("Physical Address : %d x %d + %d = %d\n",
                       frameNum, frameSize, offset, physicalAddr);
                break;
            }

            case 5:
                printf("\nCOMPLETE PAGING DETAILS\n");

                printf("\nMemory Configuration:\n");
                printf("Total Physical Memory : %d bytes\n", totalMemory);
                printf("Page Size             : %d bytes\n", pageSize);
                printf("Frame Size            : %d bytes\n", frameSize);
                printf("Total Frames          : %d\n", numFrames);

                printf("\nProcess Details:\n");
                printf("Process Size          : %d bytes\n", processSize);
                printf("Pages Required        : %d\n", numPages);
                printf("Memory Allocated      : %d bytes\n", numPages * pageSize);
                printf("Internal Fragmentation: %d bytes\n", internalFrag);

                printf("\nPage Table:\n");
                printf("Page No\t\tFrame No\tBase Address\n");
                for (i = 0; i < numPages; i++)
                {
                    printf("%d\t\t%d\t\t%d\n", i, pageTable[i],
                           pageTable[i] * frameSize);
                }

                // ADDED: Display frame list inside All Details
                printf("\nFrame List (Frame Allocation Status):\n");
                printf("Frame No\tStatus\t\tAllocated To\n");
                for (i = 0; i < numFrames; i++)
                {
                    if (frameList[i] == -1)
                        printf("%d\t\tFREE\t\t--\n", i);
                    else
                        printf("%d\t\tOCCUPIED\tPage %d\n", i, frameList[i]);
                }
                break;

            // ADDED: New case to display only the Frame List
            case 6:
                printf("\nFRAME LIST (Frame Allocation Status)\n");
                printf("Frame No\tStatus\t\tAllocated To (Page No)\n");
                for (i = 0; i < numFrames; i++)
                {
                    if (frameList[i] == -1)
                        printf("%d\t\tFREE\t\t--\n", i);
                    else
                        printf("%d\t\tOCCUPIED\t%d\n", i, frameList[i]);
                }
                printf("\nTotal Frames: %d\n", numFrames);
                printf("Used Frames : %d\n", numPages);
                printf("Free Frames : %d\n", numFrames - numPages);
                break;

            case 7:
                printf("Exiting program.\n");
                break;

            default:
                printf("\nInvalid option selected!\n");
        }

        if (option >= 1 && option <= 6)
        {
            printf("\nDo you want to continue? (y/n): ");
            scanf(" %c", &choice);
        }
        else if (option == 7)
        {
            choice = 'n';
        }
        else
        {
            choice = 'y';
        }

    } while (choice == 'y' || choice == 'Y');

    return 0;
}


