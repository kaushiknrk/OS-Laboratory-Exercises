
#define MAX_CACHE_SIZE  4096

struct lru_cache_data {
    int val;
    bool used;
    struct lru_cache_data *prev, *next;
};

typedef struct {
    struct lru_cache_data cache[MAX_CACHE_SIZE], *head, *tail;
    size_t cp, sz;
} LRUCache;


LRUCache* lRUCacheCreate(int capacity) {
    LRUCache *c = calloc(1, sizeof(LRUCache));
    c && (c->cp = capacity);
    return c;
}

static void lru_cache_move_to_front(LRUCache *obj, struct lru_cache_data *c) {
    if (c == obj->head)
        return;       
    struct lru_cache_data **h = &obj->head, **t = &obj->tail;    
    c == *t && (*t = (*t)->prev);
    c->prev && (c->prev->next = c->next);
    c->next && (c->next->prev = c->prev);
    c->prev = NULL;
    c->next = *h;
    *h && ((*h)->prev = c) || (*t = c);
    *h = c;
}

int lRUCacheGet(LRUCache* obj, int key) {
    struct lru_cache_data *c = &obj->cache[key];
    c->used && (lru_cache_move_to_front(obj, c), 1);
    return c->used ? c->val : -1;
}

static void lru_cache_evict(LRUCache *obj) {
    struct lru_cache_data **h = &obj->head, **t = &obj->tail, *e = *t;
    !(*t = (*t)->prev) && (*h = NULL, 1) || ((*t)->next = NULL);
    e->used = false;
    e->prev = e->next = NULL;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    struct lru_cache_data *c = &obj->cache[key];
    !c->used && (obj->sz == obj->cp && (lru_cache_evict(obj) , 1) || obj->sz++);
    c->used = true;
    c->val = value;
    lru_cache_move_to_front(obj, c);    
}

void lRUCacheFree(LRUCache* obj) {
    free(obj);
}
