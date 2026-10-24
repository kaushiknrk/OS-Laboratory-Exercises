#include <stdio.h>

int frames[100];
int frameCount;

void initFrames()
{
    int i;
    for (i = 0; i < frameCount; i++)
        frames[i] = -1;
}

int isPagePresent(int page)
{
    int i;
    for (i = 0; i < frameCount; i++)
    {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

void displayFrames()
{
    int i;
    for (i = 0; i < frameCount; i++)
    {
        if (frames[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frames[i]);
    }
}

void fifo(int pages[], int pageCount)
{
    int faults = 0;
    int hits = 0;
    int pointer = 0;
    int i;

    initFrames();

    printf("\nFIFO PAGE REPLACEMENT\n");
    printf("\nPage\tFrames\t\t\tStatus\n");

    for (i = 0; i < pageCount; i++)
    {
        printf("%d\t", pages[i]);

        if (isPagePresent(pages[i]))
        {
            hits++;
            displayFrames();
            printf("\tHit\n");
        }
        else
        {
            frames[pointer] = pages[i];
            pointer = (pointer + 1) % frameCount;
            faults++;
            displayFrames();
            printf("\tFault\n");
        }
    }

    printf("\nTotal Page Faults : %d\n", faults);
    printf("Total Page Hits   : %d\n", hits);
    printf("Hit Ratio         : %.2f%%\n", (float)hits / pageCount * 100);
    printf("Fault Ratio       : %.2f%%\n", (float)faults / pageCount * 100);
}

void lru(int pages[], int pageCount)
{
    int faults = 0;
    int hits = 0;
    int recent[100];
    int i, j;

    initFrames();

    printf("\nLRU PAGE REPLACEMENT\n");
    printf("\nPage\tFrames\t\t\tStatus\n");

    for (i = 0; i < pageCount; i++)
    {
        printf("%d\t", pages[i]);

        if (isPagePresent(pages[i]))
        {
            hits++;

            for (j = 0; j < frameCount; j++)
            {
                if (frames[j] == pages[i])
                {
                    recent[j] = i;
                    break;
                }
            }

            displayFrames();
            printf("\tHit\n");
        }
        else
        {
            int placed = 0;
            for (j = 0; j < frameCount; j++)
            {
                if (frames[j] == -1)
                {
                    frames[j] = pages[i];
                    recent[j] = i;
                    placed = 1;
                    break;
                }
            }

            if (!placed)
            {
                int lruIndex = 0;
                int minRecent = recent[0];

                for (j = 1; j < frameCount; j++)
                {
                    if (recent[j] < minRecent)
                    {
                        minRecent = recent[j];
                        lruIndex = j;
                    }
                }

                frames[lruIndex] = pages[i];
                recent[lruIndex] = i;
            }

            faults++;
            displayFrames();
            printf("\tFault\n");
        }
    }

    printf("\nTotal Page Faults : %d\n", faults);
    printf("Total Page Hits   : %d\n", hits);
    printf("Hit Ratio         : %.2f%%\n", (float)hits / pageCount * 100);
    printf("Fault Ratio       : %.2f%%\n", (float)faults / pageCount * 100);
}

void optimal(int pages[], int pageCount)
{
    int faults = 0;
    int hits = 0;
    int i, j, k;

    initFrames();

    printf("\nOPTIMAL PAGE REPLACEMENT\n");
    printf("\nPage\tFrames\t\t\tStatus\n");

    for (i = 0; i < pageCount; i++)
    {
        printf("%d\t", pages[i]);

        if (isPagePresent(pages[i]))
        {
            hits++;
            displayFrames();
            printf("\tHit\n");
        }
        else
        {
            int placed = 0;
            for (j = 0; j < frameCount; j++)
            {
                if (frames[j] == -1)
                {
                    frames[j] = pages[i];
                    placed = 1;
                    break;
                }
            }

            if (!placed)
            {
                int replaceIndex = 0;
                int farthest = -1;

                for (j = 0; j < frameCount; j++)
                {
                    int nextUse = -1;

                    for (k = i + 1; k < pageCount; k++)
                    {
                        if (pages[k] == frames[j])
                        {
                            nextUse = k;
                            break;
                        }
                    }

                    if (nextUse == -1)
                    {
                        replaceIndex = j;
                        break;
                    }

                    if (nextUse > farthest)
                    {
                        farthest = nextUse;
                        replaceIndex = j;
                    }
                }

                frames[replaceIndex] = pages[i];
            }

            faults++;
            displayFrames();
            printf("\tFault\n");
        }
    }

    printf("\nTotal Page Faults : %d\n", faults);
    printf("Total Page Hits   : %d\n", hits);
    printf("Hit Ratio         : %.2f%%\n", (float)hits / pageCount * 100);
    printf("Fault Ratio       : %.2f%%\n", (float)faults / pageCount * 100);
}

int main()
{
    int pages[100];
    int pageCount, option;
    int i;
    char choice;

    printf("Enter number of frames: ");
    scanf("%d", &frameCount);

    printf("Enter number of pages: ");
    scanf("%d", &pageCount);

    printf("Enter the page reference string:\n");
    for (i = 0; i < pageCount; i++)
    {
        printf("Page %d: ", i + 1);
        scanf("%d", &pages[i]);
    }

    do
    {
        printf("\nPAGE REPLACEMENT ALGORITHMS\n");
        printf("1. FIFO (First In First Out)\n");
        printf("2. LRU (Least Recently Used)\n");
        printf("3. Optimal\n");
        printf("4. Compare All\n");
        printf("5. Exit\n");

        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                fifo(pages, pageCount);
                break;

            case 2:
                lru(pages, pageCount);
                break;

            case 3:
                optimal(pages, pageCount);
                break;

            case 4:
                fifo(pages, pageCount);
                printf("\n");
                lru(pages, pageCount);
                printf("\n");
                optimal(pages, pageCount);
                break;

            case 5:
                printf("Exiting program.\n");
                break;

            default:
                printf("\nInvalid option selected!\n");
        }

        if (option >= 1 && option <= 4)
        {
            printf("\nDo you want to try another method? (y/n): ");
            scanf(" %c", &choice);
        }
        else if (option == 5)
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
