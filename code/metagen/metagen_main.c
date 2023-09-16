#include "metagen/metagen_base/base_inc.h"
#include "metagen/metagen_os/os_inc.h"
#include "metagen/metagen_c_lex/c_lex.h"

#define MD_DEFAULT_SPRINTF 0
#define MD_DEFAULT_ARENA 0
#define MD_IMPL_Arena Arena
#define MD_IMPL_ArenaMinPos sizeof(Arena)
#define MD_IMPL_ArenaAlloc ArenaAllocDefault
#define MD_IMPL_ArenaRelease ArenaRelease
#define MD_IMPL_ArenaGetPos ArenaPos
#define MD_IMPL_ArenaPush ArenaPushNoZero
#define MD_IMPL_ArenaPopTo ArenaPopTo
#define MD_IMPL_ArenaSetAutoAlign ArenaSetAutoAlign
#define MD_IMPL_Vsnprintf ts_stbsp_vsnprintf
#include "third_party/meow_hash_x64_aesni.h"
#include "third_party/metadesk/md.h"

#include "metagen.h"
#include "metagen_embed.h"
#include "metagen_table.h"
#include "metagen_tweak.h"

#include "metagen/metagen_base/base_inc.c"
#include "metagen/metagen_os/os_inc.c"
#include "metagen/metagen_c_lex/c_lex.c"

#include "third_party/metadesk/md.c"

#include "metagen.c"
#include "metagen_embed.c"
#include "metagen_table.c"
#include "metagen_tweak.c"

#include "metagen/metagen_os/core/os_entry_point.c"

////////////////////////////////
//~ rjf: Entry Point

function void
EntryPoint(CmdLine *cmdln)
{
 OS_Init();
 mg_arena = ArenaAllocDefault();
 //ProfBeginCapture("metagen");
 
 //- rjf: find directories
 String8 binary_path = OS_NormalizedPathFromStr8(mg_arena, OS_StringFromSystemPathKind(mg_arena, OS_SystemPathKind_Binary));
 String8 project_path = Str8PathChopLastSlash(Str8PathChopLastSlash(binary_path));
 String8 code_path = PushStr8F(mg_arena, "%S/code/", project_path);
 mg_project_path = project_path;
 
 //- rjf: gather all paths to look at
 String8List path_list = {0};
 ProfScope("gather all paths to look at")
 {
  typedef struct Task Task;
  struct Task
  {
   Task *next;
   String8 dir;
  };
  Task start_task = {0, code_path};
  Task *first_task = &start_task;
  Task *last_task = &start_task;
  for(Task *task = first_task; task != 0; task = task->next)
  {
   OS_FileIter *it = OS_FileIterBegin(mg_arena, task->dir);
   for(OS_FileInfo info = {0}; OS_FileIterNext(mg_arena, it, &info);)
   {
    if(info.attributes.flags & OS_FileFlag_Directory)
    {
     String8 path = PushStr8F(mg_arena, "%S%S/", task->dir, info.name);
     Task *task = PushArray(mg_arena, Task, 1);
     QueuePush(first_task, last_task, task);
     task->dir = path;
    }
    else
    {
     String8 path = PushStr8F(mg_arena, "%S%S", task->dir, info.name);
     Str8ListPush(mg_arena, &path_list, path);
    }
   }
   OS_FileIterEnd(it);
  }
 }
 
 //- rjf: load & lex all C files
 MG_CFileList c_files = {0};
 ProfScope("load & lex all C files")
 {
  for(String8Node *n = path_list.first; n != 0; n = n->next)
  {
   String8 path = n->string;
   B32 is_metacode = FindSubstr8(path, Str8Lit(".meta."), 0, 0) < path.size;
   B32 is_third_party = FindSubstr8(path, Str8Lit("third_party/"), 0, 0) < path.size;
   if(is_metacode || is_third_party)
   {
    continue;
   }
   if(Str8Match(Str8PathSkipLastPeriod(path), Str8Lit("c"), MatchFlag_CaseInsensitive) ||
      Str8Match(Str8PathSkipLastPeriod(path), Str8Lit("h"), MatchFlag_CaseInsensitive))
    ProfScope("%.*s", Str8VArg(path))
   {
    MG_CFile *c_file = PushArray(mg_arena, MG_CFile, 1);
    QueuePush(c_files.first, c_files.last, c_file);
    c_file->path = path;
    c_file->data = OS_DataFromFilePath(mg_arena, path);
    c_file->tokens = CL_TokenArrayFromString(mg_arena, c_file->data);
    c_files.count += 1;
   }
  }
 }
 
 //- rjf: gather metadesk file parses
 MD_Node *md_file_list = MD_MakeList(mg_arena);
 ProfScope("load & parse all metadesk files")
 {
  for(String8Node *n = path_list.first; n != 0; n = n->next)
  {
   String8 path = n->string;
   if(Str8Match(Str8PathSkipLastPeriod(path), Str8Lit("mdesk"), MatchFlag_CaseInsensitive))
    ProfScope("%.*s", Str8VArg(path))
   {
    MD_ParseResult parse = MD_ParseWholeFile(mg_arena, MDFromStr8(path));
    MD_PushNewReference(mg_arena, md_file_list, parse.node);
   }
  }
 }
 
 //- rjf: send all metadesk parses to backends
 ProfScope("[embed] generate") MG_EMBED_Generate(md_file_list);
 ProfScope("[tbl] generate")   MG_TBL_Generate(md_file_list);
 ProfScope("[tweak] generate") MG_TWK_Generate(&c_files);
 
 //- rjf: flush all files
 ProfScope("flush all files")  MG_CloseAllFiles();
 
 //ProfEndCapture();
}
