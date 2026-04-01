/*!
 * \file mem_arena.c
 * \brief Memory arena allocator implementation — pure ANSI C99, hardened.
 *
 * Hardening:
 *   - All allocations aligned to MEM_ARENA_ALIGN (natural alignment)
 *   - Integer overflow check on used + size
 *   - Double-init guard (detects leak)
 */

#include "mem_arena.h"
#include <stdlib.h>
#include <stdio.h>

/** Round \p n up to the next multiple of MEM_ARENA_ALIGN. */
static size_t align_up(size_t n) {
    return (n + MEM_ARENA_ALIGN - 1) & ~(MEM_ARENA_ALIGN - 1);
}

int mem_arena_init(MemArena *arena, size_t size) {
    if (arena == NULL || size == 0) {
        return -1;
    }
    /* Guard: double init without free leaks memory */
    if (arena->memory != NULL) {
        (void)fprintf(stderr, "arena: already initialized (call free first)\n");
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
    size_t aligned;
    if (arena == NULL || arena->memory == NULL || size == 0) {
        return NULL;
    }
    aligned = align_up(size);
    /* Overflow check: aligned < size means wrap-around */
    if (aligned < size || arena->used > arena->size - aligned) {
        (void)fprintf(stderr, "arena: out of space (need %zu, have %zu)\n",
                       size, arena->size - arena->used);
        return NULL;
    }
    {
        void *ptr = arena->memory + arena->used;
        arena->used += aligned;
        return ptr;
    }
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
