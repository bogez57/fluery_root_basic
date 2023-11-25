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
#include "ui/ui_inc.h"

//- rjf: [h] demo
#include "hmb_demo/hmb_demo.h"

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
#include "ui/ui_inc.c"

//- rjf: entry point
#include "os/core/os_entry_point.c"

////////////////////////////////
//~ rjf: Demo Functions

function Handle
HandleZero(void)
{
 Handle handle = {0};
 return handle;
}

function Entity *
EntityAlloc(State *state)
{
 Entity *entity = state->first_free_entity;
 if(entity == 0)
 {
  entity = PushArray(state->entities_arena, Entity, 1);
  state->entities_count += 1;
 }
 else
 {
  state->first_free_entity = state->first_free_entity->next;
 }
 entity->is_active = 1;
 return entity;
}

function void
EntityRelease(State *state, Entity *entity)
{
 entity->is_active = 0;
 entity->next = state->first_free_entity;
 state->first_free_entity = entity;
}

function void
EntityEquipName(State *state, Entity *entity, String8 string)
{
 if(entity->name_chunks.count != 0)
 {
  NameChunkListRelease(state, &entity->name_chunks);
 }
 entity->name_chunks = NameChunkListAlloc(state, string);
}

function String8
NameFromEntity(Arena *arena, Entity *entity)
{
 String8 string = {0};
 string.size = entity->name_chunks.total_size;
 string.str = PushArrayNoZero(arena, U8, string.size);
 U64 idx = 0;
 U64 bytes_left = string.size;
 for(NameChunk *chunk = entity->name_chunks.first; chunk != 0; chunk = chunk->next)
 {
  U64 bytes_to_copy = Min(bytes_left, NAME_CHUNK_PAYLOAD_SIZE);
  MemoryCopy(string.str+idx, chunk+1, bytes_to_copy);
  idx += bytes_to_copy;
  bytes_left -= bytes_to_copy;
 }
 return string;
}

function Handle
HandleFromEntity(Entity *entity)
{
 Handle handle = {0};
 if(entity != 0)
 {
  handle.u64[0] = (U64)entity;
  handle.u64[1] = entity->gen;
 };
 return handle;
}

function Entity *
EntityFromHandle(Handle handle)
{
 Entity *result = (Entity *)handle.u64[0];;
 if(result != 0 && handle.u64[1] != result->gen)
 {
  result = 0;
 }
 return result;
}

function NameChunkList
NameChunkListAlloc(State *state, String8 string)
{
 NameChunkList result = {0};
 U64 needed_chunks = (string.size + (NAME_CHUNK_PAYLOAD_SIZE-1)) / NAME_CHUNK_PAYLOAD_SIZE;
 U64 bytes_left = string.size;
 U64 string_off = 0;
 for(U64 idx = 0; idx < needed_chunks; idx += 1)
 {
  NameChunk *chunk = state->first_free_name_chunk;
  if(chunk == 0)
  {
   chunk = (NameChunk *)ArenaPush(state->name_chunk_arena, sizeof(NameChunk)+NAME_CHUNK_PAYLOAD_SIZE);
  }
  else
  {
   state->first_free_name_chunk = state->first_free_name_chunk->next;
  }
  U64 bytes_to_copy = Min(bytes_left, NAME_CHUNK_PAYLOAD_SIZE);
  MemoryCopy(chunk+1, string.str+string_off, bytes_to_copy);
  QueuePush(result.first, result.last, chunk);
  result.count += 1;
  result.total_size += bytes_to_copy;
  bytes_left -= bytes_to_copy;
  string_off += bytes_to_copy;
 }
 return result;
}

function NameChunkList
NameChunkListAllocF(State *state, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 va_end(args);
 NameChunkList result = NameChunkListAlloc(state, string);
 ScratchEnd(scratch);
 return result;
}

function void
NameChunkListRelease(State *state, NameChunkList *list)
{
 for(NameChunk *chunk = list->first, *next = 0;
     chunk != 0;
     chunk = next)
 {
  next = chunk->next;
  chunk->next = state->first_free_name_chunk;
  state->first_free_name_chunk = chunk;
 }
 MemoryZeroStruct(list);
}

