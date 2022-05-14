#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_RC(r, t) (*(t*)((r)->data))
#define LNIL (lisp_val) {.type = lispt_nil, .val = NULL}
#define LINT(n) (lisp_val) {.type = lispt_int, .val = (void*) n}
#define LFLOAT(n) (lisp_val) {.type = lispt_float, .val = d2p(n)}
#define LSYM(n) (lisp_val) {.type = lispt_sym, .val = (void*) n}
#define LLIST(n) (lisp_val) {.type = lispt_list, .val = (void*) n}

typedef enum {
    lispt_nil,
    lispt_int,
    lispt_float,
    lispt_str,
    lispt_list,
    lispt_sym
} lisp_type;

typedef struct {
    lisp_type type;
    void* val;
} lisp_val;

typedef union {
    double x;
    void* p;
} dpu;

typedef struct {
    uint64_t refs;
    char data[];
} rc;

typedef lisp_val lisp_func(rc*);

typedef struct {
    size_t len;
    char data[];
} stri;

typedef struct {
    lisp_val head;
    rc* tail;
} listi;

void* d2p(double x) {
    dpu u;
    u.x = x;
    return u.p;
}

double p2d(void* p) {
    dpu u;
    u.p = p;
    return u.x;
}

void droprc(rc* p) {
    p->refs--;
    if (p->refs == 0) {
        free(p);
    }
}

rc* mkrc(size_t n) {
    return malloc(sizeof(rc) + n * sizeof(char));
}

rc* mklist(lisp_val h) {
    rc* l = mkrc(sizeof(listi));
    l->refs = 1;
    READ_RC(l, listi).head = h;
    READ_RC(l, listi).tail = NULL;
    return l;
}

rc* mkcons(lisp_val h, rc* t) {
    rc* l = mklist(h);
    READ_RC(l, listi).tail = t;
    return l;
}

void droplist(rc* l);

void dropval(lisp_val v) {
    if (v.type == lispt_list) {
        droplist(v.val);
    }
}

void droplist(rc* l) {
    if (l == NULL) {
        return;
    }
    l->refs --;
    if (l->refs == 0) {
        dropval(READ_RC(l, listi).head);
        if (READ_RC(l, listi).tail != NULL) {
            droplist(READ_RC(l, listi).tail);
        }
        free(l);
    }
}

// assumes a->tail == NULL
void cons(rc* a, rc* b) {
    READ_RC(a, listi).tail = b;
    b->refs ++;
}

lisp_val lf_dbg(rc* args) {
    printf("in dbg\n");
    rc* ptr = args;
    while (ptr != NULL) {
        printf("waw %p -> %p\n", (void*) ptr, (void*) READ_RC(ptr, listi).head.val);
        ptr = READ_RC(ptr, listi).tail;
    }
    return LNIL;
}

lisp_func* funcs[256] = {lf_dbg};

void panic(char* msg) {
    fputs(msg, stderr);
    exit(1);
}

lisp_val run_lisp(lisp_val l) {
    if (l.type != lispt_list) {
        return l;
    }
    rc* ptr = l.val;
    if (ptr == NULL) {
        panic("Cannot run empty list.\n");
    }
    lisp_val h = run_lisp(READ_RC(ptr, listi).head);
    if (h.type != lispt_sym) {
        panic("Cannot run invalid list.\n");
    }
    lisp_func* f = funcs[(uint8_t)(uintptr_t) h.val];
    if (f == NULL) {
        panic("Invalid function.\n");
    }
    rc* evaled_list = NULL;
    rc* evaled_tail = NULL;
    rc* cur = READ_RC(ptr, listi).tail;
    while (cur != NULL) {
        rc* tmp = mklist(run_lisp(READ_RC(cur, listi).head));
        if (evaled_list == NULL) {
            evaled_list = tmp;
            evaled_tail = tmp;
        } else {
            READ_RC(evaled_tail, listi).tail = tmp;
            evaled_tail = tmp;
        }
        cur = READ_RC(cur, listi).tail;
    }
    lisp_val ret = f(evaled_list);
    droplist(evaled_list);
    return ret;
}


int main(void) {
    rc* lmao0 = mkcons(LSYM(0), mkcons(LINT(1), mkcons(LINT(2), mklist(LINT(3)))));
    run_lisp(LLIST(lmao0));
    droplist(lmao0);
    return 0;
}
