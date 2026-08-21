#ifndef SHARED_H
#define SHARED_H

#define SIZE 10
#define SHM_KEY 12345 

struct shared {
    int buffer[SIZE];
    int in;
    int out;
    int count;
};

#endif
