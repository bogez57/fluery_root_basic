////////////////////////////////
//~ rjf: Includes

//- rjf: [h] root
#include "base/base_inc.h"
#include "mdesk/mdesk.h"
#include "os/os_inc.h"
#include "content/content_inc.h"
#include "file_stream/file_stream.h"
#include "render/render_inc.h"
#include "texture_cache/texture_cache.h"
#include "font_provider/font_provider_inc.h"
#include "font_cache/font_cache.h"
#include "draw/draw.h"

//- rjf: [c] root
#include "base/base_inc.c"
#include "mdesk/mdesk.c"
#include "os/os_inc.c"
#include "content/content_inc.c"
#include "file_stream/file_stream.c"
#include "render/render_inc.c"
#include "texture_cache/texture_cache.c"
#include "font_provider/font_provider_inc.c"
#include "font_cache/font_cache.c"
#include "draw/draw.c"

//- rjf: entry point
#include "os/core/os_entry_point.c"

////////////////////////////////
//~ rjf: Entry Points

function void
EntryPoint(CmdLine *cmdln)
{
 //- rjf: initialize dependency layers
 OS_InitReceipt os_init = OS_Init();
 OS_InitGfxReceipt os_init_gfx = OS_InitGfx(os_init);
 R_InitReceipt r_init = R_Init(os_init, os_init_gfx);
 C_InitReceipt c_init = C_Init(os_init);
 FS_InitReceipt fs_init = FS_Init(os_init, c_init);
 FP_InitReceipt fp_init = FP_Init(c_init);
 F_InitReceipt f_init = F_Init(fp_init, r_init, V2S64(1024, 1024));
 D_InitReceipt d_init = D_Init(r_init, f_init);
 
 //- rjf: open window
 OS_Handle window = OS_WindowOpen(0, V2S64(1280, 720), Str8Lit("Application Template"));
 R_Handle window_r = R_WindowEquip(window);
 OS_WindowFirstPaint(window);
 
 //- rjf: main loop
 for(B32 quit = 0; quit == 0;)
 {
  Temp scratch = ScratchBegin(0, 0);
  
  //- rjf: get events
  OS_EventList events = OS_GetEvents(scratch.arena);
  
  //- rjf: draw
  {
   Rng2F32 client_rect = OS_ClientRectFromWindow(window);
   Vec2F32 client_rect_dim = Dim2F32(client_rect);
   Vec2S64 render_dim = Vec2S64FromVec(client_rect_dim);
   Vec2F32 mouse = OS_MouseFromWindow(window);
   R_WindowStart(window_r, render_dim);
   {
    D_Bucket *bucket = D_BucketMake(scratch.arena);
    D_BucketScope(scratch.arena, bucket)
    {
     local_persist F32 turns = 0.f;
     turns += 0.005f;
     D_Text2D(V2(60, 60), F_TagFromFontPath(Str8Lit("data/app_templates/Roboto.ttf")), 24.f, V4(1, 1, 1, 1), Str8Lit("Hello, World!"));
     D_Transform2D(Mul3x3F32(MakeTranslate3x3F32(mouse), MakeRotate3x3F32(turns)))
     {
      D_Rect2D(R2(V2(-50, -50), V2(+50, +50)), .color = V4(1, 0.85f, 0, 1), .corner = 16.f, .softness = 1.f);
     }
    }
    D_Submit(window_r, bucket);
   }
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
  
  ScratchEnd(scratch);
 }
}
