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
 OS_AccessFlag_All       = 0xFFFFFFFF,
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

////////////////////////////////
//~ rjf: System Path Types

typedef enum OS_SystemPath
{
 OS_SystemPath_Null,
 OS_SystemPath_Initial,
 OS_SystemPath_Current,
 OS_SystemPath_Binary,
 OS_SystemPath_AppData,
 OS_SystemPath_COUNT,
}
OS_SystemPath;

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

//- rjf: path normalizations
root_function String8 OS_NormalizedPathFromStr8(Arena *arena, String8 string);

//- rjf: file system interaction bundlers
root_function String8 OS_LoadFile(Arena *arena, String8 path, OS_ErrorList *out_errors);
root_function void OS_SaveFile(Arena *arena, String8 path, String8List data, OS_ErrorList *out_errors);
root_function B32 OS_FileExists(String8 path);

//- rjf: stripe table
root_function OS_StripeTable *OS_StripeTableAlloc(Arena *arena, U64 count);
root_function void OS_StripeTableRelease(OS_StripeTable *table);

//- rjf: timestamp <-> date time
root_function DateTime OS_DateTimeFromTimestamp(OS_Timestamp timestamp);
root_function OS_Timestamp OS_TimestampFromDateTime(DateTime date_time);

////////////////////////////////
//~ rjf: @os_per_backend General Program API

root_function OS_InitReceipt OS_Init(void);
root_function void           OS_Abort(void);
root_function String8        OS_GetSystemPath(Arena *arena, OS_SystemPath path);

////////////////////////////////
//~ rjf: @os_per_backend Memory

root_function U64   OS_PageSize(void);
root_function void *OS_Reserve(U64 size);
root_function void  OS_Release(void *ptr, U64 size);
root_function void  OS_Commit(void *ptr, U64 size);
root_function void  OS_Decommit(void *ptr, U64 size);
root_function void  OS_SetMemoryAccessFlags(void *ptr, U64 size, OS_AccessFlags flags);

////////////////////////////////
//~ rjf: @os_per_backend Libraries

root_function OS_Handle     OS_LibraryOpen(String8 path);
root_function void          OS_LibraryClose(OS_Handle handle);
root_function VoidFunction *OS_LibraryLoadFunction(OS_Handle handle, String8 name);

////////////////////////////////
//~ rjf: @os_per_backend File System

//- rjf: granular handle operations
root_function OS_Handle OS_FileOpen(Arena *arena, OS_AccessFlags access_flags, String8 path, OS_ErrorList *out_errors);
root_function void OS_FileClose(OS_Handle file);
root_function String8 OS_FileRead(Arena *arena, OS_Handle file, Rng1U64 range, OS_ErrorList *out_errors);
root_function void OS_FileWrite(Arena *arena, OS_Handle file, U64 off, String8List data, OS_ErrorList *out_errors);
root_function B32 OS_FileIsValid(OS_Handle file);
root_function OS_FileAttributes OS_AttributesFromFile(OS_Handle file);

//- rjf: whole-file operations
root_function void OS_DeleteFile(String8 path);
root_function void OS_MoveFile(String8 dst_path, String8 src_path);
root_function B32  OS_CopyFile(String8 dst_path, String8 src_path);
root_function B32  OS_MakeDirectory(String8 path);
root_function B32  OS_DeleteDirectory(String8 path);

//- rjf: file system introspection
root_function OS_FileIter *     OS_FileIterBegin(Arena *arena, String8 path);
root_function B32               OS_FileIterNext(Arena *arena, OS_FileIter *it, OS_FileInfo *out_info);
root_function void              OS_FileIterEnd(OS_FileIter *it);
root_function OS_FileAttributes OS_FileAttributesFromPath(String8 path);

////////////////////////////////
//~ rjf: @os_per_backend Time

root_function DateTime OS_DateTimeCurrent(void);
root_function U64 OS_TimeMicroseconds(void);
root_function U64 OS_TimeCycles(void);
root_function void OS_Sleep(U64 milliseconds);
root_function void OS_Wait(U64 end_time_microseconds);

////////////////////////////////
//~ rjf: @os_per_backend Threads & Synchronization Primitives

//- rjf: thread controls
root_function U64 OS_TID(void);
root_function OS_Handle OS_ThreadStart(void *params, OS_ThreadFunction *func);
root_function void OS_ThreadJoin(OS_Handle thread);
root_function void OS_ThreadDetach(OS_Handle thread);

//- rjf: mutexes
root_function OS_Handle OS_MutexAlloc(void);
root_function void OS_MutexRelease(OS_Handle mutex);
root_function void OS_MutexBlockEnter(OS_Handle mutex);
root_function void OS_MutexBlockLeave(OS_Handle mutex);
#define OS_MutexBlock(m) DeferLoop(OS_MutexBlockEnter(m), OS_MutexBlockLeave(m))

//- rjf: slim reader/writer mutexes
root_function OS_Handle OS_SRWMutexAlloc(void);
root_function void OS_SRWMutexRelease(OS_Handle mutex);
root_function void OS_SRWMutexWriterBlockEnter(OS_Handle mutex);
root_function void OS_SRWMutexWriterBlockLeave(OS_Handle mutex);
root_function void OS_SRWMutexReaderBlockEnter(OS_Handle mutex);
root_function void OS_SRWMutexReaderBlockLeave(OS_Handle mutex);
#define OS_SRWMutexWriterBlock(m) DeferLoop(OS_SRWMutexWriterBlockEnter(m), OS_SRWMutexWriterBlockLeave(m))
#define OS_SRWMutexReaderBlock(m) DeferLoop(OS_SRWMutexReaderBlockEnter(m), OS_SRWMutexReaderBlockLeave(m))

//- rjf: semaphores
root_function OS_Handle OS_SemaphoreAlloc(U32 initial_count, U32 max_count);
root_function void OS_SemaphoreRelease(OS_Handle handle);
root_function B32 OS_SemaphoreWait(OS_Handle handle, U32 max_milliseconds);
root_function U64 OS_SemaphoreSignal(OS_Handle handle);

//- rjf: condition variables
root_function OS_Handle OS_ConditionVariableAlloc(void);
root_function void OS_ConditionVariableRelease(OS_Handle cv);
root_function B32 OS_ConditionVariableWait(OS_Handle cv, OS_Handle mutex, U64 end_time_microseconds);
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

////////////////////////////////
//~ rjf: @os_per_backend System Info

root_function F32 OS_CaretBlinkTime(void);
root_function F32 OS_DoubleClickTime(void);
root_function U64 OS_LogicalProcessorCount(void);

#endif // OS_CORE_H
