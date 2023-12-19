/* date = October 25th 2022 9:09 pm */

#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

////////////////////////////////
//~ rjf: Basic Types

typedef struct T_InitReceipt T_InitReceipt;
struct T_InitReceipt
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Main State

typedef U32 T_NodeFlags;
enum
{
 T_NodeFlag_Hot = (1<<0),
};

typedef enum T_NodeAllocKind
{
 T_NodeAllocKind_Null,
 T_NodeAllocKind_Atlas,
 T_NodeAllocKind_Standalone,
 T_NodeAllocKind_COUNT
}
T_NodeAllocKind;

typedef struct T_Node T_Node;
struct T_Node
{
 T_Node *hash_next;
 T_Node *hash_prev;
 U128 hash;
 Vec2F32 dim;
 T_NodeFlags flags;
 T_NodeAllocKind alloc_kind;
 union
 {
  struct
  {
   Rng2S64 alloc_region;
   S32 atlas_num;
  } alloc_atlas;
  struct
  {
   R_Handle texture;
  } alloc_standalone;
 };
};

typedef struct T_Slot T_Slot;
struct T_Slot
{
 T_Node *first;
 T_Node *last;
 U64 count;
};

typedef struct T_Atlas T_Atlas;
struct T_Atlas
{
 T_Atlas *next;
 T_Atlas *prev;
 OS_Handle mutex;
 Arena *arena;
 Atlas *atlas;
 Vec2S64 atlas_dim;
 R_Handle texture;
};

typedef struct T_State T_State;
struct T_State
{
 Arena *arena;
 R_Handle invalid_texture;
 
 //- rjf: tag upload request queue
 U8 *req_ring_base;
 U64 req_ring_size;
 U64 req_ring_read_pos;
 U64 req_ring_write_pos;
 OS_Handle req_ring_cv;
 OS_Handle req_ring_mutex;
 
 //- rjf: shared data structure for (tag) -> (tex)
 U64 tex_table_size;
 U64 tex_table_stripe_count;
 T_Slot *tex_table;
 OS_Handle *tex_table_stripe_mutexes;
 OS_Handle *tex_table_stripe_cvs;
 Arena **tex_table_stripe_arenas;
 
 //- rjf: shared data structure for atlases
 OS_Handle atlas_list_mutex;
 T_Atlas *first_atlas;
 T_Atlas *last_atlas;
 
 //- rjf: threads
 U64 thread_count;
 OS_Handle *threads;
};

////////////////////////////////
//~ rjf: Globals

root_global T_State *t_state;

////////////////////////////////
//~ rjf: Main API

root_function T_InitReceipt T_Init(C_InitReceipt c_init_receipt, R_InitReceipt r_init_receipt);

////////////////////////////////
//~ rjf: Accessors

root_function R_Handle T_InvalidTexture(void);

////////////////////////////////
//~ rjf: Tag -> Texture Region Mapping

root_function R_Slice2F32 T_Slice2F32FromHash(U128 hash, U64 endt_microseconds);

////////////////////////////////
//~ rjf: GPU Upload Request Ring Buffer

root_function B32 T_EnqueueLoadRequest(U128 hash);
root_function U128 T_DequeueLoadRequest(void);

////////////////////////////////
//~ rjf: GPU Data Upload Thread Implementation

root_function void T_ThreadEntryPoint(void *p);

#endif // TEXTURE_CACHE_H
