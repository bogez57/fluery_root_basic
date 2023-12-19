#ifndef OS_CORE_H
#define OS_CORE_H

////////////////////////////////
//~ rjf: Basics

typedef struct OS_InitReceipt OS_InitReceipt;
struct OS_InitReceipt
{
 U64 u64[1];
};

typedef struct OS_Handle OS_Handle;
struct OS_Handle
{
 U64 u64[1];
};

typedef U32 OS_AccessFlags;
enum
{
 OS_AccessFlag_Read      = (1<<0),
 OS_AccessFlag_Write     = (1<<1),
 OS_AccessFlag_Execute   = (1<<2),
 OS_AccessFlag_CreateNew = (1<<3),
 OS_AccessFlag_Shared    = (1<<4),
};

////////////////////////////////
//~ rjf: Errors

typedef enum OS_ErrorCode
{
 OS_ErrorCode_Null,
 OS_ErrorCode_COUNT
}
OS_ErrorCode;

typedef struct OS_Error OS_Error;
struct OS_Error
{
 OS_Error *next;
 OS_ErrorCode code;
};

typedef struct OS_ErrorList OS_ErrorList;
struct OS_ErrorList
{
 OS_Error *first;
 OS_Error *last;
 U64 count;
};

////////////////////////////////
//~ rjf: Timestamps

typedef U64 OS_Timestamp;

////////////////////////////////
//~ rjf: Filesystem Types

typedef struct OS_FileIter OS_FileIter;
struct OS_FileIter
{
 U8 opaque[1024];
};

typedef U32 OS_FileFlags;
enum
{
 OS_FileFlag_Directory = (1<<0),
};

typedef struct OS_FileAttributes OS_FileAttributes;
struct OS_FileAttributes
{
 OS_FileFlags flags;
 U64 size;
 OS_Timestamp last_modified;
};

typedef struct OS_FileInfo OS_FileInfo;
struct OS_FileInfo
{
 String8 name;
 OS_FileAttributes attributes;
};

typedef struct OS_FileInfoNode OS_FileInfoNode;
struct OS_FileInfoNode
{
 OS_FileInfoNode *next;
 OS_FileInfo v;
};

typedef struct OS_FileInfoList OS_FileInfoList;
struct OS_FileInfoList
{
 OS_FileInfoNode *first;
 OS_FileInfoNode *last;
 U64 count;
};

////////////////////////////////
//~ rjf: System Path Types

typedef enum OS_SystemPathKind
{
 OS_SystemPathKind_Null,
 OS_SystemPathKind_Initial,
 OS_SystemPathKind_Current,
 OS_SystemPathKind_Binary,
 OS_SystemPathKind_AppData,
 OS_SystemPathKind_COUNT,
}
OS_SystemPathKind;

////////////////////////////////
//~ rjf: Thread & Process Types

typedef void OS_ThreadFunction(void *params);

typedef struct OS_ProcessStatus OS_ProcessStatus;
struct OS_ProcessStatus
{
 B8 launch_failed;
 B8 running;
 B8 read_failed;
 B8 kill_failed;
 B8 was_killed;
 U32 exit_code;
};

typedef struct OS_Stripe OS_Stripe;
struct OS_Stripe
{
 OS_Handle cv;
 OS_Handle mutex;
 Arena *arena;
};

typedef struct OS_StripeTable OS_StripeTable;
struct OS_StripeTable
{
 U64 count;
 OS_Stripe *stripes;
};

////////////////////////////////
//~ rjf: Helpers

//- rjf: handle type functions
root_function OS_Handle OS_HandleZero(void);
root_function B32 OS_HandleMatch(OS_Handle a, OS_Handle b);

//- rjf: file info list functions
root_function void OS_FileInfoListPush(Arena *arena, OS_FileInfoList *list, OS_FileInfo *v);

//- rjf: path normalizations
root_function String8 OS_NormalizedPathFromStr8(Arena *arena, String8 string);

//- rjf: file system interaction bundlers
root_function String8 OS_DataFromFilePath(Arena *arena, String8 path);
root_function void OS_WriteDataToFilePath(String8 path, String8List data);
root_function B32 OS_FileExistsAtPath(String8 path);
root_function OS_FileInfoList OS_FileInfoListFromPath(Arena *arena, String8 path);

//- rjf: stripe table
root_function OS_StripeTable *OS_StripeTableAlloc(Arena *arena, U64 count);
root_function void OS_StripeTableRelease(OS_StripeTable *table);

//- rjf: timestamp <-> date time
root_function DateTime OS_DateTimeFromTimestamp(OS_Timestamp timestamp);
root_function OS_Timestamp OS_TimestampFromDateTime(DateTime date_time);

////////////////////////////////
//~ rjf: @os_per_backend Main Layer Initialization

root_function OS_InitReceipt OS_Init(void);

////////////////////////////////
//~ rjf: @os_per_backend Aborting

root_function void OS_Abort(void);

////////////////////////////////
//~ rjf: @os_per_backend System Info

root_function String8 OS_StringFromSystemPathKind(Arena *arena, OS_SystemPathKind path);
root_function U64 OS_PageSize(void);
root_function F32 OS_CaretBlinkTime(void);
root_function F32 OS_DoubleClickTime(void);
root_function U64 OS_LogicalProcessorCount(void);

////////////////////////////////
//~ rjf: @os_per_backend Memory

