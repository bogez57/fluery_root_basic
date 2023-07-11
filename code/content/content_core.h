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

typedef struct C_Hash C_Hash;
struct C_Hash
{
 U64 u64[2];
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
 C_Hash hash;
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

core_global B32 c_initialized;
core_global C_State *c_state;

////////////////////////////////
//~ rjf: User Thread APIs

core_function C_InitReceipt C_Init(OS_InitReceipt os_init_receipt);

core_function C_Scope *C_ScopeOpen(void);
core_function void C_ScopeClose(C_Scope *scope);

core_function C_Hash C_HashZero(void);
core_function C_Hash C_HashMake(U64 a, U64 b);
core_function C_Hash C_HashFromString(String8 string);
core_function B32 C_HashMatch(C_Hash a, C_Hash b);

core_function C_Hash C_SubmitData(Arena **permanent_arena, String8 data);
core_function void C_SubmitStaticData(String8 data, C_Hash hash);
core_function String8 C_DataFromHash(C_Scope *scope, C_Hash hash);

#endif // CONTENT_CORE_H
