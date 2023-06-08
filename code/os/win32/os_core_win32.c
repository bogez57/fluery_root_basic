////////////////////////////////
//~ rjf: Globals

#if BUILD_ROOT
HINSTANCE os_w32_hinstance = 0;
OS_W32_State *os_w32_state = 0;
BOOL (*RtlGenRandom)(VOID *RandomBuffer, ULONG RandomBufferLength);
#endif

////////////////////////////////
//~ rjf: File System Helpers

function OS_FileAttributes
OS_W32_FileAttributesFromFindData(WIN32_FIND_DATAW find_data)
{
 OS_FileAttributes attributes = {0};
 if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
 {
  attributes.flags |= OS_FileFlag_Directory;
 }
 attributes.size = (((U64)find_data.nFileSizeHigh) << 32) | (find_data.nFileSizeLow);
 attributes.last_modified = (((U64)find_data.ftLastWriteTime.dwHighDateTime) << 32) | (find_data.ftLastWriteTime.dwLowDateTime);
 return attributes;
}

////////////////////////////////
//~ rjf: Entities

//- rjf: processes

function OS_W32_Process *
OS_W32_ProcessAlloc(void)
{
 OS_W32_Process *result = 0;
 AcquireSRWLockExclusive(&os_w32_state->process_srw_lock);
 {
  result = os_w32_state->first_free_process;
  if(result != 0)
  {
   StackPop(os_w32_state->first_free_process);
  }
  else
  {
   result = PushArrayNoZero(os_w32_state->process_arena, OS_W32_Process, 1);
  }
  MemoryZeroStruct(result);
 }
 ReleaseSRWLockExclusive(&os_w32_state->process_srw_lock);
 return result;
}

function void
OS_W32_ProcessRelease(OS_W32_Process *process)
{
 AcquireSRWLockExclusive(&os_w32_state->process_srw_lock);
 {
  StackPush(os_w32_state->first_free_process, process);
 }
 ReleaseSRWLockExclusive(&os_w32_state->process_srw_lock);
}

//- rjf: threads

function OS_W32_Thread *
OS_W32_ThreadAlloc(void)
{
 OS_W32_Thread *result = 0;
 AcquireSRWLockExclusive(&os_w32_state->thread_srw_lock);
 {
  result = os_w32_state->first_free_thread;
  if(result != 0)
  {
   StackPop(os_w32_state->first_free_thread);
  }
  else
  {
   result = PushArrayNoZero(os_w32_state->thread_arena, OS_W32_Thread, 1);
  }
  MemoryZeroStruct(result);
 }
 ReleaseSRWLockExclusive(&os_w32_state->thread_srw_lock);
 return result;
}

function void
OS_W32_ThreadRelease(OS_W32_Thread *thread)
{
 AcquireSRWLockExclusive(&os_w32_state->thread_srw_lock);
 {
  StackPush(os_w32_state->first_free_thread, thread);
 }
 ReleaseSRWLockExclusive(&os_w32_state->thread_srw_lock);
}

//- rjf: critical sections

function OS_W32_CriticalSection *
OS_W32_CriticalSectionAlloc(void)
{
 OS_W32_CriticalSection *result = 0;
 AcquireSRWLockExclusive(&os_w32_state->critical_section_srw_lock);
 {
  result = os_w32_state->first_free_critical_section;
  if(result != 0)
  {
   StackPop(os_w32_state->first_free_critical_section);
  }
  else
  {
   result = PushArrayNoZero(os_w32_state->critical_section_arena, OS_W32_CriticalSection, 1);
  }
  MemoryZeroStruct(result);
 }
 ReleaseSRWLockExclusive(&os_w32_state->critical_section_srw_lock);
 return result;
}

function void
OS_W32_CriticalSectionRelease(OS_W32_CriticalSection *critical_section)
{
 AcquireSRWLockExclusive(&os_w32_state->critical_section_srw_lock);
 {
  StackPush(os_w32_state->first_free_critical_section, critical_section);
 }
 ReleaseSRWLockExclusive(&os_w32_state->critical_section_srw_lock);
}

//- rjf: srw locks

function OS_W32_SRWLock *
OS_W32_SRWLockAlloc(void)
{
 OS_W32_SRWLock *result = 0;
 AcquireSRWLockExclusive(&os_w32_state->srw_lock_srw_lock);
 {
  result = os_w32_state->first_free_srw_lock;
  if(result != 0)
  {
   StackPop(os_w32_state->first_free_srw_lock);
  }
  else
  {
   result = PushArrayNoZero(os_w32_state->srw_lock_arena, OS_W32_SRWLock, 1);
  }
  MemoryZeroStruct(result);
 }
 ReleaseSRWLockExclusive(&os_w32_state->srw_lock_srw_lock);
 return result;
}

function void
OS_W32_SRWLockRelease(OS_W32_SRWLock *srw_lock)
{
 AcquireSRWLockExclusive(&os_w32_state->srw_lock_srw_lock);
 {
  StackPush(os_w32_state->first_free_srw_lock, srw_lock);
 }
 ReleaseSRWLockExclusive(&os_w32_state->srw_lock_srw_lock);
}

