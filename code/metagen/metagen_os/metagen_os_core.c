////////////////////////////////
//~ rjf: Helpers

//- rjf: handle type functions

root_function OS_Handle
OS_HandleZero(void)
{
 OS_Handle handle = {0};
 return handle;
}

root_function B32
OS_HandleMatch(OS_Handle a, OS_Handle b)
{
 return a.u64[0] == b.u64[0];
}

//- rjf: path normalizations

root_function String8
OS_NormalizedPathFromStr8(Arena *arena, String8 string)
{
 Temp scratch = ScratchBegin(&arena, 1);
 String8 source = OS_GetSystemPath(scratch.arena, OS_SystemPath_Current);
 String8 result = NormalizedPathFromStr8(arena, source, string);
 ScratchEnd(scratch);
 return result;
}

//- rjf: file system interaction bundlers

root_function String8
OS_LoadFile(Arena *arena, String8 path, OS_ErrorList *out_errors)
{
 String8 result = {0};
 OS_Handle file = OS_FileOpen(arena, OS_AccessFlag_Read, path, out_errors);
 OS_FileAttributes atts = OS_AttributesFromFile(file);
 Rng1U64 range = R1U64(0, atts.size);
 result = OS_FileRead(arena, file, range, out_errors);
 OS_FileClose(file);
 return result;
}

root_function void
OS_SaveFile(Arena *arena, String8 path, String8List data, OS_ErrorList *out_errors)
{
 OS_Handle file = OS_FileOpen(arena, OS_AccessFlag_Write, path, out_errors);
 OS_FileWrite(arena, file, 0, data, out_errors);
 OS_FileClose(file);
}

root_function B32
OS_FileExists(String8 path)
{
 Temp scratch = ScratchBegin(0, 0);
 OS_ErrorList errors = {0};
 OS_Handle file = OS_FileOpen(scratch.arena, OS_AccessFlag_Read, path, &errors);
 B32 result = errors.count != 0 && OS_FileIsValid(file);
 OS_FileClose(file);
 return result;
}

//- rjf: stripe table

root_function OS_StripeTable *
OS_StripeTableAlloc(Arena *arena, U64 count)
{
 OS_StripeTable *table = PushArray(arena, OS_StripeTable, 1);
 table->count = count;
 table->stripes = PushArray(arena, OS_Stripe, count);
 for(U64 idx = 0; idx < count; idx += 1)
 {
  table->stripes[idx].cv    = OS_ConditionVariableAlloc();
  table->stripes[idx].mutex = OS_MutexAlloc();
  table->stripes[idx].arena = ArenaAlloc(Gigabytes(1));
 }
 return table;
}

root_function void
OS_StripeTableRelease(OS_StripeTable *table)
{
 for(U64 idx = 0; idx < table->count; idx += 1)
 {
  OS_ConditionVariableRelease(table->stripes[idx].cv);
  OS_MutexRelease(table->stripes[idx].mutex);
  ArenaRelease(table->stripes[idx].arena);
 }
}

//- rjf: timestamp <-> date time

root_function DateTime
OS_DateTimeFromTimestamp(OS_Timestamp timestamp)
{
 DateTime dt = {0};
 {
  // NOTE(rjf): origin => 1/1/1601
  U64 ns_since_origin = timestamp;
  U64 us_since_origin = ns_since_origin/1000;
  U64 ms_since_origin = us_since_origin/1000;
  U64 s_since_origin  = ms_since_origin/1000;
  U64 m_since_origin  = s_since_origin/60;
  U64 h_since_origin  = m_since_origin/60;
  U64 d_since_origin  = h_since_origin/24;
 }
 return dt;
}

root_function OS_Timestamp
OS_TimestampFromDateTime(DateTime date_time)
{
 OS_Timestamp timestamp = 0;
 {
  
 }
 return timestamp;
}
