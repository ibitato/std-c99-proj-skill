/*!
 * \file test_arena.c
 * \brief Memory arena tests — pure ANSI C99. Must pass Valgrind clean.
 */

#include <stdio.h>
#include <string.h>
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
    (void)mem_arena_init(&a, 4096);
    p = (int *)mem_arena_alloc(&a, 10 * sizeof(int));
    ASSERT(p != NULL,                       "alloc 10 ints");
    ASSERT(a.used == 10 * sizeof(int),      "used updated after alloc");
    mem_arena_free(&a);
}

static void test_alloc_fill(void) {
    MemArena a;
    void *p1;
    void *p2;
    (void)mem_arena_init(&a, 64);
    p1 = mem_arena_alloc(&a, 64);
    ASSERT(p1 != NULL,                      "alloc exactly full capacity");
    p2 = mem_arena_alloc(&a, 1);
    ASSERT(p2 == NULL,                      "alloc fails when full");
    mem_arena_free(&a);
}

static void test_reset(void) {
    MemArena a;
    (void)mem_arena_init(&a, 256);
    (void)mem_arena_alloc(&a, 100);
    mem_arena_reset(&a);
    ASSERT(a.used == 0,                     "used is 0 after reset");
    ASSERT(a.size == 256,                   "size unchanged after reset");
    /* Can allocate again after reset */
    ASSERT(mem_arena_alloc(&a, 256) != NULL, "alloc full after reset");
    mem_arena_free(&a);
}

static void test_edge_cases(void) {
    MemArena a;
    ASSERT(mem_arena_init(NULL, 1024) == -1,  "init NULL arena fails");
    ASSERT(mem_arena_init(&a, 0) == -1,       "init size 0 fails");
    ASSERT(mem_arena_alloc(NULL, 10) == NULL,  "alloc NULL arena fails");
    (void)mem_arena_init(&a, 128);
    ASSERT(mem_arena_alloc(&a, 0) == NULL,     "alloc size 0 returns NULL");
    mem_arena_free(&a);
    /* Double free is safe */
    mem_arena_free(&a);
}

int main(void) {
    test_init_free();
    test_alloc_basic();
    test_alloc_fill();
    test_reset();
    test_edge_cases();

    (void)printf("\n%d/%d tests passed\n", tests_run - tests_failed, tests_run);
    return tests_failed > 0 ? 1 : 0;
}
