////////////////////////////////
//~ rjf: Includes

//- rjf: os features
#define OS_FEATURE_GFX 1

//- rjf: [h] hidden grove
#include "base/base_inc.h"
#include "mdesk/mdesk.h"
#include "os/os_inc.h"
#include "content/content_inc.h"
#include "file_io/file_io.h"
#include "render/render_inc.h"
#include "texture_cache/texture_cache.h"
#include "font_provider/font_provider_inc.h"
#include "font_cache/font_cache.h"
#include "draw/draw.h"

//- rjf: [c] hidden grove
#include "base/base_inc.c"
#include "mdesk/mdesk.c"
#include "os/os_inc.c"
#include "content/content_inc.c"
#include "file_io/file_io.c"
#include "render/render_inc.c"
#include "texture_cache/texture_cache.c"
#include "font_provider/font_provider_inc.c"
#include "font_cache/font_cache.c"
#include "draw/draw.c"

//- rjf: entry point
#include "os/os_entry_point.c"

////////////////////////////////
//~ rjf: Entry Points

core_function void
EntryPoint(CmdLine *cmdln)
{
 //- rjf: initialize dependency layers
 OS_InitReceipt os_init = OS_Init();
 OS_InitGfxReceipt os_init_gfx = OS_InitGfx(os_init);
 R_InitReceipt r_init = R_Init(os_init, os_init_gfx);
 
 //- rjf: open window
 OS_Handle window = OS_WindowOpen(0, V2S64(1280, 720), Str8Lit("Application Template"));
 R_Handle window_r = R_WindowEquip(window);
 OS_WindowFirstPaint(window);
 
 //- rjf: main loop
 for(B32 quit = 0; quit == 0;)
 {
  ArenaTemp scratch = GetScratch(0, 0);
  
  //- rjf: get events
  OS_EventList events = OS_GetEvents(scratch.arena);
  
  //- rjf: draw
  {
   Rng2F32 client_rect = OS_ClientRectFromWindow(window);
   Vec2F32 client_rect_dim = Dim2F32(client_rect);
   Vec2S64 render_dim = Vec2S64FromVec(client_rect_dim);
   R_WindowStart(window_r, render_dim);
   R_WindowFinish(window_r);
  }
  
  //- rjf: process window close events
  for(OS_Event *event = events.first; event != 0; event = event->next)
  {
   if(event->kind == OS_EventKind_WindowClose)
   {
    quit = 1;
    break;
   }
  }
  
  ReleaseScratch(scratch);
 }
}
