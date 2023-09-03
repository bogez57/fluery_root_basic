/* date = August 2nd 2023 6:24 am */

#ifndef HMB_DEMO_H
#define HMB_DEMO_H

#define NAME_CHUNK_PAYLOAD_SIZE 56

typedef struct Handle Handle;
struct Handle
{
 U64 u64[2];
};

typedef struct NameChunk NameChunk;
struct NameChunk
{
 NameChunk *next;
};

typedef struct NameChunkList NameChunkList;
struct NameChunkList
{
 NameChunk *first;
 NameChunk *last;
 U64 count;
 U64 total_size;
};

typedef struct Entity Entity;
struct Entity
{
 Entity *next;
 B32 is_active;
 U64 gen;
 NameChunkList name_chunks;
 Vec2F32 pos;
 Vec2F32 vel;
 F32 radius;
 Vec4F32 color;
};

typedef struct State State;
struct State
{
 Arena *arena;
 Arena *name_chunk_arena;
 NameChunk *first_free_name_chunk;
 Arena *entities_arena;
 Entity *entities_base;
 U64 entities_count;
 Entity *first_free_entity;
 Handle dragging_entity;
 Handle edit_entity;
 TxtPt cursor;
 TxtPt mark;
 U8 name_edit_buffer[4096];
 U64 name_edit_size;
};

////////////////////////////////
//~ rjf: Demo Functions

function Handle HandleZero(void);

function Entity *EntityAlloc(State *state);
function void EntityRelease(State *state, Entity *entity);
function void EntityEquipName(State *state, Entity *entity, String8 string);
function String8 NameFromEntity(Arena *arena, Entity *entity);

function Handle HandleFromEntity(Entity *entity);
function Entity *EntityFromHandle(Handle handle);

function NameChunkList NameChunkListAlloc(State *state, String8 string);
function NameChunkList NameChunkListAllocF(State *state, char *fmt, ...);
function void NameChunkListRelease(State *state, NameChunkList *list);

typedef struct ArenaFreeListDrawData ArenaFreeListDrawData;
struct ArenaFreeListDrawData
{
 Arena *arena;
 void *free_list_top;
};
function UI_CUSTOM_DRAW_FUNCTION(ArenaFreeListDraw);

#endif //HMB_DEMO_H