//- rjf: condition variables

function OS_W32_ConditionVariable *
OS_W32_ConditionVariableAlloc(void)
{
 OS_W32_ConditionVariable *cv = 0;
 AcquireSRWLockExclusive(&os_w32_state->condition_variable_srw_lock);
 {
  cv = os_w32_state->first_free_condition_variable;
  if(cv != 0)
  {
   StackPop(os_w32_state->first_free_condition_variable);
  }
  else
  {
   cv = PushArrayNoZero(os_w32_state->condition_variable_arena, OS_W32_ConditionVariable, 1);
  }
  MemoryZeroStruct(cv);
 }
 ReleaseSRWLockExclusive(&os_w32_state->condition_variable_srw_lock);
 return cv;
}

function void
OS_W32_ConditionVariableRelease(OS_W32_ConditionVariable *cv)
{
 AcquireSRWLockExclusive(&os_w32_state->condition_variable_srw_lock);
 {
  StackPush(os_w32_state->first_free_condition_variable, cv);
 }
 ReleaseSRWLockExclusive(&os_w32_state->condition_variable_srw_lock);
}

////////////////////////////////
//~ rjf: Thread Entry Point

function DWORD
OS_W32_ThreadEntryPoint(void *params)
{
 OS_W32_Thread *thread = (OS_W32_Thread *)params;
 ThreadCtx tctx = ThreadCtxAlloc();
 SetThreadCtx(&tctx);
 thread->func(thread->params);
 ThreadCtxRelease(&tctx);
 return 0;
}

////////////////////////////////
//~ rjf: General Program API

core_function OS_InitReceipt
OS_Init(void)
{
 if(IsMainThread() && os_w32_state == 0)
 {
  Arena *arena = ArenaAlloc(Gigabytes(1));
  os_w32_state = PushArray(arena, OS_W32_State, 1);
  os_w32_state->arena = arena;
  os_w32_state->advapi_dll = LoadLibraryA("advapi32.dll");
  if(os_w32_state->advapi_dll)
  {
   *(FARPROC *)&RtlGenRandom = GetProcAddress(os_w32_state->advapi_dll, "SystemFunction036");
  }
  
  GetSystemInfo(&os_w32_state->system_info);
  QueryPerformanceFrequency(&os_w32_state->counts_per_second);
  
  // rjf: initialize entity locks & arenas
  InitializeSRWLock(&os_w32_state->process_srw_lock);
  InitializeSRWLock(&os_w32_state->thread_srw_lock);
  InitializeSRWLock(&os_w32_state->critical_section_srw_lock);
  InitializeSRWLock(&os_w32_state->srw_lock_srw_lock);
  InitializeSRWLock(&os_w32_state->condition_variable_srw_lock);
  os_w32_state->process_arena = ArenaAlloc(Kilobytes(256));
  os_w32_state->thread_arena = ArenaAlloc(Kilobytes(256));
  os_w32_state->critical_section_arena = ArenaAlloc(Kilobytes(256));
  os_w32_state->srw_lock_arena = ArenaAlloc(Kilobytes(256));
  os_w32_state->condition_variable_arena = ArenaAlloc(Kilobytes(256));
  
  // rjf: gather paths
  {
   ArenaTemp scratch = GetScratch(0, 0);
   
   // rjf: gather binary path
   String8 binary_path = {0};
   {
    U64 size = Kilobytes(32);
    U16 *buffer = PushArrayNoZero(scratch.arena, U16, size);
    DWORD length = GetModuleFileNameW(0, (WCHAR*)buffer, size);
    binary_path = Str8From16(scratch.arena, Str16(buffer, length));
    binary_path = Str8PathChopPastLastSlash(binary_path);
   }
   
   // rjf: gather app data path
   String8 app_data_path = {0};
   {
    U64 size = Kilobytes(32);
    U16 *buffer = PushArrayNoZero(scratch.arena, U16, size);
    if(SUCCEEDED(SHGetFolderPathW(0, CSIDL_APPDATA, 0, 0, (WCHAR*)buffer)))
    {
     app_data_path = Str8From16(scratch.arena, Str16C(buffer));
    }
   }
   
   // rjf: commit
   os_w32_state->binary_path = PushStr8Copy(arena, binary_path);
   os_w32_state->initial_path = os_w32_state->binary_path;
   os_w32_state->app_data_path = PushStr8Copy(arena, app_data_path);
   
   ReleaseScratch(scratch);
  }
  
  // rjf: enable tight-granularity sleeps
  os_w32_state->granular_sleep_enabled = timeBeginPeriod(1);
 }
 OS_InitReceipt result = {0};
 return result;
}

core_function void
OS_Abort(void)
{
 ExitProcess(1);
}

