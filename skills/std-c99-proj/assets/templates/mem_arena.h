/*!
 * \file mem_arena.h
 * \brief Memory arena allocator — pure ANSI C99.
 *
 * Single allocation backing buffer. All sub-allocations are bump-pointer.
 * Free everything at once with mem_arena_free().
 */

#ifndef MEM_ARENA_H
#define MEM_ARENA_H

#include <stddef.h>

typedef struct {
    char  *memory;  /**< Backing buffer. */
    size_t size;    /**< Total capacity in bytes. */
    size_t used;    /**< Bytes currently allocated. */
} MemArena;

/**
 * \brief Initialize an arena with the given capacity.
 * \param arena Pointer to arena struct.
 * \param size  Capacity in bytes (must be > 0).
 * \return 0 on success, -1 on failure.
 */
int mem_arena_init(MemArena *arena, size_t size);

/**
 * \brief Allocate \p size bytes from the arena.
 * \return Pointer to allocated block, or NULL if out of space.
 */
void *mem_arena_alloc(MemArena *arena, size_t size);

/**
 * \brief Reset the arena — marks all space as free without releasing memory.
 */
void mem_arena_reset(MemArena *arena);

/**
 * \brief Free the backing buffer and zero the struct.
 */
void mem_arena_free(MemArena *arena);

#endif /* MEM_ARENA_H */
