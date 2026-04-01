/*!
 * \file main.c
 * \brief Entry point — pure ANSI C99 with memory arena.
 */

#include <stdio.h>
#include "mem_arena.h"
#include "utils.h"

#define ARENA_SIZE ((size_t)(1024 * 1024))  /* 1 MiB */

int main(void) {
    MemArena arena;
    int i;

    arena.memory = NULL;
    if (mem_arena_init(&arena, ARENA_SIZE) != 0) {
        (void)fprintf(stderr, "Error: arena init failed\n");
        return 1;
    }

    int *arr = (int *)mem_arena_alloc(&arena, 10 * sizeof(int));
    if (arr == NULL) {
        mem_arena_free(&arena);
        return 1;
    }

    for (i = 0; i < 10; i++) {
        arr[i] = i * 2;
    }

    print_message("Array allocated in memory arena:");
    for (i = 0; i < 10; i++) {
        (void)printf("  [%d] = %d\n", i, arr[i]);
    }

    mem_arena_free(&arena);
    return 0;
}