function UI_CUSTOM_DRAW_FUNCTION(ArenaFreeListDraw)
{
 ArenaFreeListDrawData *draw_data = (ArenaFreeListDrawData *)box->ext_custom_draw->custom_draw_user_data;
 Arena *arena = draw_data->arena;
 void *free_list_top = draw_data->free_list_top;
 Vec2F32 box_dim = Dim2F32(box->rect);
 U64 idx = 0;
 for(void *free_list_node = free_list_top;
     free_list_node != 0;
     free_list_node = *(void **)free_list_node, idx += 1)
 {
  U64 off = (U64)((U8 *)free_list_node - (U8 *)arena);
  F32 pct = (F64)off / (F64)ArenaPos(arena);
  D_Rect2D(R2(V2(box->rect.x0 + pct*box_dim.x-3.f, box->rect.y0),
              V2(box->rect.x0 + pct*box_dim.x+3.f, box->rect.y1)),
           .color = V4(1, Min(1.f, idx*0.1f), 0, 1),
           .softness = 1.f);
 }
}

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
 FP_InitReceipt fp_init = FP_Init(c_init);
 F_InitReceipt f_init = F_Init(fp_init, r_init, V2S64(1024, 1024));
 D_InitReceipt d_init = D_Init(r_init, f_init);
 
 //- rjf: set up state
 Arena *state_arena = ArenaAlloc(Gigabytes(2));
 State *state = PushArray(state_arena, State, 1);
 state->arena = state_arena;
 state->name_chunk_arena = ArenaAlloc(Gigabytes(64));
 state->entities_arena = ArenaAlloc(Gigabytes(64));
 state->entities_base = PushArray(state->entities_arena, Entity, 0);
 UI_State *ui = UI_StateAlloc();
 
 //- rjf: open window
 OS_Handle window = OS_WindowOpen(0, V2S64(1280, 720), Str8Lit("Handmade Boston Demo"));
 R_Handle window_r = R_WindowEquip(window);
 
 //- rjf: main loop
 U64 frame_idx = 0;
 F32 gravity_t = 0.f;
 F32 sim_dt_t = 1.f;
 B32 diagnostics_panel_on = 0;
 Dist dist = DistMakeUniform(1337);
 for(B32 quit = 0; quit == 0; frame_idx += 1)
 {
  Temp scratch = ScratchBegin(0, 0);
  
  //- rjf: get events
  OS_EventList events = OS_GetEvents(scratch.arena);
  
  //- rjf: update & render
  {
   F32 dt = 1.f/OS_DefaultRefreshRate();
   Rng2F32 client_rect = OS_ClientRectFromWindow(window);
   Vec2F32 client_rect_dim = Dim2F32(client_rect);
   Vec2S64 render_dim = Vec2S64FromVec(client_rect_dim);
   R_BeginFrame();
   D_BeginFrame();
   UI_BeginFrame(dt);
   UI_SelectState(ui);
   R_WindowStart(window_r, render_dim);
   D_Bucket *bucket = D_BucketMake(scratch.arena);
   D_BucketScope(scratch.arena, bucket)
   {
    //- rjf: mouse -> entity
    Entity *mouse_entity = 0;
    if(EntityFromHandle(state->dragging_entity) == 0 && UI_KeyMatch(UI_HotKey(), UI_KeyZero()))
    {
     Entity *entities_first = state->entities_base;
     Entity *entities_opl = entities_first+state->entities_count;
     F32 best_mouse_entity_dist_sq = F32Max;
     for(Entity *entity = entities_first; entity < entities_opl; entity += 1)
     {
      F32 dist_sq = LengthSquared2(Sub2(entity->pos, UI_Mouse()));
      if(dist_sq < best_mouse_entity_dist_sq && dist_sq <= entity->radius*entity->radius)
      {
       mouse_entity = entity;
       best_mouse_entity_dist_sq = dist_sq;
      }
     }
    }
    
    //- rjf: build UI
    UI_BeginBuild(window, &events);
    OS_SetCursor(OS_CursorKind_Null);
    UI_FontSize(12.f)
    {
     //- rjf: mouse entity tooltip
     if(mouse_entity != 0) UI_Tooltip
     {
      Temp scratch = ScratchBegin(0, 0);
      String8 entity_name = NameFromEntity(scratch.arena, mouse_entity);
      UI_Label(entity_name);
      ScratchEnd(scratch);
     }
     
     //- rjf: build arena diagnostics panel
     if(diagnostics_panel_on)
      UI_FixedPos(V2F32(30.f, 30.f))
      UI_PrefWidth(UI_Em(60.f, 1.f))
      UI_PrefHeight(UI_Em(40.f, 1.f))
      UI_CornerRadius(UI_TopFontSize()*0.2f)
     {
      UI_SetNextChildLayoutAxis(Axis2_X);
      UI_Box *panel_box = UI_BoxMakeF(UI_BoxFlag_Clickable|UI_BoxFlag_Floating|UI_BoxFlag_DrawBorder|UI_BoxFlag_DrawDropShadow|UI_BoxFlag_DrawBackground, "panel");
      UI_Parent(panel_box) UI_WidthFill UI_PrefHeight(UI_Em(3.f, 1.f)) UI_Padding(UI_Em(2.f, 1.f))
      {
       UI_Column UI_Padding(UI_Em(2.f, 1.f))
       {
        struct
        {
         Arena *arena;
         String8 name;
         void *free_list_top;
        }
        arenas[] =
        {
         { state->arena, Str8Lit("arena"), 0},
         { state->name_chunk_arena, Str8Lit("name_chunk_arena"), state->first_free_name_chunk},
         { state->entities_arena, Str8Lit("entities_arena"), state->first_free_entity},
        };
        for(U64 idx = 0; idx < ArrayCount(arenas); idx += 1)
        {
         Arena *arena = arenas[idx].arena;
         String8 name = arenas[idx].name;
         void *free_list_top = arenas[idx].free_list_top;
         UI_LabelF("%S (%I64d GiB reserved, %I64d KiB committed)",
                   name,
                   arena->size/Gigabytes(1),
                   ArenaPos(arena)/Kilobytes(1));
         {
          UI_SetNextFlags(UI_BoxFlag_DrawBackground|UI_BoxFlag_DrawBorder);
          UI_SetNextBackgroundColor(V4F32(0.3f, 0.6f, 0.9f, 0.5f));
          UI_Box *box = UI_BoxMakeF(0, "");
          ArenaFreeListDrawData *draw_data = PushArray(UI_FrameArena(), ArenaFreeListDrawData, 1);
          draw_data->arena = arena;
          draw_data->free_list_top = free_list_top;
          UI_BoxEquipCustomDrawFunction(box, ArenaFreeListDraw, draw_data);
         }
        }
       }
      }
      UI_Signal signal = UI_SignalFromBox(panel_box);
     }
     
     //- rjf: build menu
     UI_WidthFill UI_HeightFill
      UI_Row UI_Padding(UI_Pct(1, 0))
      UI_Column
     {
      UI_Spacer(UI_Pct(1, 0));
      UI_PrefHeight(UI_Em(3, 1))
       UI_CornerRadius(UI_TopFontSize()*0.2f)
       UI_TextAlign(UI_TextAlignment_Center)
      {
       Entity *edit_entity = EntityFromHandle(state->edit_entity);
       
       //- rjf: entity editor
       if(edit_entity != 0)
       {
        Temp scratch = ScratchBegin(0, 0);
        String8 entity_name = NameFromEntity(scratch.arena, edit_entity);
        UI_LabelF("Editing \"%S\"", entity_name);
        UI_Spacer(UI_Em(1.f, 1.f));
        UI_SetFocus(1);
        UI_LineEditF(&state->cursor, &state->mark, sizeof(state->name_edit_buffer), state->name_edit_buffer, &state->name_edit_size, "Entity Name");
        EntityEquipName(state, edit_entity, Str8(state->name_edit_buffer, state->name_edit_size));
        UI_Spacer(UI_Em(1.f, 1.f));
        if(UI_ButtonF("Delete").clicked)
        {
         EntityRelease(state, edit_entity);
         state->edit_entity = HandleZero();
        }
        UI_Spacer(UI_Em(1.f, 1.f));
        if(UI_ButtonF("Close").clicked)
        {
         state->edit_entity = HandleZero();
        }
        UI_Spacer(UI_Em(1.f, 1.f));
        ScratchEnd(scratch);
       }
       
       //- rjf: main menu
       else
       {
        if(UI_ButtonF("Spawn Entity").clicked)
        {
         Entity *entity = EntityAlloc(state);
         EntityEquipName(state, entity, Str8Lit("Entity"));
         entity->radius = SampleFromDist1F32(&dist, R1(10.f, 50.f));
         entity->pos = V2F32(SampleFromDist1F32(&dist, R1(entity->radius, client_rect_dim.x-entity->radius)),
                             SampleFromDist1F32(&dist, R1(entity->radius, client_rect_dim.y-entity->radius)));
         entity->vel = V2F32(SampleFromDist1F32(&dist, R1(-200.f, +200.f)),
                             SampleFromDist1F32(&dist, R1(-200.f, +200.f)));
         entity->color = V4F32(SampleFromDist1F32(&dist, R1(0.5f, +1.f)),
                               SampleFromDist1F32(&dist, R1(0.3f, +1.f)),
                               SampleFromDist1F32(&dist, R1(0.1f, +1.f)),
                               SampleFromDist1F32(&dist, R1(0.5f,+1.f)));
        }
        UI_Spacer(UI_Em(1.f, 1.f));
        if(UI_ButtonF("Clear All").clicked)
        {
         ArenaClear(state->name_chunk_arena);
         state->first_free_name_chunk = 0;
         ArenaClear(state->entities_arena);
         state->entities_base = PushArray(state->entities_arena, Entity, 0);
         state->entities_count = 0;
         state->first_free_entity = 0;
        }
        UI_Spacer(UI_Em(1.f, 1.f));
        UI_SliderF32(&gravity_t, R1(-1.f, 1.f), Str8Lit("Gravity"));
        UI_Spacer(UI_Em(1.f, 1.f));
        UI_SliderF32(&sim_dt_t, R1(0.f, 1.f), Str8Lit("Simulation Rate"));
        UI_Spacer(UI_Em(1.f, 1.f));
        if(UI_CheckF(diagnostics_panel_on, "Diagnostics").clicked)
        {
         diagnostics_panel_on ^= 1;
        }
        UI_Spacer(UI_Em(1.f, 1.f));
       }
      }
     }
    }
    UI_EndBuild();
    UI_Layout();
    UI_Animate(dt);
    
    //- rjf: click on entity -> start drag
    if(mouse_entity != 0 && OS_KeyPress(&events, window, OS_Key_MouseLeft, 0))
    {
     state->dragging_entity = HandleFromEntity(mouse_entity);
    }
    
    //- rjf: right-click on entity -> start edit
    if(mouse_entity != 0 && OS_KeyPress(&events, window, OS_Key_MouseRight, 0))
    {
     Temp scratch = ScratchBegin(0, 0);
     String8 name = NameFromEntity(scratch.arena, mouse_entity);
     state->edit_entity = HandleFromEntity(mouse_entity);
     state->cursor = state->mark = TxtPtMake(1, mouse_entity->name_chunks.total_size+1);
     MemoryCopy(state->name_edit_buffer, name.str, Min(sizeof(state->name_edit_buffer), name.size));
     state->name_edit_size = name.size;
     ScratchEnd(scratch);
    }
    
    //- rjf: do drag
    {
     Entity *dragged_entity = EntityFromHandle(state->dragging_entity);
     if(dragged_entity != 0)
     {
      dragged_entity->pos = UI_Mouse();
      if(OS_KeyRelease(&events, window, OS_Key_MouseLeft, 0))
      {
       state->dragging_entity = HandleZero();
      }
     }
    }
    
    //- rjf: simulate entities
    {
     Entity *entities_first = state->entities_base;
     Entity *entities_opl = entities_first+state->entities_count;
     for(Entity *entity = entities_first; entity < entities_opl; entity += 1)
     {
      if(entity->is_active == 0) { continue; }
      entity->vel.y += gravity_t*1000.f*dt*sim_dt_t;
      entity->pos = Add2(entity->pos, Scale2(entity->vel, dt*sim_dt_t));
      if(entity->pos.x-entity->radius < 0)
      {
       entity->vel.x *= -1.f;
      }
      if(entity->pos.y-entity->radius < 0)
      {
       entity->vel.y *= -1.f;
      }
      if(entity->pos.x+entity->radius > client_rect_dim.x)
      {
       entity->vel.x *= -1.f;
      }
      if(entity->pos.y+entity->radius > client_rect_dim.y)
      {
       entity->vel.y *= -1.f;
      }
     }
    }
    
    //- rjf: draw
    {
     D_Rect2D(client_rect, .color = V4(0.1f, 0.1f, 0.1f, 1.f));
     
     //- rjf: draw entities
     {
      Entity *edit_entity = EntityFromHandle(state->edit_entity);
      Entity *entities_first = state->entities_base;
      Entity *entities_opl = entities_first+state->entities_count;
      for(Entity *entity = entities_first; entity < entities_opl; entity += 1)
      {
       if(entity->is_active == 0) { continue; }
       D_Rect2D(R2(Sub2(entity->pos, V2(entity->radius, entity->radius)),
                   Add2(entity->pos, V2(entity->radius, entity->radius))),
                .color = entity->color,
                .corner = entity->radius-1.f,
                .softness = 1.f);
       if(entity == mouse_entity)
       {
        D_Rect2D(R2(Sub2(entity->pos, V2(entity->radius, entity->radius)),
                    Add2(entity->pos, V2(entity->radius, entity->radius))),
                 .color = V4(1, 1, 1, 1),
                 .corner = entity->radius-1.f,
                 .softness = 1.f,
                 .thickness = 4.f);
       }
       if(entity == edit_entity)
       {
        D_Rect2D(R2(Sub2(entity->pos, V2(entity->radius, entity->radius)),
                    Add2(entity->pos, V2(entity->radius, entity->radius))),
                 .color = V4(1, 0, 0, 1),
                 .corner = entity->radius-1.f,
                 .softness = 1.f,
                 .thickness = 4.f);
       }
      }
     }
     
     //- rjf: draw UI
     UI_Draw();
    }
   }
   D_Submit(window_r, bucket);
   R_WindowFinish(window_r);
   UI_EndFrame();
   D_EndFrame();
   R_EndFrame();
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
  
  //- rjf: first paint after first frame
  if(frame_idx == 0)
  {
   OS_WindowFirstPaint(window);
  }
  
  ScratchEnd(scratch);
 }
}
