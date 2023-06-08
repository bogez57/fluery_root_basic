//- rjf: thread context construction & set/get

core_function ThreadCtx
ThreadCtxAlloc(void)
{
 ThreadCtx tctx = {0};
 for(U64 arena_idx = 0; arena_idx < ArrayCount(tctx.arenas); arena_idx += 1)
 {
  tctx.arenas[arena_idx] = ArenaAlloc(Gigabytes(8));
 }
 return tctx;
}

core_function void
ThreadCtxRelease(ThreadCtx *tctx)
{
 for(U64 arena_idx = 0; arena_idx < ArrayCount(tctx->arenas); arena_idx += 1)
 {
  ArenaRelease(tctx->arenas[arena_idx]);
 }
}

#if BUILD_ROOT

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

core_function void
SetThreadName(String8 string)
{
 ThreadCtx *tctx = GetThreadCtx();
 tctx->thread_name_size = Min(string.size, sizeof(tctx->thread_name));
 MemoryCopy(tctx->thread_name, string.str, tctx->thread_name_size);
}

core_function String8
GetThreadName(void)
{
 ThreadCtx *tctx = GetThreadCtx();
 String8 result = Str8(tctx->thread_name, tctx->thread_name_size);
 return result;
}

core_function B32
IsMainThread(void)
{
 ThreadCtx *tctx = GetThreadCtx();
 return tctx->is_main_thread;
}

//- rjf: source code location set helper

core_function void
SetThreadFileAndLine_(char *file, int line)
{
 ThreadCtx *tctx = GetThreadCtx();
 tctx->file_name = file;
 tctx->line_number = line;
}

//- rjf: scratch memory

core_function ArenaTemp
GetScratch(Arena **conflicts, U64 conflict_count)
{
 ArenaTemp scratch = {0};
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

//- rjf: main thread entry point

core_function void
BaseMainThreadEntry(void (*entry)(CmdLine *cmdln), U64 argument_count, char **arguments)
{
 ThreadCtx tctx = ThreadCtxAlloc();
 tctx.is_main_thread = 1;
 SetThreadCtx(&tctx);
 String8List args_list = {0};
 for(U64 argument_idx = 1; argument_idx < argument_count; argument_idx += 1)
 {
  Str8ListPush(tctx.arenas[0], &args_list, Str8C(arguments[argument_idx]));
 }
 CmdLine cmdline = CmdLineFromStringList(tctx.arenas[0], args_list);
 entry(&cmdline);
 ThreadCtxRelease(&tctx);
}