core_function String8
OS_GetSystemPath(Arena *arena, OS_SystemPath path)
{
 ArenaTemp scratch = GetScratch(&arena, 1);
 String8 result = {0};
 switch(path)
 {
  case OS_SystemPath_Initial:
  {
   result = os_w32_state->initial_path;
  }break;
  case OS_SystemPath_Current:
  {
   DWORD length = GetCurrentDirectoryW(0, 0);
   U16 *memory = PushArray(scratch.arena, U16, length + 1);
   length = GetCurrentDirectoryW(length + 1, (WCHAR *)memory);
   result = Str8From16(arena, Str16(memory, length));
  }break;
  case OS_SystemPath_Binary:
  {
   result = os_w32_state->binary_path;
  }break;
  case OS_SystemPath_AppData:
  {
   result = os_w32_state->app_data_path;
  }break;
 }
 ReleaseScratch(scratch);
 return result;
}

////////////////////////////////
//~ rjf: Memory

core_function U64
OS_PageSize(void)
{
 SYSTEM_INFO info;
 GetSystemInfo(&info);
 return info.dwPageSize;
}

core_function void *
OS_Reserve(U64 size)
{
 U64 gb_snapped_size = size;
 gb_snapped_size += Gigabytes(1) - 1;
 gb_snapped_size -= gb_snapped_size%Gigabytes(1);
 void *ptr = VirtualAlloc(0, gb_snapped_size, MEM_RESERVE, PAGE_NOACCESS);
 return ptr;
}

core_function void
OS_Release(void *ptr, U64 size)
{
 VirtualFree(ptr, 0, MEM_RELEASE);
}

core_function void
OS_Commit(void *ptr, U64 size)
{
 U64 page_snapped_size = size;
 page_snapped_size += OS_PageSize() - 1;
 page_snapped_size -= page_snapped_size%OS_PageSize();
 VirtualAlloc(ptr, page_snapped_size, MEM_COMMIT, PAGE_READWRITE);
}

core_function void
OS_Decommit(void *ptr, U64 size)
{
 VirtualFree(ptr, size, MEM_DECOMMIT);
}

core_function void
OS_SetMemoryAccessFlags(void *ptr, U64 size, OS_AccessFlags flags)
{
 // rjf: size -> page snapped size
 U64 page_snapped_size = size;
 page_snapped_size += OS_PageSize() - 1;
 page_snapped_size -= page_snapped_size%OS_PageSize();
 
 // rjf: flags -> win32 flags
 DWORD new_flags = 0;
 {
  switch(flags)
  {
   default:
   {
    new_flags = PAGE_NOACCESS;
   }break;
#define Map(win32_code, bitflags) case bitflags:{new_flags = win32_code;}break
   Map(PAGE_EXECUTE,           OS_AccessFlag_Execute);
   Map(PAGE_EXECUTE_READ,      OS_AccessFlag_Execute|OS_AccessFlag_Read);
   Map(PAGE_EXECUTE_READWRITE, OS_AccessFlag_Execute|OS_AccessFlag_Read|OS_AccessFlag_Write);
   Map(PAGE_EXECUTE_WRITECOPY, OS_AccessFlag_Execute|OS_AccessFlag_Write);
   Map(PAGE_READONLY,          OS_AccessFlag_Read);
   Map(PAGE_READWRITE,         OS_AccessFlag_Read|OS_AccessFlag_Write);
#undef Map
  }
 }
 
 // rjf: protect
 DWORD old_flags = 0;
 VirtualProtect(ptr, page_snapped_size, new_flags, &old_flags);
}

////////////////////////////////
//~ rjf: Libraries

core_function OS_Handle
OS_LibraryOpen(String8 path)
{
 ArenaTemp scratch = GetScratch(0, 0);
 String16 path16 = Str16From8(scratch.arena, path);
 HMODULE hmodule = LoadLibraryW((WCHAR*)path16.str);
 ReleaseScratch(scratch);
 OS_Handle handle = {0};
 handle.u64[0] = (U64)hmodule;
 return handle;
}

core_function void
OS_LibraryClose(OS_Handle handle)
{
 HMODULE hmodule = (HMODULE)handle.u64[0];
 FreeLibrary(hmodule);
}

core_function VoidFunction *
OS_LibraryLoadFunction(OS_Handle handle, String8 name)
{
 ArenaTemp scratch = GetScratch(0, 0);
 HMODULE hmodule = (HMODULE)handle.u64[0];
 String8 name_copy = PushStr8Copy(scratch.arena, name);
 VoidFunction *result = (VoidFunction *)GetProcAddress(hmodule, (char *)name_copy.str);
 ReleaseScratch(scratch);
 return result;
}

////////////////////////////////
//~ rjf: File System

//- rjf: granular handle operations

