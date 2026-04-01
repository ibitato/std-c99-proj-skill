/*!
 * \file mem_arena.c
 * \brief Memory arena allocator implementation — pure ANSI C99.
 */

#include "mem_arena.h"
#include <stdlib.h>
#include <stdio.h>

int mem_arena_init(MemArena *arena, size_t size) {
    if (arena == NULL || size == 0) {
        return -1;
    }
    arena->memory = (char *)malloc(size);
    if (arena->memory == NULL) {
        return -1;
    }
    arena->size = size;
    arena->used = 0;
    return 0;
}

void *mem_arena_alloc(MemArena *arena, size_t size) {
    if (arena == NULL || arena->memory == NULL || size == 0) {
        return NULL;
    }
    if (arena->used + size > arena->size) {
        (void)fprintf(stderr, "arena: out of space (need %zu, have %zu)\n",
                       size, arena->size - arena->used);
        return NULL;
    }
    void *ptr = arena->memory + arena->used;
    arena->used += size;
    return ptr;
}

void mem_arena_reset(MemArena *arena) {
    if (arena != NULL) {
        arena->used = 0;
    }
}

void mem_arena_free(MemArena *arena) {
    if (arena != NULL && arena->memory != NULL) {
        free(arena->memory);
        arena->memory = NULL;
        arena->size = 0;
        arena->used = 0;
    }
}
