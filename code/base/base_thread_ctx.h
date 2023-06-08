#ifndef BASE_THREAD_CTX_H
#define BASE_THREAD_CTX_H

typedef struct ThreadCtx ThreadCtx;
struct ThreadCtx
{
 Arena *arenas[2];
 char *file_name;
 U64 line_number;
 U8 thread_name[32];
 U64 thread_name_size;
 B32 is_main_thread;
};

//- rjf: thread context construction & set/get
core_function ThreadCtx ThreadCtxAlloc(void);
core_function void ThreadCtxRelease(ThreadCtx *tctx);
no_name_mangle void SetThreadCtx(ThreadCtx *tctx);
no_name_mangle ThreadCtx *GetThreadCtx(void);

//- rjf: thread metadata
core_function void SetThreadName(String8 string);
core_function String8 GetThreadName(void);
core_function B32 IsMainThread(void);

//- rjf: source code location set helper
core_function void SetThreadFileAndLine_(char *file, int line);
#define RecordCodeLoc() SetThreadFileAndLine_(__FILE__, __LINE__)

//- rjf: scratch memory
core_function ArenaTemp GetScratch(Arena **conflicts, U64 conflict_count);
#define ReleaseScratch(temp) ArenaTempEnd(temp)

//- rjf: main thread entry point
core_function void BaseMainThreadEntry(void (*entry)(struct CmdLine *cmdln), U64 argument_count, char **arguments);

#endif // BASE_THREAD_CTX_H
