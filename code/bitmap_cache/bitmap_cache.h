/* date = July 24th 2023 9:11 pm */

#ifndef BITMAP_CACHE_H
#define BITMAP_CACHE_H

////////////////////////////////
//~ rjf: Basic Types

typedef struct BC_InitReceipt BC_InitReceipt;
struct BC_InitReceipt
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Scope Type

typedef struct BC_Scope BC_Scope;
struct BC_Scope
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Bitmap Type

typedef struct BC_Bitmap BC_Bitmap;
struct BC_Bitmap
{
 U8 *pixels;
 Vec2S64 dim;
 U64 bytes_per_pixel;
};

////////////////////////////////
//~ rjf: Cache Types

typedef struct BC_Node BC_Node;
struct BC_Node
{
 BC_Node *next;
 BC_Node *prev;
 U128 hash;
 Arena *arena;
 BC_Bitmap *bitmap;
 U64 last_t_queried_us;
};

typedef struct BC_Slot BC_Slot;
struct BC_Slot
{
 BC_Node *first;
 BC_Node *last;
};

typedef struct BC_State BC_State;
struct BC_State
{
 // rjf: top-level arena
 Arena *arena;
 
 // rjf: bitmap cache table
 U64 slots_count;
 BC_Slot *slots;
 OS_StripeTable *stripes;
 
 // rjf: user -> parsing threads ring
 U64 u2p_ring_size;
 U64 u2p_ring_write_pos;
 U64 u2p_ring_read_pos;
 U8 *u2p_ring_base;
 OS_Handle u2p_ring_cv;
 OS_Handle u2p_ring_mutex;
 
 // rjf: parsing threads
 U64 p_thread_count;
 OS_Handle *p_threads;
};

////////////////////////////////
//~ rjf: Globals

root_global BC_State *bc_state;
root_global BC_Bitmap bc_nil_bitmap;

////////////////////////////////
//~ rjf: Top-Level API

root_function BC_InitReceipt BC_Init(OS_InitReceipt os_init, C_InitReceipt c_init);

////////////////////////////////
//~ rjf: Scopes

root_function BC_Scope *BC_ScopeOpen(void);
root_function void BC_ScopeClose(BC_Scope *scope);

////////////////////////////////
//~ rjf: Cache Interaction

root_function BC_Bitmap *BC_BitmapFromHash(BC_Scope *scope, U128 hash, U64 end_time_microseconds);

////////////////////////////////
//~ rjf: User -> Parsing Thread Ring Buffer

root_function void BC_EnqueueParseRequest(U128 hash);
root_function U128 BC_DequeueParseRequest(void);

////////////////////////////////
//~ rjf: Parsing Threads

root_function void BC_ParseThreadEntryPoint(void *p);

#endif // BITMAP_CACHE_H
