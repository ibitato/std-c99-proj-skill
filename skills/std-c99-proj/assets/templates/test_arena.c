/*!
 * \file test_arena.c
 * \brief Memory arena tests — pure ANSI C99, hardened. Must pass Valgrind clean.
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "mem_arena.h"

static int tests_run    = 0;
static int tests_failed = 0;

#define ASSERT(cond, msg) do { \
    tests_run++; \
    if (!(cond)) { \
        tests_failed++; \
        (void)fprintf(stderr, "FAIL [%d]: %s\n", tests_run, (msg)); \
    } else { \
        (void)printf("PASS [%d]: %s\n", tests_run, (msg)); \
    } \
} while (0)

static void test_init_free(void) {
    MemArena a;
    a.memory = NULL; /* required before first init */
    ASSERT(mem_arena_init(&a, 1024) == 0,  "init 1024 bytes");
    ASSERT(a.size == 1024,                  "size is 1024");
    ASSERT(a.used == 0,                     "used is 0 after init");
    mem_arena_free(&a);
    ASSERT(a.memory == NULL,                "memory is NULL after free");
    ASSERT(a.size == 0,                     "size is 0 after free");
}

static void test_alloc_basic(void) {
    MemArena a;
    int *p;
    a.memory = NULL;
    (void)mem_arena_init(&a, 4096);
    p = (int *)mem_arena_alloc(&a, 10 * sizeof(int));
    ASSERT(p != NULL,                       "alloc 10 ints");
    ASSERT(a.used >= 10 * sizeof(int),      "used updated after alloc");
    mem_arena_free(&a);
}

static void test_alloc_fill(void) {
    MemArena a;
    void *p1;
    void *p2;
    a.memory = NULL;
    /* Use aligned size so fill is exact */
    (void)mem_arena_init(&a, MEM_ARENA_ALIGN * 4);
    p1 = mem_arena_alloc(&a, MEM_ARENA_ALIGN * 4);
    ASSERT(p1 != NULL,                      "alloc exactly full capacity");
    p2 = mem_arena_alloc(&a, 1);
    ASSERT(p2 == NULL,                      "alloc fails when full");
    mem_arena_free(&a);
}

static void test_reset(void) {
    MemArena a;
    size_t cap;
    a.memory = NULL;
    cap = MEM_ARENA_ALIGN * 16;
    (void)mem_arena_init(&a, cap);
    (void)mem_arena_alloc(&a, 100);
    mem_arena_reset(&a);
    ASSERT(a.used == 0,                     "used is 0 after reset");
    ASSERT(a.size == cap,                   "size unchanged after reset");
    ASSERT(mem_arena_alloc(&a, cap) != NULL, "alloc full after reset");
    mem_arena_free(&a);
}

static void test_alignment(void) {
    MemArena a;
    void *p1;
    void *p2;
    size_t addr;
    a.memory = NULL;
    (void)mem_arena_init(&a, 4096);

    /* Alloc odd size, next alloc must still be aligned */
    p1 = mem_arena_alloc(&a, 3);
    p2 = mem_arena_alloc(&a, sizeof(double));
    ASSERT(p1 != NULL,                      "alloc 3 bytes");
    ASSERT(p2 != NULL,                      "alloc double after odd size");
    addr = (size_t)(const char *)p2;
    ASSERT(addr % MEM_ARENA_ALIGN == 0,     "second alloc is aligned");

    /* Alloc 1 byte, check alignment of next */
    p1 = mem_arena_alloc(&a, 1);
    p2 = mem_arena_alloc(&a, sizeof(int));
    ASSERT(p1 != NULL && p2 != NULL,        "alloc 1 + int");
    addr = (size_t)(const char *)p2;
    ASSERT(addr % MEM_ARENA_ALIGN == 0,     "int alloc after 1 byte is aligned");

    mem_arena_free(&a);
}

static void test_overflow(void) {
    MemArena a;
    a.memory = NULL;
    (void)mem_arena_init(&a, 256);

    /* Request near SIZE_MAX — must not wrap around */
    ASSERT(mem_arena_alloc(&a, (size_t)-1) == NULL,  "alloc SIZE_MAX fails");
    ASSERT(mem_arena_alloc(&a, (size_t)-16) == NULL,  "alloc near SIZE_MAX fails");
    /* Arena still usable after failed allocs */
    ASSERT(mem_arena_alloc(&a, 8) != NULL,            "arena usable after overflow reject");

    mem_arena_free(&a);
}

static void test_double_init(void) {
    MemArena a;
    a.memory = NULL;
    (void)mem_arena_init(&a, 128);
    /* Second init without free must fail (prevents leak) */
    ASSERT(mem_arena_init(&a, 256) == -1,   "double init rejected");
    ASSERT(a.size == 128,                    "original arena unchanged");
    mem_arena_free(&a);
}

static void test_edge_cases(void) {
    MemArena a;
    a.memory = NULL;
    ASSERT(mem_arena_init(NULL, 1024) == -1,  "init NULL arena fails");
    ASSERT(mem_arena_init(&a, 0) == -1,       "init size 0 fails");
    ASSERT(mem_arena_alloc(NULL, 10) == NULL,  "alloc NULL arena fails");
    (void)mem_arena_init(&a, 128);
    ASSERT(mem_arena_alloc(&a, 0) == NULL,     "alloc size 0 returns NULL");
    mem_arena_free(&a);
    /* Double free is safe */
    mem_arena_free(&a);
    /* Alloc on freed arena returns NULL */
    ASSERT(mem_arena_alloc(&a, 8) == NULL,     "alloc on freed arena fails");
    /* Reset on freed arena is safe */
    mem_arena_reset(&a);
}

int main(void) {
    test_init_free();
    test_alloc_basic();
    test_alloc_fill();
    test_reset();
    test_alignment();
    test_overflow();
    test_double_init();
    test_edge_cases();

    (void)printf("\n%d/%d tests passed\n", tests_run - tests_failed, tests_run);
    return tests_failed > 0 ? 1 : 0;
}
