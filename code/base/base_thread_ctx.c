//- rjf: thread context construction & set/get

root_function ThreadCtx
ThreadCtxAlloc(void)
{
 ThreadCtx tctx = {0};
 for(U64 arena_idx = 0; arena_idx < ArrayCount(tctx.arenas); arena_idx += 1)
 {
  tctx.arenas[arena_idx] = ArenaAlloc(Gigabytes(8));
 }
 return tctx;
}

root_function void
ThreadCtxRelease(ThreadCtx *tctx)
{
 for(U64 arena_idx = 0; arena_idx < ArrayCount(tctx->arenas); arena_idx += 1)
 {
  ArenaRelease(tctx->arenas[arena_idx]);
 }
}

#if BUILD_CORE
per_thread ThreadCtx *tl_tctx = 0;

no_name_mangle void
SetThreadCtx(ThreadCtx *tctx)
{
 tl_tctx = tctx;
}

no_name_mangle ThreadCtx *
GetThreadCtx(void)
{
 return tl_tctx;
}
#endif

//- rjf: thread name

root_function void
SetThreadName(String8 string)
{
 ThreadCtx *tctx = GetThreadCtx();
 tctx->thread_name_size = Min(string.size, sizeof(tctx->thread_name));
 MemoryCopy(tctx->thread_name, string.str, tctx->thread_name_size);
}

root_function String8
GetThreadName(void)
{
 ThreadCtx *tctx = GetThreadCtx();
 String8 result = Str8(tctx->thread_name, tctx->thread_name_size);
 return result;
}

root_function B32
IsMainThread(void)
{
 ThreadCtx *tctx = GetThreadCtx();
 return tctx->is_main_thread;
}

//- rjf: source code location set helper

root_function void
SetThreadFileAndLine_(char *file, int line)
{
 ThreadCtx *tctx = GetThreadCtx();
 tctx->file_name = file;
 tctx->line_number = line;
}

//- rjf: scratch memory

root_function Temp
ScratchBegin(Arena **conflicts, U64 conflict_count)
{
 Temp scratch = {0};
 ThreadCtx *tctx = GetThreadCtx();
 for(U64 tctx_idx = 0; tctx_idx < ArrayCount(tctx->arenas); tctx_idx += 1)
 {
  B32 is_conflicting = 0;
  for(Arena **conflict = conflicts; conflict < conflicts+conflict_count; conflict += 1)
  {
   if(*conflict == tctx->arenas[tctx_idx])
   {
    is_conflicting = 1;
    break;
   }
  }
  if(is_conflicting == 0)
  {
   scratch.arena = tctx->arenas[tctx_idx];
   scratch.pos = scratch.arena->pos;
   break;
  }
 }
 return scratch;
}