core_function OS_Handle
OS_FileOpen(Arena *arena, OS_AccessFlags access_flags, String8 path, OS_ErrorList *out_errors)
{
 // rjf: unpack args
 ArenaTemp scratch = GetScratch(&arena, 1);
 String16 path16 = Str16From8(scratch.arena, path);
 
 // rjf: map to w32 access flags
 DWORD desired_access = 0;
 if(access_flags & OS_AccessFlag_Read)  { desired_access |= GENERIC_READ; }
 if(access_flags & OS_AccessFlag_Write) { desired_access |= GENERIC_WRITE; }
 
 // rjf: create share mode
 DWORD share_mode = 0;
 
 // rjf: create security attributes
 SECURITY_ATTRIBUTES security_attributes =
 {
  (DWORD)sizeof(SECURITY_ATTRIBUTES),
  0,
  0,
 };
 
 // rjf: map to w32 creation disposition
 DWORD creation_disposition = 0;
 if(!(access_flags & OS_AccessFlag_CreateNew))
 {
  creation_disposition = OPEN_EXISTING;
 }
 
 // rjf: misc.
 DWORD flags_and_attributes = 0;
 HANDLE template_file = 0;
 
 // rjf: open handle
 HANDLE file = CreateFileW((WCHAR*)path16.str,
                           desired_access,
                           share_mode,
                           &security_attributes,
                           creation_disposition,
                           flags_and_attributes,
                           template_file);
 
 // rjf: accumulate errors
 if(file != INVALID_HANDLE_VALUE)
 {
  // TODO(rjf): append to errors
 }
 
 // rjf: map to abstract handle
 OS_Handle handle = {0};
 handle.u64[0] = (U64)file;
 
 ReleaseScratch(scratch);
 return handle;
}

core_function void
OS_FileClose(OS_Handle file)
{
 HANDLE handle = (HANDLE)file.u64[0];
 if(handle != INVALID_HANDLE_VALUE)
 {
  CloseHandle(handle);
 }
}

core_function String8
OS_FileRead(Arena *arena, OS_Handle file, Rng1U64 range, OS_ErrorList *out_errors)
{
 String8 result = {0};
 HANDLE handle = (HANDLE)file.u64[0];
 LARGE_INTEGER off_li = {0};
 off_li.QuadPart = range.min;
 if(handle == INVALID_HANDLE_VALUE)
 {
  // TODO(rjf): accumulate errors
 }
 else if(SetFilePointerEx(handle, off_li, 0, FILE_BEGIN))
 {
  U64 bytes_to_read = Dim1U64(range);
  U64 bytes_actually_read = 0;
  result.str = PushArrayNoZero(arena, U8, bytes_to_read);
  result.size = 0;
  U8 *ptr = result.str;
  U8 *opl = result.str + bytes_to_read;
  for(;;)
  {
   U64 unread = (U64)(opl - ptr);
   DWORD to_read = (DWORD)(ClampTop(unread, U32Max));
   DWORD did_read = 0;
   if(!ReadFile(handle, ptr, to_read, &did_read, 0))
   {
    break;
   }
   ptr += did_read;
   result.size += did_read;
   if(ptr >= opl)
   {
    break;
   }
  }
 }
 return result;
}

core_function void
OS_FileWrite(Arena *arena, OS_Handle file, U64 off, String8List data, OS_ErrorList *out_errors)
{
 HANDLE handle = (HANDLE)file.u64[0];
 LARGE_INTEGER off_li = {0};
 off_li.QuadPart = off;
 if(handle == 0 || handle == INVALID_HANDLE_VALUE)
 {
  // TODO(rjf): accumulate errors
 }
 else if(SetFilePointerEx(handle, off_li, 0, FILE_BEGIN))
 {
  for(String8Node *node = data.first; node != 0; node = node->next)
  {
   U8 *ptr = node->string.str;
   U8 *opl = ptr + node->string.size;
   for(;;)
   {
    U64 unwritten = (U64)(opl - ptr);
    DWORD to_write = (DWORD)(ClampTop(unwritten, U32Max));
    DWORD did_write = 0;
    if(!WriteFile(handle, ptr, to_write, &did_write, 0))
    {
     goto fail_out;
    }
    ptr += did_write;
    if(ptr >= opl)
    {
     break;
    }
   }
  }
 }
 fail_out:;
}

core_function B32
OS_FileIsValid(OS_Handle file)
{
 HANDLE handle = (HANDLE)file.u64[0];
 return handle != INVALID_HANDLE_VALUE;
}

core_function U64
OS_SizeFromFile(OS_Handle file)
{
 HANDLE handle = (HANDLE)file.u64[0];
 U32 high_bits = 0;
 U32 low_bits = GetFileSize(handle, (DWORD *)&high_bits);
 U64 size = (U64)low_bits | (((U64)high_bits) << 32);
 return size;
}

//- rjf: whole-file operations

core_function void
OS_DeleteFile(String8 path)
{
 ArenaTemp scratch = GetScratch(0, 0);
 String16 path16 = Str16From8(scratch.arena, path);
 DeleteFileW((WCHAR *)path16.str);
 ReleaseScratch(scratch);
}

core_function void
OS_MoveFile(String8 dst_path, String8 src_path)
{
 ArenaTemp scratch = GetScratch(0, 0);
 String16 dst_path_16 = Str16From8(scratch.arena, dst_path);
 String16 src_path_16 = Str16From8(scratch.arena, src_path);
 MoveFileW((WCHAR *)src_path_16.str, (WCHAR *)dst_path_16.str);
 ReleaseScratch(scratch);
}

core_function B32
OS_CopyFile(String8 dst_path, String8 src_path)
{
 ArenaTemp scratch = GetScratch(0, 0);
 String16 dst_path_16 = Str16From8(scratch.arena, dst_path);
 String16 src_path_16 = Str16From8(scratch.arena, src_path);
 B32 result = CopyFileW((WCHAR *)src_path_16.str, (WCHAR *)dst_path_16.str, 0);
 ReleaseScratch(scratch);
 return result;
}

