# Arena Allocator for AST Nodes

## Overview

This is a simple arena (or region-based) memory allocator implemented in C. It is designed for allocating Abstract Syntax Tree (AST) nodes efficiently without worrying about freeing each node individually. Instead, all memory allocated can be freed at once when the arena is no longer needed.

---

## Why Arena Allocator?

- Reduces malloc/free overhead
- Simple and fast memory management
- Useful for ASTs where allocations have similar lifetime

---

## How it works

1. Memory is allocated in large blocks (arena blocks).
2. Small allocations are fulfilled from the current block.
3. If the current block can't fit the requested size, a new block is allocated.
4. All blocks are linked together as a singly linked list.
5. Freeing the arena frees all allocated memory.

---

## Code Explanation

### `Arena` Structure

```c
typedef struct Arena {
    char *block;      // Pointer to actual memory block
    size_t offset;    // Current allocation offset
    size_t size;      // Total block size
    struct Arena *next; // Linked list pointer to next arena
} Arena;
```

### Global Arena Pointer

```c
Arena *arenaHead = NULL;
```

### Arena Allocator

```c
void *arenaAlloc(size_t size);
```
- Allocates memory from the arena.
- Creates a new block if needed.

### AST Node Allocation

```c
ASTNode *allocateNode(NodeType type);
```
- Allocates and initializes an AST node using `arenaAlloc()`.

### Free Arena

```c
void freeArena();
```
- Frees all blocks at once.

---

## Arena Allocator Diagram

### Initial State

```
arenaHead = NULL
```

### After first allocation (size = 100 bytes)

```
+------------------+   +------------------------------------+
|   Arena Struct   |-->|       block[ARENA_BLOCK_SIZE]      |
|  offset = 100    |   |   (100 bytes used, rest free)      |
|  size = 4096     |   +------------------------------------+
|  next  = NULL    |
+------------------+
```

### After second allocation (size = 200 bytes)

```
+------------------+   +------------------------------------+
|   Arena Struct   |-->|       block[ARENA_BLOCK_SIZE]      |
|  offset = 300    |   |   (100 + 200 = 300 bytes used)     |
|  size = 4096     |   +------------------------------------+
|  next  = NULL    |
+------------------+
```

### If a big allocation comes (size = 5000 bytes > ARENA_BLOCK_SIZE)

```
+------------------+   +------------------------------------+
|   Arena Struct   |-->|      block[5000 bytes]             |  <- New Arena
|  offset = 5000   |   +------------------------------------+
|  size = 5000     |   |
|  next  ----------|---+
|                  |
v
+------------------+   +------------------------------------+
|   Arena Struct   |-->|       block[ARENA_BLOCK_SIZE]      |  <- Previous Arena
|  offset = 300    |   +------------------------------------+
|  size = 4096     |
|  next  = NULL    |
+------------------+
```

### Arena Linked List View

```
[5000 bytes block] --> [4096 bytes block] --> NULL
```

---

### Allocation Flow

```
allocateNode() --> arenaAlloc(size)
                     ↓
         checks if current arena has space
         ↓               ↓
     YES: alloc inside   NO: alloc new Arena block 
         ↓                       ↓
      return ptr            link new arena to list
                                 ↓
                           return ptr
```

---

## Notes

- `ARENA_BLOCK_SIZE` is a constant (recommended ~4096 bytes).
- This implementation assumes AST nodes are short-lived and will be freed together.
- If you need individual free, arena is not suitable.

---

## Author

@shivang-k (GitHub)

