#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

////////////////////////////////
//~ rjf: Constants

#if !defined(ARENA_COMMIT_GRANULARITY)
#define ARENA_COMMIT_GRANULARITY Kilobytes(4)
#endif

#if !defined(ARENA_DECOMMIT_THRESHOLD)
#define ARENA_DECOMMIT_THRESHOLD Megabytes(64)
#endif

////////////////////////////////
//~ rjf: Arena Allocator Types

typedef struct Arena Arena;
struct Arena
{
 U64 pos;
 U64 commit_pos;
 U64 align;
 U64 size;
 Arena *ptr;
 U64 _unused_[3];
};

typedef struct Temp Temp;
struct Temp
{
 Arena *arena;
 U64 pos;
};

////////////////////////////////
//~ rjf: Arena Functions

root_function Arena *ArenaAlloc(U64 size);
root_function Arena *ArenaAllocDefault(void);
root_function void ArenaRelease(Arena *arena);
root_function void *ArenaPushNoZero(Arena *arena, U64 size);
root_function void *ArenaPushAligner(Arena *arena, U64 alignment);
root_function void *ArenaPush(Arena *arena, U64 size);
root_function void ArenaPopTo(Arena *arena, U64 pos);
root_function void ArenaSetAutoAlign(Arena *arena, U64 align);
root_function void ArenaPop(Arena *arena, U64 size);
root_function void ArenaClear(Arena *arena);
root_function U64 ArenaPos(Arena *arena);

#define PushArrayNoZero(arena, type, count) (type *)ArenaPushNoZero((arena), sizeof(type)*(count))
#define PushArray(arena, type, count)       (type *)ArenaPush((arena), sizeof(type)*(count))

////////////////////////////////
//~ rjf: Arena Temp Functions

root_function Temp TempBegin(Arena *arena);
root_function void TempEnd(Temp temp);

#define ArenaTempBlock(arena, name) Temp name = {0}; DeferLoop(name = TempBegin(arena), TempEnd(name))

#endif // BASE_MEMORY_H
