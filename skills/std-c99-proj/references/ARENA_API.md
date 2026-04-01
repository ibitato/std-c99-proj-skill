# Memory Arena API Reference

## Overview

The memory arena provides a simple bump-pointer allocator. A single `malloc`
call backs the entire arena. Sub-allocations are O(1) pointer bumps. All
memory is freed at once — no individual `free` calls, no fragmentation, no leaks.

## Data Structure

```c
typedef struct {
    char  *memory;  /* Backing buffer */
    size_t size;    /* Total capacity in bytes */
    size_t used;    /* Bytes currently allocated */
} MemArena;
```

## Functions

### `mem_arena_init`

```c
int mem_arena_init(MemArena *arena, size_t size);
```

Allocates a backing buffer of `size` bytes.

- **Returns**: `0` on success, `-1` on failure (NULL arena, size 0, or malloc failure).
- **Postcondition**: `arena->used == 0`, `arena->size == size`.

### `mem_arena_alloc`

```c
void *mem_arena_alloc(MemArena *arena, size_t size);
```

Bump-allocates `size` bytes from the arena.

- **Returns**: pointer to the allocated block, or `NULL` if the arena is full,
  arena is NULL, or size is 0.
- **Note**: returned pointer is valid until `mem_arena_free` is called.
  `mem_arena_reset` invalidates the data but the pointer remains within bounds.

### `mem_arena_reset`

```c
void mem_arena_reset(MemArena *arena);
```

Resets `arena->used` to 0. The backing buffer is not freed — the arena can be
reused. Previous allocations are logically invalidated.

### `mem_arena_free`

```c
void mem_arena_free(MemArena *arena);
```

Frees the backing buffer and zeros all fields. Safe to call multiple times
(idempotent). After `mem_arena_free`, the arena must be re-initialized before use.

## Usage Pattern

```c
MemArena arena;
mem_arena_init(&arena, 1024 * 1024);  /* 1 MiB */

int *data = (int *)mem_arena_alloc(&arena, 100 * sizeof(int));
/* use data... */

mem_arena_reset(&arena);  /* reuse without freeing */

char *buf = (char *)mem_arena_alloc(&arena, 512);
/* use buf... */

mem_arena_free(&arena);  /* single free — zero leaks */
```

## Rules

- Application code must NOT call `malloc`/`free` directly — use the arena.
- The arena itself calls `malloc` exactly once (in `init`) and `free` exactly
  once (in `free`).
- Always check return values: `init` returns -1 on failure, `alloc` returns NULL.
