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

typedef struct ArenaTemp ArenaTemp;
struct ArenaTemp
{
 Arena *arena;
 U64 pos;
};

////////////////////////////////
//~ rjf: Arena Functions

core_function Arena *ArenaAlloc(U64 size);
core_function Arena *ArenaAllocDefault(void);
core_function void ArenaRelease(Arena *arena);
core_function void *ArenaPushNoZero(Arena *arena, U64 size);
core_function void *ArenaPushAligner(Arena *arena, U64 alignment);
core_function void *ArenaPush(Arena *arena, U64 size);
core_function void ArenaPopTo(Arena *arena, U64 pos);
core_function void ArenaSetAutoAlign(Arena *arena, U64 align);
core_function void ArenaPop(Arena *arena, U64 size);
core_function void ArenaClear(Arena *arena);
core_function U64 ArenaPos(Arena *arena);

#define PushArrayNoZero(arena, type, count) (type *)ArenaPushNoZero((arena), sizeof(type)*(count))
#define PushArray(arena, type, count)       (type *)ArenaPush((arena), sizeof(type)*(count))

////////////////////////////////
//~ rjf: Arena Temp Functions

core_function ArenaTemp ArenaTempBegin(Arena *arena);
core_function void ArenaTempEnd(ArenaTemp temp);

#define ArenaTempBlock(arena, name) ArenaTemp name = {0}; DeferLoop(name = ArenaTempBegin(arena), ArenaTempEnd(name))

#endif // BASE_MEMORY_H
