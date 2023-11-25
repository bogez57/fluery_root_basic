/* date = December 3rd 2022 0:37 pm */

#ifndef FILE_STREAM_H
#define FILE_STREAM_H

////////////////////////////////
//~ rjf: Basic Types

typedef struct FS_InitReceipt FS_InitReceipt;
struct FS_InitReceipt
{
 U64 u64[1];
};

typedef struct FS_Tag FS_Tag;
struct FS_Tag
{
 U64 u64[2];
};

////////////////////////////////
//~ rjf: State Types

typedef struct FS_Node FS_Node;
struct FS_Node
{
 FS_Node *hash_next;
 FS_Tag tag;
 String8 path;
 OS_Timestamp last_modified;
 C_Hash hash;
 B32 hot_version_is_stale;
 B32 data_has_been_queried;
};

typedef struct FS_Slot FS_Slot;
struct FS_Slot
{
 FS_Node *first;
 FS_Node *last;
};

typedef struct FS_State FS_State;
struct FS_State
{
 Arena *arena;
 
 //- rjf: request ring buffer
 U8 *req_ring_base;
 U64 req_ring_size;
 U64 req_ring_write_pos;
 U64 req_ring_read_pos;
 OS_Handle req_ring_mutex;
 OS_Handle req_ring_cv;
 
 //- rjf: shared (tag) -> (info) data structure
 U64 tag_table_count;
 FS_Slot *tag_table_slots;
 OS_StripeTable *tag_table_stripes;
 
 //- rjf: threads
 U64 loader_thread_count;
 OS_Handle *loader_threads;
 S64 volatile loader_thread_working_count;
 S64 volatile loader_thread_request_count;
 OS_Handle change_detector_thread;
};

////////////////////////////////
//~ rjf: Globals

root_global FS_State *fs_state;

////////////////////////////////
//~ rjf: Top-Level API

root_function FS_InitReceipt FS_Init(OS_InitReceipt os_init_receipt, C_InitReceipt c_init_receipt);

////////////////////////////////
//~ rjf: Tag Functions

root_function FS_Tag FS_TagZero(void);
root_function FS_Tag FS_TagFromPath(String8 path);
root_function String8 FS_PathFromTag(Arena *arena, FS_Tag tag);
root_function B32 FS_TagMatch(FS_Tag a, FS_Tag b);
root_function C_Hash FS_ContentHashFromTag(FS_Tag tag, U64 endt_microseconds);

////////////////////////////////
//~ rjf: Request Ring Buffer Encoding

root_function void FS_EnqueueLoadRequest(FS_Tag tag);
root_function FS_Tag FS_DequeueLoadRequest(void);

////////////////////////////////
//~ rjf: Active Work Info

root_function S64 FS_LoaderThreadWorkingCount(void);
root_function S64 FS_LoaderThreadRequestCount(void);

////////////////////////////////
//~ rjf: Worker Thread Implementations

root_function void FS_LoaderThreadEntryPoint(void *p);
root_function void FS_ChangeDetectorThreadEntryPoint(void *p);

#endif // FILE_STREAM_H
