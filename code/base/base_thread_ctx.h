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
root_function ThreadCtx ThreadCtxAlloc(void);
root_function void ThreadCtxRelease(ThreadCtx *tctx);
no_name_mangle void SetThreadCtx(ThreadCtx *tctx);
no_name_mangle ThreadCtx *GetThreadCtx(void);

//- rjf: thread metadata
root_function void SetThreadName(String8 string);
root_function String8 GetThreadName(void);
root_function B32 IsMainThread(void);

//- rjf: source code location set helper
root_function void SetThreadFileAndLine_(char *file, int line);
#define RecordCodeLoc() SetThreadFileAndLine_(__FILE__, __LINE__)

//- rjf: scratch memory
root_function Temp ScratchBegin(Arena **conflicts, U64 conflict_count);
#define ScratchEnd(temp) TempEnd(temp)

#endif // BASE_THREAD_CTX_H
