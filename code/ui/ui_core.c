////////////////////////////////
//~ rjf: Globals

#if BUILD_CORE
F32 ui_g_cursor_blink_t = 0;
read_only UI_BoxTextExt ui_g_nil_box_text_ext = {0};
read_only UI_BoxRectStyleExt ui_g_nil_box_rect_style_ext = {0};
read_only UI_BoxBucketExt ui_g_nil_box_bucket_ext = {0};
read_only UI_BoxCustomDrawExt ui_g_nil_box_custom_draw_ext = {0};
read_only UI_Box ui_g_nil_box =
{
 &ui_g_nil_box,
 &ui_g_nil_box,
 &ui_g_nil_box,
 &ui_g_nil_box,
 &ui_g_nil_box,
 &ui_g_nil_box,
 &ui_g_nil_box,
};
#endif

////////////////////////////////
//~ rjf: Thread-Locals

per_thread UI_State *ui_state = 0;

////////////////////////////////
//~ rjf: Basic Type Functions

//- rjf: id strings

root_function String8
UI_DisplayPartFromBoxString(String8 string)
{
 U64 double_pound_pos = FindSubstr8(string, Str8Lit("##"), 0, 0);
 if(double_pound_pos < string.size)
 {
  string.size = double_pound_pos;
 }
 return string;
}

root_function String8
UI_HashPartFromBoxString(String8 string)
{
 U64 triple_pound_pos = FindSubstr8(string, Str8Lit("###"), 0, 0);
 if(triple_pound_pos < string.size)
 {
  string = Str8Skip(string, triple_pound_pos);
 }
 return string;
}

//- rjf: keys

root_function UI_Key
UI_KeyZero(void)
{
 UI_Key key = {0};
 return key;
}

root_function UI_Key
UI_KeyFromString(UI_Key seed, String8 string)
{
 UI_Key key = {0};
 if(string.size != 0)
 {
  MemoryCopyStruct(&key, &seed);
  for(U64 i = 0; i < string.size; i += 1)
  {
   key.u64[0] = ((key.u64[0] << 5) + key.u64[0]) + string.str[i];
  }
 }
 return key;
}

