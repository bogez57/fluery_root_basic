/* date = November 8th 2022 4:56 am */

#ifndef CONTENT_CORE_H
#define CONTENT_CORE_H

////////////////////////////////
//~ rjf: Basic Types

typedef struct C_InitReceipt C_InitReceipt;
struct C_InitReceipt
{
 U64 u64[1];
};

typedef struct C_Scope C_Scope;
struct C_Scope
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Tag -> Data Cache Types

typedef struct C_Tag2DataCacheNode C_Tag2DataCacheNode;
struct C_Tag2DataCacheNode
{
 C_Tag2DataCacheNode *hash_next;
 C_Tag2DataCacheNode *hash_prev;
 Arena *arena;
 U128 hash;
 String8 data;
 U64 last_accessed_time_us;
};

typedef struct C_Tag2DataCacheSlot C_Tag2DataCacheSlot;
struct C_Tag2DataCacheSlot
{
 C_Tag2DataCacheNode *first;
 C_Tag2DataCacheNode *last;
};

////////////////////////////////
//~ rjf: Main State

typedef struct C_State C_State;
struct C_State
{
 Arena *arena;
 U64 tag2data_table_size;
 C_Tag2DataCacheSlot *tag2data_table;
 OS_StripeTable *tag2data_stripes;
};

////////////////////////////////
//~ rjf: Globals

root_global C_State *c_state;

////////////////////////////////
//~ rjf: Top-Level API

root_function C_InitReceipt C_Init(OS_InitReceipt os_init_receipt);

////////////////////////////////
//~ rjf: Scopes

root_function C_Scope *C_ScopeOpen(void);
root_function void C_ScopeClose(C_Scope *scope);

////////////////////////////////
//~ rjf: Hashes

root_function U128 C_HashFromString(String8 string);

////////////////////////////////
//~ rjf: Cache Interaction

root_function U128 C_SubmitData(Arena **permanent_arena, String8 data);
root_function void C_SubmitStaticData(String8 data, U128 hash);
root_function String8 C_DataFromHash(C_Scope *scope, U128 hash);

#endif // CONTENT_CORE_H
