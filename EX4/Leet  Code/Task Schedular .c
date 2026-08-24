#include <stdio.h>
#include <string.h>

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

int leastInterval(char* tasks, int tasksSize, int n) {
    int frequencies[26] = {0};
    for (int i = 0; i < tasksSize; i++) {
        frequencies[tasks[i] - 'A']++;
    }

    int max_freq = 0;
    for (int i = 0; i < 26; i++) {
        if (frequencies[i] > max_freq) {
            max_freq = frequencies[i];
        }
    }

    int max_freq_count = 0;
    for (int i = 0; i < 26; i++) {
        if (frequencies[i] == max_freq) {
            max_freq_count++;
        }
    }

    int part_count = max_freq - 1;
    int part_length = n - (max_freq_count - 1);
    int empty_slots = part_count * MAX(0, part_length);
    int available_tasks = tasksSize - (max_freq * max_freq_count);
    int idles = MAX(0, empty_slots - available_tasks);

    return tasksSize + idles;
}