core_function B32
OS_MakeDirectory(String8 path)
{
 ArenaTemp scratch = GetScratch(0, 0);
 String16 path16 = Str16From8(scratch.arena, path);
 B32 result = 1;
 if(!CreateDirectoryW((WCHAR *)path16.str, 0))
 {
  result = 0;
 }
 ReleaseScratch(scratch);
 return result;
}

core_function B32
OS_DeleteDirectory(String8 path)
{
 // TODO(rjf)
}

//- rjf: file system introspection

core_function OS_FileIter *
OS_FileIterBegin(Arena *arena, String8 path)
{
 OS_W32_FileFindData *file_find_data = PushArray(arena, OS_W32_FileFindData, 1);
 OS_FileIter *it = (OS_FileIter *)file_find_data;
 path = Str8SkipChopWhitespace(path);
 path = Str8PathChopPastLastSlash(path);
 if(path.size != 0)
 {
  ArenaTemp scratch = GetScratch(&arena, 1);
  path = PushStr8F(scratch.arena, "%S*", path);
  String16 path16 = Str16From8(scratch.arena, path);
  file_find_data->handle = FindFirstFileW((WCHAR *)path16.str, &file_find_data->find_data);
  ReleaseScratch(scratch);
 }
 return it;
}

core_function B32
OS_FileIterNext(Arena *arena, OS_FileIter *it, OS_FileInfo *out_info)
{
 // rjf: grab find data
 B32 result = 0;
 OS_W32_FileFindData *file_find_data = (OS_W32_FileFindData *)it;
 WIN32_FIND_DATAW find_data = {0};
 for(;;)
 {
  // rjf: first, check initial results from FindFirstFile (dumb Windows API... never
  // have 2 entry points (and thus caller codepaths) to return the same stuff!)
  B32 first_was_returned = 0;
  if(!file_find_data->returned_first)
  {
   result = (file_find_data->handle != 0 && file_find_data->handle != INVALID_HANDLE_VALUE);
   find_data = file_find_data->find_data;
   file_find_data->returned_first = 1;
   first_was_returned = 1;
  }
  
  // rjf: if we didn't return the first, OR the first was not good, then proceed to
  // FindNextFile
  if(first_was_returned == 0)
  {
   result = FindNextFileW(file_find_data->handle, &find_data);
  }
  
  // rjf: check for filename validity. if it's invalid, skip.
  B32 filename_is_invalid = (find_data.cFileName[0] == '.' &&
                             (find_data.cFileName[1] == 0 ||
                              find_data.cFileName[1] == '.'));
  if(result == 0 || filename_is_invalid == 0)
  {
   break;
  }
 }
 
 // rjf: fill output
 if(result != 0)
 {
  String16 name16 = {0};
  name16.str = (U16 *)find_data.cFileName;
  name16.size = 0;
  for(U64 idx = 0; idx < MAX_PATH; idx += 1)
  {
   if(find_data.cFileName[idx] == 0)
   {
    break;
   }
   name16.size += 1;
  }
  MemoryZeroStruct(out_info);
  out_info->name = Str8From16(arena, name16);
  out_info->attributes = OS_W32_FileAttributesFromFindData(find_data);
 }
 
 return result;
}

core_function void
OS_FileIterEnd(OS_FileIter *it)
{
 OS_W32_FileFindData *file_find_data = (OS_W32_FileFindData *)it;
 FindClose(file_find_data->handle);
}

core_function OS_FileAttributes
OS_FileAttributesFromPath(String8 path)
{
 WIN32_FIND_DATAW find_data = {0};
 ArenaTemp scratch = GetScratch(0, 0);
 String16 path16 = Str16From8(scratch.arena, path);
 HANDLE handle = FindFirstFileW((WCHAR *)path16.str, &find_data);
 FindClose(handle);
 OS_FileAttributes attributes = OS_W32_FileAttributesFromFindData(find_data);
 ReleaseScratch(scratch);
 return attributes;
}

////////////////////////////////
//~ rjf: Time

core_function DateTime
OS_DateTimeCurrent(void)
{
 SYSTEMTIME st = {0};
 GetSystemTime(&st);
 DateTime dt = {0};
 {
  dt.year         = (U16)st.wYear;
  dt.month        = (U8)st.wMonth;
  dt.day_of_week  = (U8)st.wDayOfWeek;
  dt.day          = (U8)st.wDay;
  dt.hour         = (U8)st.wHour;
  dt.second       = (U8)st.wSecond;
  dt.milliseconds = (U16)st.wMilliseconds;
 }
 return dt;
}

core_function U64
OS_TimeMicroseconds(void)
{
 LARGE_INTEGER current_time;
 QueryPerformanceCounter(&current_time);
 F64 time_in_seconds = ((F64)current_time.QuadPart)/((F64)os_w32_state->counts_per_second.QuadPart);
 U64 time_in_microseconds = (U64)(time_in_seconds * Million(1));
 return time_in_microseconds;
}

