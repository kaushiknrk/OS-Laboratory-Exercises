
#include <immintrin.h>
#include <stdalign.h>

struct sample {
    alignas(64) char lock;
    alignas(64) int c;
} alignas(64);

typedef struct sample H2O;

void releaseHydrogen();
void releaseOxygen();

H2O* h2oCreate() {
    H2O* obj = malloc(sizeof(H2O));   
    obj->c = 0;
    obj->lock = 0;   
    return obj;
}

static inline void lock_acquire(char *lock) {
    do
        while (__atomic_load_n(lock, __ATOMIC_RELAXED))
            sleep(0.5);
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE));
}

static inline void lock_release(char *lock) {
    __atomic_clear(lock, __ATOMIC_RELEASE);        
}

static inline void hydoxy(H2O *obj, char m) {
    int c = 1, a;
    while (c) {
        lock_acquire(&obj->lock);        
        int v = __atomic_load_n(&obj->c, __ATOMIC_RELAXED);
        if ((c = (m == 'H' ? v == 2 : v != 2)))
            lock_release(&obj->lock);
    }
    m == 'H' ? (releaseHydrogen(), a = 1) : (releaseOxygen(), a = -2);
    __atomic_fetch_add(&obj->c, a, __ATOMIC_RELAXED);    
    lock_release(&obj->lock);
}

void hydrogen(H2O* obj) {
    hydoxy(obj, 'H');
}

void oxygen(H2O* obj) {
    hydoxy(obj, 'O');
}

void h2oFree(H2O* obj) {   
    free(obj);
}