root_function UI_Key
UI_KeyFromStringF(UI_Key seed, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Key result = UI_KeyFromString(seed, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

root_function B32
UI_KeyMatch(UI_Key a, UI_Key b)
{
 return (a.u64[0] == b.u64[0]);
}

//- rjf: boxes

root_function B32
UI_BoxIsNil(UI_Box *box)
{
 return box == 0 || box == &ui_g_nil_box;
}

root_function UI_BoxRec
UI_BoxRecurseDepthFirst(UI_Box *box, UI_Box *stopper, MemberOffset sib, MemberOffset child)
{
 UI_BoxRec rec = {0};
 rec.next = &ui_g_nil_box;
 if(!UI_BoxIsNil(MemberFromOff(box, UI_Box *, child)))
 {
  rec.next = MemberFromOff(box, UI_Box *, child);
  rec.push_count = 1;
 }
 else for(UI_Box *b = box; !UI_BoxIsNil(b) && b != stopper; b = b->parent)
 {
  if(!UI_BoxIsNil(MemberFromOff(b, UI_Box *, sib)))
  {
   rec.next = MemberFromOff(b, UI_Box *, sib);
   break;
  }
  rec.pop_count += 1;
 }
 return rec;
}

//- rjf: sizes

root_function UI_Size
UI_SizeMake(UI_SizeKind kind, F32 value, F32 strictness)
{
 UI_Size result = {0};
 result.kind = kind;
 result.value = value;
 result.strictness = strictness;
 return result;
}

////////////////////////////////
//~ rjf: Global Concepts

root_function F_Tag
UI_IconFont(void)
{
 U128 hash = ui_g_icon_font_hash;
 return F_TagFromHash(hash);
}

root_function F32
UI_CursorBlinkT(void)
{
 return ui_g_cursor_blink_t;
}

root_function void
UI_ResetCursorBlinkT(void)
{
 ui_g_cursor_blink_t = 0;
}

////////////////////////////////
//~ rjf: Frame Boundaries

root_function void
UI_BeginFrame(F32 delta_time)
{
 ui_g_cursor_blink_t += delta_time;
}

root_function void
UI_EndFrame(void)
{
 
}

////////////////////////////////
//~ rjf: State Allocation & Selection

root_function UI_State *
UI_StateAlloc(void)
{
 Arena *arena = ArenaAlloc(Gigabytes(64));
 UI_State *state = PushArray(arena, UI_State, 1);
 state->arena = arena;
 state->box_table_size = 4096;
 state->box_table = PushArray(arena, UI_BoxSlot, state->box_table_size);
 for(U64 idx = 0; idx < ArrayCount(state->frame_arenas); idx += 1)
 {
  state->frame_arenas[idx] = ArenaAlloc(Gigabytes(8));
 }
 state->drag_data_arena = ArenaAlloc(Megabytes(64));
 C_SubmitStaticData(ui_g_mono_font, ui_g_mono_font_hash);
 C_SubmitStaticData(ui_g_icon_font, ui_g_icon_font_hash);
 UI_InitStackNils(state);
 return state;
}

root_function void
UI_StateRelease(UI_State *state)
{
 ArenaRelease(state->drag_data_arena);
 for(U64 idx = 0; idx < ArrayCount(state->frame_arenas); idx += 1)
 {
  ArenaRelease(state->frame_arenas[idx]);
 }
 ArenaRelease(state->arena);
}

root_function void
UI_SelectState(UI_State *state)
{
 ui_state = state;
}

root_function UI_State *
UI_SelectedState(void)
{
 return ui_state;
}

////////////////////////////////
//~ rjf: Selected State Accessors

root_function Arena *
UI_FrameArena(void)
{
 return ui_state->frame_arenas[ui_state->build_gen%ArrayCount(ui_state->frame_arenas)];
}

root_function UI_Box *
UI_Root(void)
{
 return ui_state->root;
}

root_function OS_Handle
UI_Window(void)
{
 return ui_state->window;
}

root_function OS_EventList *
UI_Events(void)
{
 return ui_state->events;
}

root_function Vec2F32
UI_Mouse(void)
{
 return ui_state->mouse;
}

root_function Vec2F32
UI_DragDelta(void)
{
 Vec2F32 result = Sub2F32(UI_Mouse(), ui_state->drag_start_mouse);
 return result;
}

root_function UI_CursorVizData
UI_GetCursorVizData(void)
{
 return ui_state->cursor_viz_data;
}

root_function UI_Box *
UI_BoxFromKey(UI_Key key)
{
 UI_Box *result = &ui_g_nil_box;
 U64 slot = key.u64[0] % ui_state->box_table_size;
 if(!UI_KeyMatch(key, UI_KeyZero()))
 {
  for(UI_Box *b = ui_state->box_table[slot].first; !UI_BoxIsNil(b); b = b->hash_next)
  {
   if(UI_KeyMatch(b->key, key))
   {
    result = b;
    break;
   }
  }
 }
 return result;
}

root_function UI_Key
UI_HotKey(void)
{
 return ui_state->hot_key;
}

root_function UI_Key
UI_ActiveKey(Side side)
{
 return ui_state->active_key[side];
}

////////////////////////////////
//~ rjf: Selected State Mutators

root_function void
UI_SetCursorViz(UI_Key key, B32 is_active, Vec2F32 p, F32 line_height, F32 advance)
{
 ui_state->cursor_viz_data.is_active = is_active;
 ui_state->target_cursor_p = p;
 ui_state->target_cursor_line_height = line_height;
 ui_state->target_cursor_advance = advance;
 if(!UI_KeyMatch(ui_state->cursor_viz_data.key, key))
 {
  ui_state->cursor_viz_data.p = ui_state->target_cursor_p;
  ui_state->cursor_viz_data.velocity = 0;
  ui_state->cursor_viz_data.line_height = ui_state->target_cursor_line_height;
 }
 ui_state->cursor_viz_data.key = key;
 ui_state->cursor_viz_data_build_gen = ui_state->build_gen;
}

////////////////////////////////
//~ rjf: Drag Data

root_function void
UI_StoreDragData(String8 data)
{
 ArenaClear(ui_state->drag_data_arena);
 ui_state->drag_data = PushStr8Copy(ui_state->drag_data_arena, data);
}

root_function String8
UI_LoadDragData(U64 needed_size)
{
 if(ui_state->drag_data.size < needed_size)
 {
  ArenaClear(ui_state->drag_data_arena);
  ui_state->drag_data = PushStr8FillByte(ui_state->drag_data_arena, needed_size, 0);
 }
 return ui_state->drag_data;
}

////////////////////////////////
//~ rjf: Layout Passes

root_function void
UI_SolveIndependentSizes(UI_Box *root, Axis2 axis)
{
 switch(root->pref_size[axis].kind)
 {
  default:break;
  case UI_SizeKind_Pixels:
  {
   root->calc_size.v[axis] = root->pref_size[axis].value;
   root->calc_size.v[axis] = FloorF32(root->calc_size.v[axis]);
  }break;
  case UI_SizeKind_TextDim:
  {
   String8 display_string = UI_DisplayStringFromBox(root);
   switch(axis)
   {
    case Axis2_X:
    {
     root->calc_size.v[axis] = F_AdvanceFromFontSizeString(root->ext_text->font_tag,
                                                           root->ext_text->font_size,
                                                           display_string);
     root->calc_size.v[axis] += root->ext_text->text_edge_padding*2.f;
     root->calc_size.v[axis] = CeilF32(root->calc_size.v[axis]);
    }break;
    
    case Axis2_Y:
    {
     F_Metrics metrics = F_MetricsFromTag(root->ext_text->font_tag, root->ext_text->font_size);
     root->calc_size.v[axis] = metrics.line_gap + metrics.ascent + metrics.descent;
     root->calc_size.v[axis] = FloorF32(root->calc_size.v[axis]);
    }break;
   }
  }break;
 }
 for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
 {
  UI_SolveIndependentSizes(child, axis);
 }
}

root_function void
UI_SolveUpwardDependentSizes(UI_Box *root, Axis2 axis)
{
 switch(root->pref_size[axis].kind)
 {
  default:break;
  case UI_SizeKind_Pct:
  {
   UI_Box *ancestor = &ui_g_nil_box;
   for(UI_Box *p = root->parent; !UI_BoxIsNil(p); p = p->parent)
   {
    if(p->pref_size[axis].kind != UI_SizeKind_SizeByChildren)
    {
     ancestor = p;
     break;
    }
   }
   if(!UI_BoxIsNil(ancestor))
   {
    root->calc_size.v[axis] = ancestor->calc_size.v[axis] * root->pref_size[axis].value;
    root->calc_size.v[axis] = FloorF32(root->calc_size.v[axis]);
   }
  }break;
 }
 for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
 {
  UI_SolveUpwardDependentSizes(child, axis);
 }
}

root_function void
UI_SolveDownwardDependentSizes(UI_Box *root, Axis2 axis)
{
 for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
 {
  UI_SolveDownwardDependentSizes(child, axis);
 }
 switch(root->pref_size[axis].kind)
 {
  default:break;
  case UI_SizeKind_SizeByChildren:
  {
   F32 value = 0;
   {
    if(axis == root->child_layout_axis)
    {
     for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
     {
      value += child->calc_size.v[axis];
     }
    }
    else
    {
     for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
     {
      value = Max(value, child->calc_size.v[axis]);
     }
    }
   }
   root->calc_size.v[axis] = value;
   root->calc_size.v[axis] = FloorF32(root->calc_size.v[axis]);
  }break;
 }
}

root_function void
UI_SolveSizeViolations(UI_Box *root, Axis2 axis)
{
 //- rjf: determine the maximum available space
 F32 available_space = root->calc_size.v[axis];
 
 //- rjf: determine the size taken by all of the children's preferred sizes, &
 // the total budget we have to fix the sizes up
 F32 taken_space = 0;
 F32 total_fixup_budget = 0;
 if(!(root->flags & (UI_BoxFlag_OverflowX<<axis)))
 {
  for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
  {
   if(!(child->flags & (UI_BoxFlag_FloatingX<<axis)))
   {
    if(axis == root->child_layout_axis)
    {
     taken_space += child->calc_size.v[axis];
    }
    else
    {
     taken_space = Max(taken_space, child->calc_size.v[axis]);
    }
    F32 fixup_budget_this_child = child->calc_size.v[axis] * (1 - child->pref_size[axis].strictness);
    total_fixup_budget += fixup_budget_this_child;
   }
  }
 }
 
 //- rjf: fixup all children as much as possible
 if(!(root->flags & (UI_BoxFlag_OverflowX<<axis)))
 {
  F32 violation = taken_space - available_space;
  if(violation > 0 && total_fixup_budget > 0)
  {
   for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
   {
    if(!(child->flags & (UI_BoxFlag_FloatingX<<axis)))
    {
     F32 fixup_budget_this_child = child->calc_size.v[axis] * (1 - child->pref_size[axis].strictness);
     F32 fixup_size_this_child = 0;
     if(axis == root->child_layout_axis)
     {
      fixup_size_this_child = fixup_budget_this_child * (violation / total_fixup_budget);
     }
     else
     {
      fixup_size_this_child = child->calc_size.v[axis] - available_space;
     }
     fixup_size_this_child = Clamp(0, fixup_size_this_child, fixup_budget_this_child);
     child->calc_size.v[axis] -= fixup_size_this_child;
     child->calc_size.v[axis] = FloorF32(child->calc_size.v[axis]);
    }
   }
  }
 }
 
 //- rjf: position all children
 {
  if(axis == root->child_layout_axis)
  {
   F32 p = 0;
   for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
   {
    if(!(child->flags & (UI_BoxFlag_FloatingX<<axis)))
    {
     child->calc_rel_pos.v[axis] = p;
     p += child->calc_size.v[axis];
    }
   }
  }
  else
  {
   for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
   {
    if(!(child->flags & (UI_BoxFlag_FloatingX<<axis)))
    {
     child->calc_rel_pos.v[axis] = 0;
    }
   }
  }
  for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
  {
   Rng2F32 last_rel_rect = child->rel_rect;
   child->rel_rect.p0.v[axis] = child->calc_rel_pos.v[axis];
   child->rel_rect.p1.v[axis] = child->rel_rect.p0.v[axis] + child->calc_size.v[axis];
   Vec2F32 last_corner_01 = V2F32(last_rel_rect.x0, last_rel_rect.y1);
   Vec2F32 last_corner_10 = V2F32(last_rel_rect.x1, last_rel_rect.y0);
   Vec2F32 this_corner_01 = V2F32(child->rel_rect.x0, child->rel_rect.y1);
   Vec2F32 this_corner_10 = V2F32(child->rel_rect.x1, child->rel_rect.y0);
   child->rel_corner_delta[Corner_00].v[axis] = child->rel_rect.p0.v[axis] - last_rel_rect.p0.v[axis];
   child->rel_corner_delta[Corner_01].v[axis] = this_corner_01.v[axis] - last_corner_01.v[axis];
   child->rel_corner_delta[Corner_10].v[axis] = this_corner_10.v[axis] - last_corner_10.v[axis];
   child->rel_corner_delta[Corner_11].v[axis] = child->rel_rect.p1.v[axis] - last_rel_rect.p1.v[axis];
   child->rect.p0.v[axis] = root->rect.p0.v[axis] + child->rel_rect.p0.v[axis] - root->view_off.v[axis];
   child->rect.p1.v[axis] = child->rect.p0.v[axis] + child->calc_size.v[axis];
   if(!(child->flags & (UI_BoxFlag_FloatingX<<axis)))
   {
    child->rect.p0.v[axis] = FloorF32(child->rect.p0.v[axis]);
    child->rect.p1.v[axis] = FloorF32(child->rect.p1.v[axis]);
   }
  }
 }
 
 //- rjf: recurse
 for(UI_Box *child = root->first; !UI_BoxIsNil(child); child = child->next)
 {
  UI_SolveSizeViolations(child, axis);
 }
}

root_function void
UI_LayoutRoot(UI_Box *root, Axis2 axis)
{
 UI_SolveIndependentSizes(root, axis);
 UI_SolveUpwardDependentSizes(root, axis);
 UI_SolveDownwardDependentSizes(root, axis);
 UI_SolveSizeViolations(root, axis);
}

root_function void
UI_Layout(void)
{
 //- rjf: position the tooltip root
 {
  Rng2F32 client_rect = OS_ClientRectFromWindow(UI_Window());
  Vec2F32 client_dim = Dim2F32(client_rect);
  Vec2F32 mouse = UI_Mouse();
  UI_Box *root = ui_state->tooltip_root;
  root->calc_rel_pos.x = mouse.x + UI_TopFontSize() * 2.f;
  root->calc_rel_pos.y = mouse.y;
  root->calc_rel_pos.x = ClampTop(root->calc_rel_pos.x, client_dim.x - root->calc_size.x);
  root->calc_rel_pos.y = ClampTop(root->calc_rel_pos.y, client_dim.y - root->calc_size.y);
  root->calc_rel_pos.x = ClampBot(root->calc_rel_pos.x, 0);
  root->calc_rel_pos.y = ClampBot(root->calc_rel_pos.y, 0);
 }
 
 //- rjf: position the context menu root
 if(!UI_KeyMatch(UI_KeyZero(), ui_state->ctx_menu_key))
 {
  Rng2F32 client_rect = OS_ClientRectFromWindow(UI_Window());
  Vec2F32 client_dim = Dim2F32(client_rect);
  UI_Box *anchor_box = UI_BoxFromKey(ui_state->ctx_menu_anchor_key);
  Vec2F32 anchor_off = ui_state->ctx_menu_anchor_offset;
  Vec2F32 ctx_menu_p =
  {
   anchor_box->rect.x0 + anchor_off.x,
   anchor_box->rect.y1 + anchor_off.y,
  };
  UI_Box *root = ui_state->ctx_menu_root;
  root->calc_rel_pos.x = ctx_menu_p.x;
  root->calc_rel_pos.y = ctx_menu_p.y;
  root->calc_rel_pos.x = ClampTop(root->calc_rel_pos.x, client_dim.x - root->calc_size.x);
  root->calc_rel_pos.y = ClampTop(root->calc_rel_pos.y, client_dim.y - root->calc_size.y);
  root->calc_rel_pos.x = ClampBot(root->calc_rel_pos.x, 0);
  root->calc_rel_pos.y = ClampBot(root->calc_rel_pos.y, 0);
  root->rect.y1 = root->rect.y0 + (root->rect.y1 - root->rect.y0) * ui_state->ctx_menu_t;
 }
 
 //- rjf: layout pass for the whole subtree
 for(Axis2 axis = (Axis2)0; axis < Axis2_COUNT; axis = (Axis2)(axis+1))
 {
  UI_LayoutRoot(UI_Root(), axis);
 }
}

////////////////////////////////
//~ rjf: Animation Passes

root_function void
UI_AnimateRoot(UI_Box *root, F32 delta_time)
{
 //- rjf: calculate animation rates
 F32 slow_rate = 1 - Pow(2.f, -20.f * delta_time);
 F32 fast_rate = 1 - Pow(2.f, -50.f * delta_time);
 
 //- rjf: animate all boxes
 for(U64 slot = 0; slot < ui_state->box_table_size; slot += 1)
 {
  for(UI_Box *box = ui_state->box_table[slot].first; !UI_BoxIsNil(box); box = box->hash_next)
  {
   B32 is_hot          = UI_KeyMatch(ui_state->hot_key, box->key);
   B32 is_active       = UI_KeyMatch(ui_state->active_key[Side_Min], box->key);
   B32 is_disabled     = !!(box->flags & UI_BoxFlag_Disabled);
   B32 is_focus_hot    = !!(box->flags & UI_BoxFlag_FocusHot)    && !(box->flags & UI_BoxFlag_FocusHotDisabled);
   B32 is_focus_active = !!(box->flags & UI_BoxFlag_FocusActive) && !(box->flags & UI_BoxFlag_FocusActiveDisabled);
   box->hot_t              += ((F32)!!is_hot    - box->hot_t)                  * fast_rate;
   box->active_t           += ((F32)!!is_active - box->active_t)               * fast_rate;
   box->disabled_t         += ((F32)!!is_disabled - box->disabled_t)           * fast_rate;
   box->focus_hot_t        += ((F32)!!is_focus_hot - box->focus_hot_t)         * fast_rate;
   box->focus_active_t     += ((F32)!!is_focus_active - box->focus_active_t)   * fast_rate;
   box->view_off.x         += (box->target_view_off.x - box->view_off.x)       * fast_rate;
   box->view_off.y         += (box->target_view_off.y - box->view_off.y)       * fast_rate;
   if(AbsoluteValueF32(box->view_off.x - box->target_view_off.x) <= 1)
   {
    box->view_off.x = box->target_view_off.x;
   }
   if(AbsoluteValueF32(box->view_off.y - box->target_view_off.y) <= 1)
   {
    box->view_off.y = box->target_view_off.y;
   }
  }
 }
}

root_function void
UI_Animate(F32 delta_time)
{
 //- rjf: calculate animation rates
 F32 slow_rate = 1 - Pow(2.f, -20.f * delta_time);
 F32 fast_rate = 1 - Pow(2.f, -50.f * delta_time);
 
 //- rjf: animate entire tree
 UI_AnimateRoot(UI_Root(), delta_time);
 
 //- rjf: animate cursor viz data
 {
  ui_state->cursor_viz_data.velocity = ui_state->target_cursor_p.x - ui_state->cursor_viz_data.p.x;
  ui_state->cursor_viz_data.p.x += (ui_state->target_cursor_p.x - ui_state->cursor_viz_data.p.x) * fast_rate;
  ui_state->cursor_viz_data.p.y += (ui_state->target_cursor_p.y - ui_state->cursor_viz_data.p.y) * fast_rate;
  ui_state->cursor_viz_data.line_height += (ui_state->target_cursor_line_height - ui_state->cursor_viz_data.line_height) * fast_rate;
  ui_state->cursor_viz_data.advance += (ui_state->target_cursor_advance - ui_state->cursor_viz_data.advance) * fast_rate;
 }
 
 //- rjf: animate context menu
 {
  B32 ctx_menu_is_open = !UI_KeyMatch(UI_KeyZero(), ui_state->ctx_menu_key);
  ui_state->ctx_menu_t += fast_rate * ((F32)ctx_menu_is_open - ui_state->ctx_menu_t);
  if(ctx_menu_is_open && AbsoluteValueF32(ui_state->ctx_menu_t - 1.f) < 0.01f)
  {
   ui_state->ctx_menu_t = 1;
  }
 }
}

////////////////////////////////
//~ rjf: Build Phase Markers

root_function void
UI_BeginBuild(OS_Handle window, OS_EventList *events)
{
 //- rjf: reset per-frame state
 ui_state->build_gen += 1;
 ArenaClear(UI_FrameArena());
 ui_state->root = &ui_g_nil_box;
 ui_state->window = window;
 ui_state->events = events;
 if(ui_state->cursor_viz_data_build_gen+1 < ui_state->build_gen)
 {
  MemoryZeroStruct(&ui_state->cursor_viz_data);
  ui_state->target_cursor_p = V2F32(0, 0);
  ui_state->target_cursor_line_height = ui_state->target_cursor_advance = 0;
 }
 ui_state->hot_key_found_this_frame = 0;
 ui_state->mouse = OS_MouseFromWindow(window);
 UI_InitStacks(ui_state);
 
 //- rjf: kill action
 if(ui_state->action_killed_this_frame)
 {
  ui_state->action_killed_this_frame = 0;
  MemoryZeroStruct(&ui_state->hot_key);
  MemoryZeroArray(ui_state->active_key);
 }
 
 //- rjf: prune all of the stale boxes
 for(U64 slot = 0; slot < ui_state->box_table_size; slot += 1)
 {
  for(UI_Box *box = ui_state->box_table[slot].first, *next = 0;
      !UI_BoxIsNil(box);
      box = next)
  {
   next = box->hash_next;
   if(UI_KeyMatch(box->key, UI_KeyZero()) || box->last_gen_touched+1 < ui_state->build_gen)
   {
    DLLRemove_NPZ(ui_state->box_table[slot].first, ui_state->box_table[slot].last, box, hash_next, hash_prev, UI_BoxIsNil, UI_BoxSetNil);
    StackPush(ui_state->first_free_box, box);
    ui_state->free_box_list_count += 1;
   }
  }
 }
 
 //- rjf: zero keys on pruned boxes
 {
  // rjf: release on active keys
  {
   for(Side side = (Side)0; side < Side_COUNT; side = (Side)(side+1))
   {
    UI_Box *box = UI_BoxFromKey(ui_state->active_key[side]);
    if(UI_BoxIsNil(box))
    {
     OS_Key key = (side == Side_Min ? OS_Key_MouseLeft : OS_Key_MouseRight);
     B32 release = 0;
     for(OS_Event *event = events->first; event != 0; event = event->next)
     {
      if(OS_HandleMatch(window, event->window) && event->kind == OS_EventKind_Release && event->key == key)
      {
       release = 1;
       break;
      }
     }
     if(release)
     {
      MemoryZeroStruct(&ui_state->active_key[side]);
     }
    }
   }
  }
  
  // rjf: hot key
  UI_Box *box = UI_BoxFromKey(ui_state->hot_key);
  if(UI_BoxIsNil(box) &&
     (UI_KeyMatch(UI_KeyZero(), ui_state->active_key[Side_Min]) ||
      !UI_KeyMatch(ui_state->hot_key, ui_state->active_key[Side_Min])) &&
     (UI_KeyMatch(UI_KeyZero(), ui_state->active_key[Side_Max]) ||
      !UI_KeyMatch(ui_state->hot_key, ui_state->active_key[Side_Max])))
  {
   ui_state->hot_key = UI_KeyZero();
  }
 }
 
 //- rjf: build root
 Rng2F32 client_rect = OS_ClientRectFromWindow(window);
 Vec2F32 client_rect_size = Dim2F32(client_rect);
 UI_SetNextPrefWidth(UI_Pixels(client_rect_size.x, 1.f));
 UI_SetNextPrefHeight(UI_Pixels(client_rect_size.y, 1.f));
 UI_SetNextChildLayoutAxis(Axis2_Y);
 UI_Box *root = UI_BoxMakeF(0, "window_root_%" PRIx64 "", window.u64[0]);
 
 //- rjf: begin root
 UI_PushParent(root);
 
 //- rjf: build tooltip root
 UI_SetNextPrefWidth(UI_SizeByChildren(1));
 UI_SetNextPrefHeight(UI_SizeByChildren(1));
 ui_state->tooltip_root = UI_BoxMakeF(UI_BoxFlag_Floating, "tooltip_root_%I64x", window.u64[0]);
 
 //- rjf: build ctx menu root
 UI_SetNextPrefWidth(UI_SizeByChildren(1));
 UI_SetNextPrefHeight(UI_SizeByChildren(1));
 ui_state->ctx_menu_root =
  UI_BoxMakeF(UI_BoxFlag_Clip|UI_BoxFlag_FloatingX|UI_BoxFlag_FloatingY, "ctx_menu_root_%" PRIx64 "", window.u64[0]);
 
 //- rjf: take events for exiting the context menu
 if(!UI_KeyMatch(ui_state->ctx_menu_key, UI_KeyZero()) && OS_KeyPress(events, window, OS_Key_Esc, 0))
 {
  UI_CloseCtxMenu();
 }
}

root_function void
UI_EndBuild(void)
{
 //- rjf: end root
 UI_PopParent();
}

////////////////////////////////
//~ rjf: Box Hierarchy Construction API

//- rjf: lowest level box construction

root_function UI_Box *
UI_BoxMakeFromKey(UI_BoxFlags flags, UI_Key key)
{
 //- rjf: map the key to a box
 UI_Box *box = UI_BoxFromKey(key);
 
 //- rjf: key has already been used - trample over key/box to become
 // id-less box
 if(box->last_gen_touched == ui_state->build_gen)
 {
  box = &ui_g_nil_box;
  key = UI_KeyZero();
 }
 
 //- rjf: allocate the box if it has not been allocated yet, or if this
 // is a duplicate key
 B32 first_frame = 0;
 if(UI_BoxIsNil(box))
 {
  U64 slot = key.u64[0] % ui_state->box_table_size;
  first_frame = 1;
  box = ui_state->first_free_box;
  if(UI_BoxIsNil(box))
  {
   box = PushArray(ui_state->arena, UI_Box, 1);
  }
  else
  {
   StackPop(ui_state->first_free_box);
   MemoryZeroStruct(box);
   ui_state->free_box_list_count -= 1;
  }
  DLLPushBack_NPZ(ui_state->box_table[slot].first, ui_state->box_table[slot].last, box, hash_next, hash_prev, UI_BoxIsNil, UI_BoxSetNil);
  box->key = key;
 }
 
 //- rjf: link it into the tree
 UI_Box *parent = UI_TopParent();
 if(UI_BoxIsNil(parent))
 {
  ui_state->root = box;
 }
 else
 {
  DLLPushBack_NPZ(parent->first, parent->last, box, next, prev, UI_BoxIsNil, UI_BoxSetNil);
  parent->child_count += 1;
  box->parent = parent;
 }
 
 //- rjf: fill state
 if(!UI_BoxIsNil(box))
 {
  // rjf: fill per-frame state
  box->child_count = 0;
  box->first = box->last = &ui_g_nil_box;
  box->flags = flags|UI_TopFlags();
  box->flags |= UI_BoxFlag_FocusHot            * !!ui_state->focus_hot_set_stack.top->v;
  box->flags |= UI_BoxFlag_FocusHotDisabled    * (!ui_state->focus_hot_set_stack.top->v && ui_state->focus_hot_possible_stack.top->v);
  box->flags |= UI_BoxFlag_FocusActive         * !!ui_state->focus_active_set_stack.top->v;
  box->flags |= UI_BoxFlag_FocusActiveDisabled * (!ui_state->focus_active_set_stack.top->v && ui_state->focus_active_possible_stack.top->v);
  box->pref_size[Axis2_X] = UI_TopPrefWidth();
  box->pref_size[Axis2_Y] = UI_TopPrefHeight();
  box->child_layout_axis  = UI_TopChildLayoutAxis();
  box->hover_cursor       = UI_TopHoverCursor();
  box->opacity            = UI_TopOpacity();
  box->last_gen_touched   = ui_state->build_gen;
  
  // rjf: fill default extensions
  box->ext_text = &ui_g_nil_box_text_ext;
  box->ext_rect_style = &ui_g_nil_box_rect_style_ext;
  box->ext_bucket = &ui_g_nil_box_bucket_ext;
  box->ext_custom_draw = &ui_g_nil_box_custom_draw_ext;
  
  // rjf: equip with extensions related to feature flags
  if(box->flags & UI_BoxFlag_DrawText)
  {
   box->ext_text = PushArray(UI_FrameArena(), UI_BoxTextExt, 1);
   box->ext_text->string_alignment  = UI_TopTextAlign();
   box->ext_text->text_edge_padding = UI_TopTextEdgePadding();
   box->ext_text->font_tag          = UI_TopFont();
   box->ext_text->font_size         = UI_TopFontSize();
   box->ext_text->text_color        = UI_TopTextColor();
  }
  if(box->flags & (UI_BoxFlag_DrawBackground|UI_BoxFlag_DrawBorder|UI_BoxFlag_DrawOverlay))
  {
   box->ext_rect_style = PushArray(UI_FrameArena(), UI_BoxRectStyleExt, 1);
   box->ext_rect_style->background_color                  = UI_TopBackgroundColor();
   box->ext_rect_style->border_color                      = UI_TopBorderColor();
   box->ext_rect_style->overlay_color                     = UI_TopOverlayColor();
   box->ext_rect_style->corner_radii[Corner_00]           = UI_TopCornerRadius00();
   box->ext_rect_style->corner_radii[Corner_01]           = UI_TopCornerRadius01();
   box->ext_rect_style->corner_radii[Corner_10]           = UI_TopCornerRadius10();
   box->ext_rect_style->corner_radii[Corner_11]           = UI_TopCornerRadius11();
   box->ext_rect_style->border_thickness                  = UI_TopBorderThickness();
   box->ext_rect_style->slice                             = UI_TopSlice2F32();
  }
  
  // rjf: fill fixed positions
  box->calc_rel_pos.x = UI_TopFixedX();
  box->calc_rel_pos.y = UI_TopFixedY();
  
  // rjf: fill first-frame state
  if(first_frame)
  {
   box->first_gen_touched = ui_state->build_gen;
  }
  
  // rjf: is focused -> disable per stack
  if(box->flags & UI_BoxFlag_FocusHot && !UI_IsFocusHot())
  {
   box->flags |= UI_BoxFlag_FocusHotDisabled;
  }
  if(box->flags & UI_BoxFlag_FocusActive && !UI_IsFocusActive())
  {
   box->flags |= UI_BoxFlag_FocusActiveDisabled;
  }
 }
 
 //- rjf: auto-pop
 UI_AutoPopStacks(ui_state);
 
 //- rjf: return
 return box;
}

root_function UI_Box *
UI_BoxMake(UI_BoxFlags flags, String8 string)
{
 //- rjf: grab seed
 UI_Key seed = UI_TopSeedKey();
 
 //- rjf: produce a key from the string
 String8 string_hash_part = UI_HashPartFromBoxString(string);
 UI_Key key = UI_KeyFromString(seed, string_hash_part);
 
 //- rjf: build the box from the key
 UI_Box *box = UI_BoxMakeFromKey(flags, key);
 
 //- rjf: defaultly equip the passed string to this box
 box->string = PushStr8Copy(UI_FrameArena(), string);
 
 return box;
}

root_function UI_Box *
UI_BoxMakeF(UI_BoxFlags flags, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Box *result = UI_BoxMake(flags, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

//- rjf: extra box equipment

root_function void
UI_BoxEquipDisplayString(UI_Box *box, String8 string)
{
 box->string = PushStr8Copy(UI_FrameArena(), string);
 box->flags |= UI_BoxFlag_DisableStringHashPart;
}

root_function void
UI_BoxEquipDrawBucket(UI_Box *box, D_Bucket *bucket, F32 anchor_weight_00, F32 anchor_weight_01, F32 anchor_weight_10, F32 anchor_weight_11)
{
 box->flags |= UI_BoxFlag_DrawBucket;
 box->ext_bucket = PushArray(UI_FrameArena(), UI_BoxBucketExt, 1);
 box->ext_bucket->bucket = bucket;
 box->ext_bucket->bucket_anchor_weights[Corner_00] = anchor_weight_00;
 box->ext_bucket->bucket_anchor_weights[Corner_01] = anchor_weight_01;
 box->ext_bucket->bucket_anchor_weights[Corner_10] = anchor_weight_10;
 box->ext_bucket->bucket_anchor_weights[Corner_11] = anchor_weight_11;
}

root_function void
UI_BoxEquipCustomDrawFunction(UI_Box *box, UI_BoxCustomDrawFunctionType *CustomDraw, void *user_data)
{
 box->flags |= UI_BoxFlag_DrawCustomFunction;
 box->ext_custom_draw = PushArray(UI_FrameArena(), UI_BoxCustomDrawExt, 1);
 box->ext_custom_draw->CustomDraw = CustomDraw;
 box->ext_custom_draw->custom_draw_user_data = user_data;
}

//- rjf: tooltip

root_function void
UI_TooltipBaseBegin(void)
{
 UI_PushParent(ui_state->tooltip_root);
 UI_Box *tooltip_section = UI_BoxMake(0, Str8Lit(""));
 UI_PushParent(tooltip_section);
}

root_function void
UI_TooltipBaseEnd(void)
{
 UI_PopParent();
 UI_PopParent();
}

root_function void
UI_TooltipBegin(void)
{
 Vec4F32 background_color = UI_TopBackgroundColor();
 background_color.alpha = ClampBot(background_color.alpha, 0.7f);
 
 // rjf: apply default styles to this tooltip section
 UI_SetNextBackgroundColor(background_color);
 UI_SetNextFlags(UI_BoxFlag_DrawBorder|UI_BoxFlag_DrawBackground|UI_BoxFlag_DrawDropShadow);
 UI_SetNextPrefWidth(UI_SizeByChildren(1));
 UI_SetNextPrefHeight(UI_SizeByChildren(1));
 UI_SetNextChildLayoutAxis(Axis2_X);
 UI_SetNextCornerRadius(UI_TopFontSize() * 0.4f);
 
 // rjf: begin tooltip section
 UI_TooltipBaseBegin();
 UI_Spacer(UI_Em(0.25f, 1.f));
 
 // rjf: begin column
 UI_SetNextPrefWidth(UI_SizeByChildren(1));
 UI_SetNextPrefHeight(UI_SizeByChildren(1));
 UI_ColumnBegin();
 UI_Spacer(UI_Em(0.25f, 1.f));
 
 // rjf: push default sizes
 UI_PushPrefWidth(UI_TextDim(1));
 UI_PushPrefHeight(UI_Em(1.5f, 1.f));
}

root_function void
UI_TooltipEnd(void)
{
 // rjf: pop default sizes
 UI_PopPrefWidth();
 UI_PopPrefHeight();
 
 // rjf: end column
 UI_Spacer(UI_Em(0.25f, 1.f));
 UI_ColumnEnd();
 
 // rjf: end tooltip section
 UI_Spacer(UI_Em(0.25f, 1.f));
 UI_TooltipBaseEnd();
}

//- rjf: box interaction

root_function UI_Signal
UI_SignalFromBox(UI_Box *box)
{
 UI_Signal sig = {0};
 Rng2F32 rect = box->rect;
 Vec2F32 mouse = UI_Mouse();
 B32 mouse_is_over = Contains2F32(rect, mouse);
 sig.box = box;
 
 //- rjf: detect clips & mutate mouse_is_over
 {
  Rng2F32 clip_rect = OS_ClientRectFromWindow(UI_Window());
  for(UI_Box *parent = box->parent; !UI_BoxIsNil(parent); parent = parent->parent)
  {
   if(parent->flags & UI_BoxFlag_Clip)
   {
    clip_rect = Intersection2F32(clip_rect, parent->rect);
   }
  }
  if(!Contains2F32(clip_rect, mouse))
  {
   mouse_is_over = 0;
  }
 }
 
 //- rjf: detect if we're building the context menu
 B32 in_ctx_menu = 0;
 for(UI_Box *parent = box->parent; !UI_BoxIsNil(parent); parent = parent->parent)
 {
  if(parent == ui_state->ctx_menu_root)
  {
   in_ctx_menu = 1;
   break;
  }
 }
 
 //- rjf: detect hovering over context menu
 if(!in_ctx_menu)
 {
  Rng2F32 ctx_rect = ui_state->ctx_menu_root->rect;
  if(Contains2F32(ctx_rect, mouse))
  {
   mouse_is_over = 0;
  }
 }
 
 //- rjf: commit mouse_is_over to signal
 sig.mouse_is_over = mouse_is_over;
 
 //- rjf: mouse clickability
 if(box->first_gen_touched != box->last_gen_touched && box->flags & UI_BoxFlag_MouseClickable)
 {
  // rjf: get mouse events
  OS_Event *left_press_event = 0;
  OS_Event *right_press_event = 0;
  OS_Event *left_release_event = 0;
  OS_Event *right_release_event = 0;
  for(OS_Event *event = UI_Events()->first; event != 0; event = event->next)
  {
   if(OS_HandleMatch(event->window, UI_Window()))
   {
    if(event->kind == OS_EventKind_Press && event->key == OS_Key_MouseLeft)
    {
     left_press_event = event;
    }
    if(event->kind == OS_EventKind_Press && event->key == OS_Key_MouseRight)
    {
     right_press_event = event;
    }
    if(event->kind == OS_EventKind_Release && event->key == OS_Key_MouseLeft)
    {
     left_release_event = event;
    }
    if(event->kind == OS_EventKind_Release && event->key == OS_Key_MouseRight)
    {
     right_release_event = event;
    }
   }
  }
  
  // rjf: hot key mgmt
  if(UI_KeyMatch(ui_state->active_key[Side_Min], UI_KeyZero()) &&
     (UI_KeyMatch(ui_state->hot_key, UI_KeyZero()) || ui_state->hot_key_found_this_frame == 0) &&
     mouse_is_over)
  {
   ui_state->hot_key = box->key;
   ui_state->hot_key_found_this_frame = 1;
  }
  else if(UI_KeyMatch(ui_state->hot_key, box->key) && (mouse_is_over || UI_KeyMatch(ui_state->active_key[Side_Min], box->key)))
  {
   ui_state->hot_key = box->key;
   ui_state->hot_key_found_this_frame = 1;
  }
  else if(UI_KeyMatch(ui_state->hot_key, box->key) && !mouse_is_over)
  {
   ui_state->hot_key = UI_KeyZero();
  }
  
  // rjf: left-side active key mgmt & clickability
  if(UI_KeyMatch(ui_state->hot_key, box->key) &&
     UI_KeyMatch(ui_state->active_key[Side_Min], UI_KeyZero()) &&
     left_press_event != 0)
  {
   OS_EatEvent(UI_Events(), left_press_event);
   ui_state->active_key[Side_Min] = box->key;
   ui_state->drag_start_mouse = mouse;
   sig.pressed = 1;
   sig.dragging = 1;
   sig.modifiers |= left_press_event->modifiers;
  }
  else if(UI_KeyMatch(ui_state->active_key[Side_Min], box->key))
  {
   sig.dragging = 1;
   sig.modifiers |= OS_GetModifiers();
   if(left_release_event != 0)
   {
    OS_EatEvent(UI_Events(), left_release_event);
    ui_state->active_key[Side_Min] = UI_KeyZero();
    sig.clicked = mouse_is_over;
    sig.released = 1;
    sig.modifiers |= left_release_event->modifiers;
   }
  }
  
  // rjf: right-side active key mgmt & clickability
  if(UI_KeyMatch(ui_state->hot_key, box->key) &&
     UI_KeyMatch(ui_state->active_key[Side_Max], UI_KeyZero()) &&
     right_press_event)
  {
   OS_EatEvent(UI_Events(), right_press_event);
   ui_state->active_key[Side_Max] = box->key;
   ui_state->drag_start_mouse = mouse;
   sig.right_pressed = 1;
   sig.right_dragging = 1;
   sig.modifiers |= right_press_event->modifiers;
  }
  else if(UI_KeyMatch(ui_state->active_key[Side_Max], box->key))
  {
   sig.right_dragging = 1;
   sig.modifiers |= OS_GetModifiers();
   if(right_release_event != 0)
   {
    OS_EatEvent(UI_Events(), right_release_event);
    ui_state->active_key[Side_Max] = UI_KeyZero();
    sig.right_clicked = mouse_is_over;
    sig.right_released = 1;
    sig.modifiers |= right_release_event->modifiers;
   }
  }
  
  // rjf: hover cursor
  if(UI_KeyMatch(ui_state->hot_key, box->key) && OS_WindowIsFocused(UI_Window()))
  {
   if(box->flags & UI_BoxFlag_Disabled)
   {
    OS_SetCursor(OS_CursorKind_Blocked);
   }
   else
   {
    OS_SetCursor(box->hover_cursor);
   }
  }
  
  // rjf: hot -> hover
  if(UI_KeyMatch(ui_state->hot_key, box->key) &&
     UI_KeyMatch(ui_state->active_key[Side_Min], UI_KeyZero()) &&
     UI_KeyMatch(ui_state->active_key[Side_Max], UI_KeyZero()) &&
     mouse_is_over)
  {
   sig.hovering = 1;
  }
 }
 
 //- rjf: keyboard-focus & clickability => enter -> click + press
 if(box->flags & UI_BoxFlag_KeyboardClickable && box->flags & UI_BoxFlag_FocusHot && OS_KeyPress(UI_Events(), UI_Window(), OS_Key_Enter, 0))
 {
  sig.clicked = 1;
  sig.pressed = 1;
  sig.keyboard_pressed = 1;
 }
 
 //- rjf: scrollability
 if(box->flags & UI_BoxFlag_ViewScroll && mouse_is_over)
 {
  // rjf: layout pre-emptively for this subtree
  if(box->flags & UI_BoxFlag_OverflowX)
  {
   UI_LayoutRoot(box, Axis2_X);
  }
  if(box->flags & UI_BoxFlag_OverflowY)
  {
   UI_LayoutRoot(box, Axis2_Y);
  }
  
  // rjf: take scroll events
  OS_EventList *events = UI_Events();
  OS_Handle window = UI_Window();
  for(OS_Event *event = events->first, *next = 0; event != 0; event = next)
  {
   next = event->next;
   if(OS_HandleMatch(event->window, window) && event->kind == OS_EventKind_MouseScroll)
   {
    box->target_view_off.y += event->scroll.y;
    OS_EatEvent(events, event);
   }
  }
  
  // rjf: clamp
  {
   Rng1F32 scroll_bounds[Axis2_COUNT] =
   {
    UI_ScrollBoundsFromBox(box, Axis2_X),
    UI_ScrollBoundsFromBox(box, Axis2_Y),
   };
   for(Axis2 axis = (Axis2)0; axis < Axis2_COUNT; axis = (Axis2)(axis+1))
   {
    box->target_view_off.v[axis] = Clamp(scroll_bounds[axis].min, box->target_view_off.v[axis], scroll_bounds[axis].max);
   }
  }
 }
 
 //- rjf: trample over certain signals on disabled/cancel
 if(box->flags & UI_BoxFlag_Disabled ||
    ui_state->action_killed_this_frame)
 {
  sig.clicked = sig.pressed = sig.double_clicked = sig.right_clicked = sig.right_pressed = sig.keyboard_pressed = sig.dragging = 0;
 }
 
 return sig;
}

root_function void
UI_KillAction(void)
{
 ui_state->action_killed_this_frame = 1;
}

////////////////////////////////
//~ rjf: Box Accessors

root_function String8
UI_DisplayStringFromBox(UI_Box *box)
{
 String8 result = box->string;
 if(!(box->flags & UI_BoxFlag_DisableStringHashPart))
 {
  result = UI_DisplayPartFromBoxString(result);
 }
 return result;
}

root_function Vec2F32
UI_TextPosFromBox(UI_Box *box)
{
 F_Tag tag = box->ext_text->font_tag;
 F_Metrics metrics = F_MetricsFromTag(tag, box->ext_text->font_size);
 Vec2F32 result = {0};
 {
  result.y = (box->rect.y0+box->rect.y1)/2 + metrics.ascent/2 - metrics.descent/4;
 }
 switch(box->ext_text->string_alignment)
 {
  default:
  case UI_TextAlignment_Left:
  {
   result.x = box->rect.x0 + box->ext_text->text_edge_padding;
  }break;
  case UI_TextAlignment_Center:
  {
   String8 display_string = UI_DisplayStringFromBox(box);
   F32 string_advance = F_AdvanceFromFontSizeString(box->ext_text->font_tag, box->ext_text->font_size, display_string);
   result.x = (box->rect.x0 + box->rect.x1)/2 - string_advance/2;
  }break;
  case UI_TextAlignment_Right:
  {
   String8 display_string = UI_DisplayStringFromBox(box);
   F32 string_advance = F_AdvanceFromFontSizeString(box->ext_text->font_tag, box->ext_text->font_size, display_string);
   result.x = (box->rect.x1 - box->ext_text->text_edge_padding) - string_advance;
  }break;
 }
 return result;
}

root_function Rng1F32
UI_ScrollBoundsFromBox(UI_Box *box, Axis2 axis)
{
 Rng1F32 bounds = {0};
 for(UI_Box *child = box->first; !UI_BoxIsNil(child); child = child->next)
 {
  bounds.min = Min(bounds.min, child->calc_rel_pos.v[axis]);
  bounds.max = Max(bounds.max, child->calc_rel_pos.v[axis]);
 }
 return bounds;
}

////////////////////////////////
//~ rjf: Focus

root_function B32
UI_IsFocusHot(void)
{
 B32 result = 0;
 UI_FocusHotSetNode *set_n = ui_state->focus_hot_set_stack.top;
 UI_FocusHotPossibleNode *pos_n = ui_state->focus_hot_possible_stack.top;
 if(set_n != 0 && pos_n != 0)
 {
  result = 1;
  for(;set_n != 0 && pos_n != 0; set_n = set_n->next, pos_n = pos_n->next)
  {
   if(!set_n->v && pos_n->v)
   {
    result = 0;
    break;
   }
  }
 }
 return result;
}

root_function B32
UI_IsFocusActive(void)
{
 B32 result = 0;
 UI_FocusActiveSetNode *set_n = ui_state->focus_active_set_stack.top;
 UI_FocusActivePossibleNode *pos_n = ui_state->focus_active_possible_stack.top;
 if(set_n != 0 && pos_n != 0)
 {
  result = 1;
  for(;set_n != 0 && pos_n != 0; set_n = set_n->next, pos_n = pos_n->next)
  {
   if(!set_n->v && pos_n->v)
   {
    result = 0;
    break;
   }
  }
 }
 return result;
}

////////////////////////////////
//~ rjf: Context Menus

root_function void
UI_OpenCtxMenu(UI_Key anchor_box_key, Vec2F32 anchor_offset, UI_Key ctx_menu_key)
{
 ui_state->ctx_menu_key = ctx_menu_key;
 ui_state->ctx_menu_anchor_key = anchor_box_key;
 ui_state->ctx_menu_anchor_offset = anchor_offset;
 ui_state->ctx_menu_t = 0;
}

root_function void
UI_CloseCtxMenu(void)
{
 ui_state->ctx_menu_key = UI_KeyZero();
}

root_function B32
UI_CtxMenuIsOpen(UI_Key key)
{
 B32 is_open = UI_KeyMatch(ui_state->ctx_menu_key, key);
 return is_open;
}

root_function B32
UI_CtxMenuBegin(UI_Key key)
{
 B32 is_open = UI_KeyMatch(ui_state->ctx_menu_key, key);
 UI_PushParent(ui_state->ctx_menu_root);
 UI_PushSeedKey(ui_state->ctx_menu_root->key);
 
 // rjf: apply ctx-menu-wide styles if we have an open context menu
 if(is_open)
 {
  ui_state->ctx_menu_root->flags |= UI_BoxFlag_DrawDropShadow|UI_BoxFlag_DrawBorder;
 }
 
 // rjf: grab bg color
 Vec4F32 background_color = UI_TopBackgroundColor();
 background_color.alpha = ClampBot(background_color.alpha, 0.7f);
 
 // rjf: apply default styles to this ctx menu
 UI_SetNextBackgroundColor(background_color);
 UI_SetNextFlags(UI_BoxFlag_DrawBorder|UI_BoxFlag_DrawBackground);
 UI_SetNextPrefWidth(UI_SizeByChildren(1));
 UI_SetNextPrefHeight(UI_SizeByChildren(1));
 UI_SetNextChildLayoutAxis(Axis2_Y);
 UI_SetNextCornerRadius(UI_TopFontSize() * 0.4f);
 
 // rjf: begin column
 UI_ColumnBegin();
 
 return is_open;
}

root_function void
UI_CtxMenuEnd(void)
{
 UI_ColumnEnd();
 UI_PopSeedKey();
 UI_PopParent();
}

////////////////////////////////
//~ rjf: Stacks

root_function void
UI_PushCornerRadius(F32 v)
{
 UI_PushCornerRadius00(v);
 UI_PushCornerRadius01(v);
 UI_PushCornerRadius10(v);
 UI_PushCornerRadius11(v);
}

root_function void
UI_PopCornerRadius(void)
{
 UI_PopCornerRadius00();
 UI_PopCornerRadius01();
 UI_PopCornerRadius10();
 UI_PopCornerRadius11();
}

root_function void
UI_SetNextCornerRadius(F32 v)
{
 UI_SetNextCornerRadius00(v);
 UI_SetNextCornerRadius01(v);
 UI_SetNextCornerRadius10(v);
 UI_SetNextCornerRadius11(v);
}

root_function void
UI_PushPrefSize(Axis2 axis, UI_Size v)
{
 (axis == Axis2_X ? UI_PushPrefWidth : UI_PushPrefHeight)(v);
}

root_function void
UI_PopPrefSize(Axis2 axis)
{
 (axis == Axis2_X ? UI_PopPrefWidth : UI_PopPrefHeight)();
}

root_function void
UI_SetNextPrefSize(Axis2 axis, UI_Size v)
{
 (axis == Axis2_X ? UI_SetNextPrefWidth : UI_SetNextPrefHeight)(v);
}

root_function void
UI_PushFixedPos(Vec2F32 v)
{
 UI_PushFixedX(v.x);
 UI_PushFixedY(v.y);
}

root_function void
UI_PopFixedPos(void)
{
 UI_PopFixedX();
 UI_PopFixedY();
}

root_function void
UI_SetNextFixedPos(Vec2F32 v)
{
 UI_SetNextFixedX(v.x);
 UI_SetNextFixedY(v.y);
}

root_function void
UI_PushFixedRect(Rng2F32 rect)
{
 Vec2F32 dim = Dim2F32(rect);
 UI_PushFixedPos(rect.p0);
 UI_PushPrefSize(Axis2_X, UI_Pixels(dim.x, 1));
 UI_PushPrefSize(Axis2_Y, UI_Pixels(dim.y, 1));
}

root_function void
UI_PopFixedRect(void)
{
 UI_PopFixedPos();
 UI_PopPrefSize(Axis2_X);
 UI_PopPrefSize(Axis2_Y);
}

root_function void
UI_SetNextFixedRect(Rng2F32 rect)
{
 Vec2F32 dim = Dim2F32(rect);
 UI_SetNextFixedPos(rect.p0);
 UI_SetNextPrefSize(Axis2_X, UI_Pixels(dim.x, 1));
 UI_SetNextPrefSize(Axis2_Y, UI_Pixels(dim.y, 1));
}

////////////////////////////////
//~ rjf: Generated Code

#define UI_StackTopImpl(state, name_upper, name_lower) \
return state->name_lower##_stack.top->v;

#define UI_StackPushImpl(state, name_upper, name_lower, type, new_value) \
UI_##name_upper##Node *node = state->name_lower##_stack.free;\
if(node != 0) {StackPop(state->name_lower##_stack.free);}\
else {node = PushArray(UI_FrameArena(), UI_##name_upper##Node, 1);}\
type old_value = state->name_lower##_stack.top->v;\
node->v = new_value;\
StackPush(state->name_lower##_stack.top, node);\
state->name_lower##_stack.auto_pop = 0;\
return old_value;

#define UI_StackPopImpl(state, name_upper, name_lower) \
UI_##name_upper##Node *popped = state->name_lower##_stack.top;\
if(popped != &state->name_lower##_nil_stack_top)\
{\
StackPop(state->name_lower##_stack.top);\
StackPush(state->name_lower##_stack.free, popped);\
state->name_lower##_stack.auto_pop = 0;\
}\
return popped->v;\

#define UI_StackSetNextImpl(state, name_upper, name_lower, type, new_value) \
UI_##name_upper##Node *node = state->name_lower##_stack.free;\
if(node != 0) {StackPop(state->name_lower##_stack.free);}\
else {node = PushArray(UI_FrameArena(), UI_##name_upper##Node, 1);}\
type old_value = state->name_lower##_stack.top->v;\
node->v = new_value;\
StackPush(state->name_lower##_stack.top, node);\
state->name_lower##_stack.auto_pop = 1;\
return old_value;

#include "generated/ui.meta.c"