core_function U64
OS_TimeCycles(void)
{
 U64 result = __rdtsc();
 return result;
}

core_function void
OS_Sleep(U64 milliseconds)
{
 Sleep(milliseconds);
}

core_function void
OS_Wait(U64 end_time_us)
{
 U64 begin_time_us = OS_TimeMicroseconds();
 if(end_time_us > begin_time_us)
 {
  U64 time_to_wait_us = end_time_us - begin_time_us;
  if(os_w32_state->granular_sleep_enabled)
  {
   OS_Sleep(time_to_wait_us/1000);
  }
  else
  {
   OS_Sleep(time_to_wait_us/15000);
  }
  for(;OS_TimeMicroseconds()<end_time_us;);
 }
}

////////////////////////////////
//~ rjf: Threads & Synchronization Primitives

//- rjf: thread controls

core_function U64
OS_TID(void)
{
 return GetThreadId(0);
}

core_function OS_Handle
OS_ThreadStart(void *params, OS_ThreadFunction *func)
{
 OS_W32_Thread *thread = OS_W32_ThreadAlloc();
 if(thread != 0)
 {
  thread->params = params;
  thread->func = func;
  thread->handle = CreateThread(0, 0, OS_W32_ThreadEntryPoint, thread, 0, &thread->thread_id);
 }
 OS_Handle result = {(U64)(thread)};
 return result;
}

core_function void
OS_ThreadJoin(OS_Handle handle)
{
 OS_W32_Thread *thread = (OS_W32_Thread *)handle.u64[0];
 if(thread != 0)
 {
  if(thread->handle != 0)
  {
   WaitForSingleObject(thread->handle, INFINITE);
   CloseHandle(thread->handle);
  }
  OS_W32_ThreadRelease(thread);
 }
}

core_function void
OS_ThreadDetach(OS_Handle handle)
{
 OS_W32_Thread *thread = (OS_W32_Thread *)handle.u64[0];
 if(thread != 0)
 {
  if(thread->handle != 0)
  {
   CloseHandle(thread->handle);
  }
  OS_W32_ThreadRelease(thread);
 }
}

//- rjf: mutexes

core_function OS_Handle
OS_MutexAlloc(void)
{
 OS_W32_CriticalSection *critical_section = OS_W32_CriticalSectionAlloc();
 InitializeCriticalSection(&critical_section->base);
 OS_Handle handle = {(U64)critical_section};
 return handle;
}

core_function void
OS_MutexRelease(OS_Handle mutex)
{
 OS_W32_CriticalSection *critical_section = (OS_W32_CriticalSection *)mutex.u64[0];
 DeleteCriticalSection(&critical_section->base);
 OS_W32_CriticalSectionRelease(critical_section);
}

core_function void
OS_MutexBlockEnter(OS_Handle mutex)
{
 OS_W32_CriticalSection *critical_section = (OS_W32_CriticalSection *)mutex.u64[0];
 EnterCriticalSection(&critical_section->base);
}

core_function void
OS_MutexBlockLeave(OS_Handle mutex)
{
 OS_W32_CriticalSection *critical_section = (OS_W32_CriticalSection *)mutex.u64[0];
 LeaveCriticalSection(&critical_section->base);
}

//- rjf: slim reader/writer mutexes

core_function OS_Handle
OS_SRWMutexAlloc(void)
{
 OS_W32_SRWLock *lock = OS_W32_SRWLockAlloc();
 OS_Handle h = {(U64)lock};
 return h;
}

core_function void
OS_SRWMutexRelease(OS_Handle mutex)
{
 OS_W32_SRWLock *lock = (OS_W32_SRWLock *)mutex.u64[0];
 OS_W32_SRWLockRelease(lock);
}

core_function void
OS_SRWMutexWriterBlockEnter(OS_Handle mutex)
{
 OS_W32_SRWLock *lock = (OS_W32_SRWLock *)mutex.u64[0];
 AcquireSRWLockExclusive(&lock->lock);
}

core_function void
OS_SRWMutexWriterBlockLeave(OS_Handle mutex)
{
 OS_W32_SRWLock *lock = (OS_W32_SRWLock *)mutex.u64[0];
 ReleaseSRWLockExclusive(&lock->lock);
}

core_function void
OS_SRWMutexReaderBlockEnter(OS_Handle mutex)
{
 OS_W32_SRWLock *lock = (OS_W32_SRWLock *)mutex.u64[0];
 AcquireSRWLockShared(&lock->lock);
}

core_function void
OS_SRWMutexReaderBlockLeave(OS_Handle mutex)
{
 OS_W32_SRWLock *lock = (OS_W32_SRWLock *)mutex.u64[0];
 ReleaseSRWLockShared(&lock->lock);
}

//- rjf: semaphores

core_function OS_Handle
OS_SemaphoreAlloc(U32 initial_count, U32 max_count)
{
 max_count = ClampTop(max_count, U32Max/2);
 initial_count = ClampTop(initial_count, max_count);
 HANDLE handle = CreateSemaphoreA(0, initial_count, max_count, 0);
 OS_Handle result = {0};
 result.u64[0] = (U64)handle;
 return result;
}

