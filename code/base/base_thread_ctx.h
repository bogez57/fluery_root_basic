#ifndef BASE_THREAD_CTX_H
#define BASE_THREAD_CTX_H

////////////////////////////////
//~ rjf: Log Types

typedef enum LogMsgKind
{
 LogMsgKind_Null,
 LogMsgKind_Warning,
 LogMsgKind_RecoverableError,
 LogMsgKind_FatalError,
 LogMsgKind_COUNT
}
LogMsgKind;

typedef struct LogMsg LogMsg;
struct LogMsg
{
 LogMsgKind kind;
 String8 scope;
 String8 string;
};

typedef struct LogMsgChunkNode LogMsgChunkNode;
struct LogMsgChunkNode
{
 LogMsgChunkNode *next;
 LogMsg *v;
 U64 count;
 U64 cap;
};

typedef struct LogMsgChunkList LogMsgChunkList;
struct LogMsgChunkList
{
 LogMsgChunkNode *first;
 LogMsgChunkNode *last;
 U64 chunk_count;
 U64 msg_count;
};

typedef struct LogMsgArray LogMsgArray;
struct LogMsgArray
{
 LogMsg *v;
 U64 count;
};

typedef struct LogNode LogNode;
struct LogNode
{
 LogNode *first;
 LogNode *last;
 LogNode *next;
 LogNode *parent;
 String8 string;
 LogMsgChunkList msgs;
};

////////////////////////////////
//~ rjf: Thread Context Bundle Type

typedef struct ThreadCtx ThreadCtx;
struct ThreadCtx
{
 Arena *scratch_arenas[2];
 char *file_name;
 U64 line_number;
 U8 thread_name[64];
 U64 thread_name_size;
 B32 is_main_thread;
 Arena *log_arena;
 LogNode *log_root;
 LogNode *log_active_parent;
};

////////////////////////////////
//~ rjf: Thread Context Construction & Equipping

root_function ThreadCtx ThreadCtxAlloc(void);
root_function void ThreadCtxRelease(ThreadCtx *tctx);
no_name_mangle void SetThreadCtx(ThreadCtx *tctx);
no_name_mangle ThreadCtx *GetThreadCtx(void);

////////////////////////////////
//~ rjf: Thread Metadata

root_function void SetThreadName(String8 string);
root_function void SetThreadNameF(char *fmt, ...);
root_function String8 GetThreadName(void);
root_function B32 IsMainThread(void);
root_function void SetThreadFileAndLine_(char *file, int line);
#define RecordCodeLoc() SetThreadFileAndLine_(__FILE__, __LINE__)

////////////////////////////////
//~ rjf: Scratch Memory

root_function Temp ScratchBegin(Arena **conflicts, U64 conflict_count);
#define ScratchEnd(temp) TempEnd(temp)

////////////////////////////////
//~ rjf: Log Building

root_function void PushLogScope(String8 name);
root_function void PushLogScopeF(char *fmt, ...);
root_function	void PopLogScope(void);
root_function void PushLogMsg(LogMsgKind kind, String8 string);
root_function void PushLogMsgF(LogMsgKind kind, char *fmt, ...);
#define LogScope(name) DeferLoop(PushLogScope(name), PopLogScope())
#define LogScopeF(...) DeferLoop(PushLogScopeF(__VA_ARGS__), PopLogScope())

////////////////////////////////
//~ rjf: Log Consumption

root_function LogMsgArray ConsumeLog(Arena *arena, LogMsgKind min_kind);

#endif // BASE_THREAD_CTX_H