root_function void *OS_Reserve(U64 size);
root_function void  OS_Release(void *ptr, U64 size);
root_function void  OS_Commit(void *ptr, U64 size);
root_function void  OS_Decommit(void *ptr, U64 size);
root_function void  OS_Protect(void *ptr, U64 size, OS_AccessFlags flags);

////////////////////////////////
//~ rjf: @os_per_backend Libraries

root_function OS_Handle     OS_LibraryOpen(String8 path);
root_function void          OS_LibraryClose(OS_Handle handle);
root_function VoidFunction *OS_LibraryLoadFunction(OS_Handle handle, String8 name);

////////////////////////////////
//~ rjf: @os_per_backend File System

//- rjf: handle-based operations
root_function OS_Handle         OS_FileOpen(OS_AccessFlags access_flags, String8 path);
root_function void              OS_FileClose(OS_Handle file);
root_function String8           OS_FileRead(Arena *arena, OS_Handle file, Rng1U64 range);
root_function void              OS_FileWrite(OS_Handle file, U64 off, String8List data);
root_function B32               OS_FileIsValid(OS_Handle file);
root_function OS_FileAttributes OS_AttributesFromFile(OS_Handle file);

//- rjf: path-based operations
root_function void OS_DeleteFile(String8 path);
root_function void OS_MoveFile(String8 dst_path, String8 src_path);
root_function B32  OS_CopyFile(String8 dst_path, String8 src_path);
root_function B32  OS_MakeDirectory(String8 path);

//- rjf: file system introspection
root_function OS_FileIter *     OS_FileIterBegin(Arena *arena, String8 path);
root_function B32               OS_FileIterNext(Arena *arena, OS_FileIter *it, OS_FileInfo *out_info);
root_function void              OS_FileIterEnd(OS_FileIter *it);
root_function OS_FileAttributes OS_AttributesFromFilePath(String8 path);

////////////////////////////////
//~ rjf: @os_per_backend Time

root_function DateTime OS_DateTimeCurrent(void);
root_function U64 OS_TimeMicroseconds(void);
root_function void OS_Sleep(U64 milliseconds);
root_function void OS_Wait(U64 end_time_microseconds);

////////////////////////////////
//~ rjf: @os_per_backend Threads & Synchronization Primitives

//- rjf: thread controls
root_function U64 OS_TID(void);
root_function void OS_SetThreadName(String8 name);
root_function OS_Handle OS_ThreadStart(void *params, OS_ThreadFunction *func);
root_function void OS_ThreadJoin(OS_Handle thread);
root_function void OS_ThreadDetach(OS_Handle thread);

//- rjf: mutexes
root_function OS_Handle OS_MutexAlloc(void);
root_function void OS_MutexRelease(OS_Handle mutex);
root_function void OS_MutexScopeEnter(OS_Handle mutex);
root_function void OS_MutexScopeLeave(OS_Handle mutex);
#define OS_MutexScope(m) DeferLoop(OS_MutexScopeEnter(m), OS_MutexScopeLeave(m))

//- rjf: slim reader/writer mutexes
root_function OS_Handle OS_SRWMutexAlloc(void);
root_function void OS_SRWMutexRelease(OS_Handle mutex);
root_function void OS_SRWMutexScopeEnter_W(OS_Handle mutex);
root_function void OS_SRWMutexScopeLeave_W(OS_Handle mutex);
root_function void OS_SRWMutexScopeEnter_R(OS_Handle mutex);
root_function void OS_SRWMutexScopeLeave_R(OS_Handle mutex);
#define OS_SRWMutexScope_W(m) DeferLoop(OS_SRWMutexScopeEnter_W(m), OS_SRWMutexScopeLeave_W(m))
#define OS_SRWMutexScope_R(m) DeferLoop(OS_SRWMutexScopeEnter_R(m), OS_SRWMutexScopeLeave_R(m))

//- rjf: semaphores
root_function OS_Handle OS_SemaphoreAlloc(U32 initial_count, U32 max_count);
root_function void OS_SemaphoreRelease(OS_Handle handle);
root_function B32 OS_SemaphoreWait(OS_Handle handle, U32 max_milliseconds);
root_function U64 OS_SemaphoreSignal(OS_Handle handle);

//- rjf: condition variables
root_function OS_Handle OS_ConditionVariableAlloc(void);
root_function void OS_ConditionVariableRelease(OS_Handle cv);
root_function B32 OS_ConditionVariableWait(OS_Handle cv, OS_Handle mutex, U64 endt_us);
root_function B32 OS_ConditionVariableWaitSRW_W(OS_Handle cv, OS_Handle mutex, U64 endt_us);
root_function B32 OS_ConditionVariableWaitSRW_R(OS_Handle cv, OS_Handle mutex, U64 endt_us);
root_function void OS_ConditionVariableSignal(OS_Handle cv);
root_function void OS_ConditionVariableSignalAll(OS_Handle cv);

////////////////////////////////
//~ rjf: @os_per_backend Child Processes

root_function OS_Handle        OS_ProcessLaunch(String8 command, String8 working_directory);
root_function void             OS_ProcessRelease(OS_Handle handle);
root_function String8          OS_ProcessReadOutput(Arena *arena, OS_Handle process);
root_function void             OS_ProcessKill(OS_Handle process);
root_function U64              OS_PIDFromProcess(OS_Handle process);
root_function OS_ProcessStatus OS_StatusFromProcess(OS_Handle process);

////////////////////////////////
//~ rjf: @os_per_backend Miscellaneous

root_function void OS_GetEntropy(void *data, U64 size);

#endif // OS_CORE_H