core_function void
OS_SemaphoreRelease(OS_Handle handle)
{
 HANDLE h = (HANDLE)handle.u64[0];
 CloseHandle(h);
}

core_function B32
OS_SemaphoreWait(OS_Handle handle, U32 max_milliseconds)
{
 HANDLE h = (HANDLE)handle.u64[0];
 DWORD wait_result = WaitForSingleObject(h, (max_milliseconds == U32Max ? INFINITE : max_milliseconds));
 B32 result = (wait_result == WAIT_OBJECT_0);
 return result;
}

core_function U64 
OS_SemaphoreSignal(OS_Handle handle)
{
 U32 count = 0;
 HANDLE h = (HANDLE)handle.u64[0];
 ReleaseSemaphore(h, 1, (LONG *)(&count));
 U64 result = (U64)count;
 return result;
}

//- rjf: condition variables

core_function OS_Handle
OS_ConditionVariableAlloc(void)
{
 OS_W32_ConditionVariable *cv = OS_W32_ConditionVariableAlloc();
 OS_Handle handle = {(U64)cv};
 return handle;
}

core_function void
OS_ConditionVariableRelease(OS_Handle handle)
{
 OS_W32_ConditionVariable *cv = (OS_W32_ConditionVariable *)handle.u64[0];
 OS_W32_ConditionVariableRelease(cv);
}

core_function B32
OS_ConditionVariableWait(OS_Handle cv_handle, OS_Handle mutex_handle, U64 end_time_microseconds)
{
 OS_W32_ConditionVariable *cv = (OS_W32_ConditionVariable *)cv_handle.u64[0];
 OS_W32_CriticalSection *crit_section = (OS_W32_CriticalSection *)mutex_handle.u64[0];
 U64 begin_time_microseconds = OS_TimeMicroseconds();
 B32 result = 0;
 if(end_time_microseconds > begin_time_microseconds)
 {
  U64 microseconds_to_wait = end_time_microseconds - begin_time_microseconds;
  U64 milliseconds_to_wait = microseconds_to_wait / 1000;
  if(end_time_microseconds == U64Max)
  {
   milliseconds_to_wait = (U64)INFINITE;
  }
  result = !!SleepConditionVariableCS(&cv->base, &crit_section->base, (U32)milliseconds_to_wait);
 }
 return result;
}

core_function void
OS_ConditionVariableSignal(OS_Handle handle)
{
 OS_W32_ConditionVariable *cv = (OS_W32_ConditionVariable *)handle.u64[0];
 WakeConditionVariable(&cv->base);
}

core_function void
OS_ConditionVariableSignalAll(OS_Handle handle)
{
 OS_W32_ConditionVariable *cv = (OS_W32_ConditionVariable *)handle.u64[0];
 WakeAllConditionVariable(&cv->base);
}

////////////////////////////////
//~ rjf: Child Processes

core_function OS_Handle
OS_ProcessLaunch(String8 command, String8 working_directory)
{
 OS_W32_Process *w32_proc = OS_W32_ProcessAlloc();
 
 ArenaTemp scratch = GetScratch(0, 0);
 
 //- rjf: convert inputs
 String8 command_prepped = PushStr8F(scratch.arena, "cmd.exe /C %.*s", Str8VArg(command));
 String16 command16 = Str16From8(scratch.arena, command_prepped);
 String16 dir16 = Str16From8(scratch.arena, working_directory);
 
 //- rjf: make security attributes
 SECURITY_ATTRIBUTES security = {sizeof(security)};
 security.bInheritHandle = TRUE;
 
 //- rjf: setup intermediates
 HANDLE child_write = INVALID_HANDLE_VALUE;
 HANDLE parent_read = INVALID_HANDLE_VALUE;
 HANDLE child_read = INVALID_HANDLE_VALUE;
 
 B32 success = 0;
 
 //- rjf: pipe
 if(CreatePipe(&parent_read, &child_write, &security, 0))
 {
  if(SetHandleInformation(parent_read, HANDLE_FLAG_INHERIT, 0))
  {
   
   //- rjf: nul file (child stdin)
   child_read = CreateFileA("nul",
                            GENERIC_READ,
                            FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                            &security,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            0);
   
   //- rjf: process
   STARTUPINFOW startup = {sizeof(startup)};
   startup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
   startup.hStdInput = child_read;
   startup.hStdOutput = child_write;
   startup.hStdError = child_write;
   startup.wShowWindow = SW_HIDE;
   if(CreateProcessW(L"c:\\windows\\system32\\cmd.exe",
                     (WCHAR*)command16.str,
                     0, 0, TRUE, 0,
                     0,
                     (WCHAR*)dir16.str,
                     &startup,
                     &w32_proc->info))
   {
    success = 1;
    w32_proc->status.running = 1;
    w32_proc->parent_read = parent_read;
    
    CloseHandle(child_write);
    child_write = INVALID_HANDLE_VALUE;
    
    CloseHandle(child_read);
    child_read = INVALID_HANDLE_VALUE;
   }
  }
 }
 
 if(!success)
 {
  w32_proc->status.launch_failed = 1;
  
  if(child_write != INVALID_HANDLE_VALUE)
  {
   CloseHandle(child_write);
  }
  if(parent_read != INVALID_HANDLE_VALUE)
  {
   CloseHandle(parent_read);
  }
  if(child_read != INVALID_HANDLE_VALUE)
  {
   CloseHandle(child_read);
  }
 }
 
 ReleaseScratch(scratch);
 
 OS_Handle result = {0};
 result.u64[0] = IntFromPtr(w32_proc);
 return result;
}

core_function void
OS_ProcessRelease(OS_Handle handle)
{
 OS_W32_Process *w32_proc = (OS_W32_Process *)PtrFromInt(handle.u64[0]);
 if(!w32_proc->status.launch_failed)
 {
  CloseHandle(w32_proc->info.hProcess);
  CloseHandle(w32_proc->info.hThread);
  CloseHandle(w32_proc->parent_read);
 }
 OS_W32_ProcessRelease(w32_proc);
}

core_function String8
OS_ProcessReadOutput(Arena *arena, OS_Handle process)
{
 OS_W32_Process *w32_proc = (OS_W32_Process *)PtrFromInt(process.u64[0]);
 
 //- rjf: if the process has exited, save the exit code.
 B32 proc_exit = 0;
 if(w32_proc->status.running)
 {
  if(WaitForSingleObject(w32_proc->info.hProcess, 0) == WAIT_OBJECT_0)
  {
   proc_exit = 1;
   DWORD exit_code = 0;
   if(GetExitCodeProcess(w32_proc->info.hProcess, &exit_code))
   {
    w32_proc->status.exit_code = exit_code;
   }
  }
 }
 
 //- rjf: read anything that is ready on the pipe.
 String8 result = {0};
 B32 success = 0;
 if(w32_proc->status.running &&
    !w32_proc->status.launch_failed &&
    !w32_proc->status.read_failed &&
    !w32_proc->status.kill_failed)
 {
  DWORD read_size = 0;
  if(PeekNamedPipe(w32_proc->parent_read, 0, 0, 0, &read_size, 0))
  {
   if(read_size == 0)
   {
    success = 1;
   }
   else
   {
    result.str = PushArrayNoZero(arena, U8, read_size);
    result.size = read_size;
    DWORD read_amount = 0;
    if(ReadFile(w32_proc->parent_read, result.str, read_size, &read_amount, 0))
    {
     if(read_amount == read_size)
     {
      success = 1;
     }
    }
   }
  }
 }
 
 //- rjf: mark failure.
 if(!success)
 {
  w32_proc->status.read_failed = 1;
 }
 
 //- rjf: mark exits.
 if(proc_exit)
 {
  w32_proc->status.running = 0;
 }
 
 return result;
}

core_function void
OS_ProcessKill(OS_Handle process)
{
 OS_W32_Process *w32_proc = (OS_W32_Process *)PtrFromInt(process.u64[0]);
 B32 success = 0;
 if(w32_proc->status.running)
 {
  if(TerminateProcess(w32_proc->info.hProcess, 0))
  {
   success = 1;
  }
 }
 if(!success)
 {
  w32_proc->status.kill_failed = 1;
 }
 else
 {
  w32_proc->status.running = 0;
  w32_proc->status.was_killed = 1;
 }
}

core_function U64
OS_PIDFromProcess(OS_Handle process)
{
 OS_W32_Process *w32_proc = (OS_W32_Process *)PtrFromInt(process.u64[0]);
 U64 result = w32_proc->info.dwProcessId;
 return result;
}

core_function OS_ProcessStatus
OS_StatusFromProcess(OS_Handle process)
{
 OS_W32_Process *w32_proc = (OS_W32_Process *)PtrFromInt(process.u64[0]);
 OS_ProcessStatus result = w32_proc->status;
 return result;
}

////////////////////////////////
//~ rjf: Miscellaneous

core_function void
OS_GetEntropy(void *data, U64 size)
{
#if 1
 // NOTE(rjf): Martins says that this method will be faster (as it does not require
 // opening a crypto provider)!
 RtlGenRandom(data, size);
#else
 HCRYPTPROV provider;
 if(CryptAcquireContextW(&provider,
                         NULL,
                         (LPCWSTR)L"Microsoft Base Cryptographic Provider v1.0",
                         PROV_RSA_FULL,
                         CRYPT_VERIFYCONTEXT))
 {
  CryptGenRandom(provider, size, (BYTE*)data);
  CryptReleaseContext(provider, 0);
 }
#endif
}

////////////////////////////////
//~ rjf: @os_per_backend System Info

core_function F32
OS_CaretBlinkTime(void)
{
 F32 seconds = GetCaretBlinkTime() / 1000.f;
 return seconds;
}

core_function F32
OS_DoubleClickTime(void)
{
 F32 seconds = GetDoubleClickTime() / 1000.f;
 return seconds;
}

core_function U64
OS_LogicalProcessorCount(void)
{
 return os_w32_state->system_info.dwNumberOfProcessors;
}
