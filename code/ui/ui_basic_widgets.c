////////////////////////////////
//~ rjf: Basics

root_function UI_Signal
UI_Label(String8 string)
{
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawText, Str8Lit(""));
 UI_BoxEquipDisplayString(box, string);
 UI_Signal result = UI_SignalFromBox(box);
 return result;
}

root_function UI_Signal
UI_LabelF(char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_Label(string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

root_function UI_Signal
UI_Image(R_Slice2F32 slice, String8 string)
{
 UI_SetNextSlice2F32(slice);
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBackground, string);
 UI_Signal result = UI_SignalFromBox(box);
 return result;
}

root_function UI_Signal
UI_ImageF(char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_Label(string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

root_function void
UI_Spacer(UI_Size size)
{
 UI_Box *parent = UI_TopParent();
 UI_SetNextPrefSize(parent->child_layout_axis, size);
 UI_SetNextPrefSize(Axis2_Flip(parent->child_layout_axis), UI_Pixels(0, 0));
 UI_Box *box = UI_BoxMake(0, Str8Lit(""));
 UnusedVariable(box);
}

root_function UI_Signal
UI_Button(String8 string)
{
 UI_SetNextHoverCursor(OS_CursorKind_Hand);
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBorder|
                          UI_BoxFlag_DrawBackground|
                          UI_BoxFlag_DrawText|
                          UI_BoxFlag_DrawHotEffects|
                          UI_BoxFlag_DrawActiveEffects|
                          UI_BoxFlag_Clickable,
                          string);
 UI_Signal result = UI_SignalFromBox(box);
 return result;
}

root_function UI_Signal
UI_ButtonF(char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_Button(string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

root_function UI_Signal
UI_Check(B32 checked, String8 string)
{
 UI_Size check_size = UI_Em(1.f, 1.f);
 UI_Size padding_size = UI_Em(0.4f, 1.f);
 UI_SetNextHoverCursor(OS_CursorKind_Hand);
 UI_SetNextChildLayoutAxis(Axis2_X);
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBorder|
                          UI_BoxFlag_DrawBackground|
                          UI_BoxFlag_DrawHotEffects|
                          UI_BoxFlag_DrawActiveEffects|
                          UI_BoxFlag_Clickable,
                          string);
 UI_Parent(box) UI_Padding(padding_size)
 {
  UI_PrefWidth(UI_SizeByChildren(1)) UI_Column UI_Padding(UI_Pct(1, 0))
  {
   UI_SetNextPrefWidth(check_size);
   UI_SetNextPrefHeight(check_size);
   UI_BoxFlags check_area_flags = UI_BoxFlag_DrawBackground|UI_BoxFlag_DisableTextTruncate;
   if(checked)
   {
    check_area_flags |= UI_BoxFlag_DrawText;
    UI_SetNextTextColor(UI_TopFillColor());
    UI_SetNextBackgroundColor(Mix4F32(UI_TopFillColor(), UI_TopBackgroundColor(), 0.6f));
    UI_SetNextFont(UI_IconFont());
    UI_SetNextTextAlign(UI_TextAlignment_Center);
   }
   UI_BoxMake(check_area_flags, ui_g_icon_kind_string_table[UI_IconKind_Check]);
  }
  UI_PrefWidth(UI_Pct(1, 0)) UI_Label(string);
 }
 UI_Signal result = UI_SignalFromBox(box);
 return result;
}

root_function UI_Signal
UI_CheckF(B32 checked, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_Check(checked, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

root_function UI_Signal
UI_Radio(B32 selected, String8 string)
{
 UI_Size check_size = UI_Em(1.f, 1.f);
 UI_Size padding_size = UI_Em(0.4f, 1.f);
 UI_SetNextHoverCursor(OS_CursorKind_Hand);
 UI_SetNextChildLayoutAxis(Axis2_X);
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBorder|
                          UI_BoxFlag_DrawBackground|
                          UI_BoxFlag_DrawHotEffects|
                          UI_BoxFlag_DrawActiveEffects|
                          UI_BoxFlag_Clickable,
                          string);
 UI_Parent(box) UI_Padding(padding_size)
 {
  UI_PrefWidth(UI_SizeByChildren(1)) UI_Column UI_Padding(UI_Pct(1, 0))
  {
   UI_SetNextPrefWidth(check_size);
   UI_SetNextPrefHeight(check_size);
   UI_SetNextCornerRadius(UI_TopFontSize() * 0.6f);
   UI_BoxFlags check_area_flags = UI_BoxFlag_DrawBackground;
   if(selected)
   {
    UI_SetNextBackgroundColor(UI_TopFillColor());
   }
   UI_Box *check_area = UI_BoxMake(check_area_flags, Str8Lit(""));
  }
  UI_PrefWidth(UI_Pct(1, 0)) UI_Label(string);
 }
 UI_Signal result = UI_SignalFromBox(box);
 return result;
}

root_function UI_Signal
UI_RadioF(B32 selected, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_Radio(selected, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

root_function UI_Signal
UI_Expander(B32 expanded, String8 string)
{
 UI_SetNextFont(UI_IconFont());
 UI_SetNextHoverCursor(OS_CursorKind_Hand);
 UI_SetNextTextAlign(UI_TextAlignment_Center);
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawText|UI_BoxFlag_Clickable|UI_BoxFlag_DisableTextTruncate, string);
 UI_BoxEquipDisplayString(box, ui_g_icon_kind_string_table[expanded ? UI_IconKind_DownCaret : UI_IconKind_RightCaret]);
 UI_Signal sig = UI_SignalFromBox(box);
 return sig;
}

root_function UI_Signal
UI_ExpanderF(B32 expanded, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_Expander(expanded, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

function UI_CUSTOM_DRAW_FUNCTION(UI_SliderF32Draw)
{
 UI_SliderF32DrawData *user = (UI_SliderF32DrawData *)box->ext_custom_draw->custom_draw_user_data;
 Rng2F32 rect = box->rect;
 Vec2F32 rect_size = Dim2F32(rect);
 rect.x1 = rect.x0 + rect_size.x * user->pct_filled;
 R_Rect2DInst *r = D_Rect2D(rect, .softness = 1, .color = user->fill_color);
 MemoryCopyArray(r->corner_radii, box->ext_rect_style->corner_radii);
}

root_function UI_Signal
UI_SliderF32(F32 *value, Rng1F32 range, String8 string)
{
 UI_SetNextHoverCursor(OS_CursorKind_WestEast);
 
 // rjf: param derivations
 F32 range_dim = Dim1F32(range);
 
 // rjf: build main interactive box
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBorder|
                          UI_BoxFlag_DrawText|
                          UI_BoxFlag_DrawHotEffects|
                          UI_BoxFlag_Clickable,
                          string);
 UI_Signal signal = UI_SignalFromBox(box);
 
 // rjf: interact
 {
  // rjf: handle dragging
  if(signal.dragging)
  {
   if(signal.pressed)
   {
    Vec2F32 drag_data = V2F32(*value, 0);
    UI_StoreDragDataStruct(&drag_data);
   }
   F32 initial_value = UI_LoadDragDataStruct(Vec2F32)->x;
   F32 delta = UI_DragDelta().x;
   F32 drag_factor_denom = Dim2F32(box->rect).x;
   drag_factor_denom = ClampBot(100, drag_factor_denom);
   F32 drag_factor = 1/drag_factor_denom * range_dim;
   *value = initial_value + delta * drag_factor;
  }
  
  // rjf: apply clamp
  *value = Clamp(range.min, *value, range.max);
 }
 
 // rjf: equip fill draw info
 {
  UI_SliderF32DrawData *draw_data = PushArray(UI_FrameArena(), UI_SliderF32DrawData, 1);
  draw_data->pct_filled = (*value - range.min) / range_dim;
  draw_data->fill_color = UI_TopFillColor();
  UI_BoxEquipCustomDrawFunction(box, UI_SliderF32Draw, draw_data);
 }
 
 return signal;
}

root_function UI_Signal
UI_SliderF32F(F32 *value, Rng1F32 range, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_SliderF32(value, range, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

root_function UI_Signal
UI_LineEdit(TxtPt *cursor, TxtPt *mark, U64 buffer_size, U8 *buffer, U64 *string_size, String8 string)
{
 Temp scratch = ScratchBegin(0, 0);
 F_Tag font_tag = UI_TopFont();
 F32 font_size = UI_TopFontSize();
 UI_Key key = UI_KeyFromString(UI_TopSeedKey(), string);
 B32 focus_active = UI_IsFocusActive();
 UI_TextAlignment align = UI_TopTextAlign();
 
 //- rjf: take text input events & mutate string
 if(focus_active)
 {
  UI_TxtActionList txt_actions = UI_TxtActionListFromEatenEvents(scratch.arena, UI_Window(), UI_Events(), 0);
  for(UI_TxtActionNode *n = txt_actions.first; n != 0; n = n->next)
  {
   UI_TxtAction *action = &n->action;
   String8 edit_string = Str8(buffer, *string_size);
   UI_TxtOp op = UI_SingleLineTxtOpFromTxtAction(scratch.arena, *cursor, *mark, edit_string, action);
   *cursor = op.cursor;
   *mark = op.mark;
   if(!TxtPtMatch(op.copy_range.min, op.copy_range.max))
   {
    OS_SetClipboardText(Substr8(edit_string, R1U64(op.copy_range.min.column-1,
                                                   op.copy_range.max.column-1)));
   }
   String8 modified_string = UI_PushStringReplaceRange(scratch.arena, edit_string,
                                                       R1U64(op.replaced_range.min.column-1,
                                                             op.replaced_range.max.column-1),
                                                       op.string);
   modified_string.size = ClampTop(modified_string.size, buffer_size);
   MemoryCopy(buffer, modified_string.str, modified_string.size);
   *string_size = modified_string.size;
   UI_ResetCursorBlinkT();
  }
 }
 
 //- rjf: grab edited edit string
 String8 edit_string = Str8(buffer, *string_size);
 
 //- rjf: build primary container box
 UI_SetNextHoverCursor(OS_CursorKind_IBar);
 UI_SetNextChildLayoutAxis(Axis2_X);
 UI_Box *box = UI_BoxMakeFromKey(UI_BoxFlag_MouseClickable|UI_BoxFlag_DrawText|UI_BoxFlag_DrawBorder|UI_BoxFlag_DrawBackground|UI_BoxFlag_Clip, key);
 
 //- rjf: build child box which visualizes string
 UI_Box *edit_string_box = &ui_g_nil_box;
 UI_Parent(box) UI_PrefWidth(UI_TextDim(1))
 {
  if(align == UI_TextAlignment_Center || align == UI_TextAlignment_Right)
  {
   UI_Spacer(UI_Pct(1, 0));
  }
  edit_string_box = UI_BoxMake(UI_BoxFlag_DrawText|UI_BoxFlag_DisableTextTruncate, Str8Lit("editstr"));
  UI_BoxEquipDisplayString(edit_string_box, edit_string);
  if(align == UI_TextAlignment_Center)
  {
   UI_Spacer(UI_Pct(1, 0));
  }
 }
 
 //- rjf: do interaction
 UI_Signal signal = UI_SignalFromBox(box);
 {
  if(signal.dragging)
  {
   Vec2F32 text_position = UI_TextPosFromBox(edit_string_box);
   F32 relative_mouse_x = UI_Mouse().x - text_position.x;
   U64 mouse_char_byte_off = F_ByteOffFromFontSizeStringAdvance(font_tag, font_size, edit_string, relative_mouse_x);
   TxtPt mouse_point = TxtPtMake(1, mouse_char_byte_off+1);
   UI_ResetCursorBlinkT();
   *cursor = mouse_point;
   
   // rjf: press => snap mark to cursor
   if(signal.pressed)
   {
    *mark = *cursor;
   }
  }
 }
 
 //- rjf: grab cursor & mark pixel offsets
 String8 string_before_mark = Prefix8(edit_string, mark->column-1);
 String8 string_before_cursor = Prefix8(edit_string, cursor->column-1);
 F32 advance_before_cursor = 0.f;
 F32 advance_before_mark = 0.f;
 {
  advance_before_mark = F_AdvanceFromFontSizeString(font_tag, font_size, string_before_mark);
  advance_before_cursor = F_AdvanceFromFontSizeString(font_tag, font_size, string_before_cursor);
 }
 
 //- rjf: force container box to contain the cursor
 if(focus_active && Dim2F32(box->rect).x != 0)
 {
  Rng1F32 cursor_visibility_range = R1F32(advance_before_cursor - font_size, advance_before_cursor + font_size);
  cursor_visibility_range.min = ClampBot(0, cursor_visibility_range.min);
  Rng1F32 box_visibility_range = R1F32(box->target_view_off.x, box->target_view_off.x + Dim2F32(box->rect).x);
  F32 delta_left = cursor_visibility_range.min - box_visibility_range.min;
  F32 delta_right = cursor_visibility_range.max - box_visibility_range.max;
  delta_left = ClampTop(delta_left, 0);
  delta_right = ClampBot(delta_right, 0);
  box->target_view_off.x += delta_left + delta_right;
 }
 else
 {
  box->target_view_off.x = 0;
 }
 
 //- rjf: extra rendering
 {
  D_Bucket *bucket = D_BucketMake(UI_FrameArena());
  D_BucketScope(UI_FrameArena(), bucket)
  {
   Vec2F32 text_p = UI_TextPosFromBox(edit_string_box);
   
   // rjf: draw selection
   if(focus_active)
   {
    UI_CursorVizData cursor_viz_data = UI_GetCursorVizData();
    Rng1F32 advance_rng = R1F32(advance_before_mark, advance_before_cursor);
    if(TxtPtLessThan(*cursor, *mark))
    {
     advance_rng.min = ClampBot(advance_rng.min, cursor_viz_data.p.x);
    }
    else
    {
     advance_rng.max = ClampTop(advance_rng.max, cursor_viz_data.p.x);
    }
    Rng2F32 advance_rect = R2F32(V2F32(text_p.x + advance_rng.min + 0.f, edit_string_box->rect.y0 + font_size/4.f),
                                 V2F32(text_p.x + advance_rng.max + 1.f, edit_string_box->rect.y1 - font_size/4.f));
    D_Rect2D(advance_rect,
             .color = UI_TopFillColor(),
             .corner = 2.f,
             .softness = 1.f);
   }
   
   // rjf: set cursor viz data
   {
    UI_SetCursorViz(edit_string_box->key, focus_active, V2F32(advance_before_cursor+2.f, font_size/4.f), Dim2F32(box->rect).y + font_size*2.f, font_size*2);
   }
  }
  UI_BoxEquipDrawBucket(edit_string_box, bucket, 1, 0, 0, 0);
 }
 
 ScratchEnd(scratch);
 return signal;
}

root_function UI_Signal
UI_LineEditF(TxtPt *cursor, TxtPt *mark, U64 buffer_size, U8 *buffer, U64 *string_size, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_LineEdit(cursor, mark, buffer_size, buffer, string_size, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

typedef struct UI_SatValPickerDrawUserData UI_SatValPickerDrawUserData;
struct UI_SatValPickerDrawUserData
{
 F32 font_size;
 F32 hue;
 F32 sat;
 F32 val;
};

function UI_CUSTOM_DRAW_FUNCTION(UI_SatValPickerDraw)
{
 UI_SatValPickerDrawUserData *user = (UI_SatValPickerDrawUserData *)box->ext_custom_draw->custom_draw_user_data;
 Vec3F32 full_rgb = RGBFromHSV(V3F32(user->hue, 1, 1));
 D_Rect2D(box->rect, .color = V4F32(0, 0, 0, 1));
 
 // rjf: value portion
 R_Rect2DInst *val_rect = D_Rect2D(box->rect, .softness = 1.f);
 val_rect->colors[Corner_00] = val_rect->colors[Corner_10] = V4F32(1, 1, 1, 1);
 val_rect->colors[Corner_01] = val_rect->colors[Corner_11] = V4F32(0, 0, 0, 1);
 
 // rjf: sat portion
 R_Rect2DInst *sat_rect = D_Rect2D(box->rect, .softness = 1.f);
 sat_rect->colors[Corner_00] = V4F32(full_rgb.r, full_rgb.g, full_rgb.b, 0);
 sat_rect->colors[Corner_01] = V4F32(0, 0, 0, 0);
 sat_rect->colors[Corner_10] = V4F32(full_rgb.r, full_rgb.g, full_rgb.b, 1);
 sat_rect->colors[Corner_11] = V4F32(full_rgb.r, full_rgb.g, full_rgb.b, 0);
 
 // rjf: visualize current selection
 {
  Vec2F32 box_dim = Dim2F32(box->rect);
  Vec2F32 center =
  {
   box->rect.x0 + box_dim.x * user->sat,
   box->rect.y0 + box_dim.y * (1 - user->val),
  };
  Vec2F32 indicator_half_size =
  {
   user->font_size*(0.8f + box->active_t*0.2f),
   user->font_size*(0.8f + box->active_t*0.2f)
  };
  Rng2F32 rect = R2F32(Sub2F32(center, indicator_half_size), Add2F32(center, indicator_half_size));
  R_Rect2DInst *r = D_Rect2D(rect, .color = V4F32(1, 1, 1, 1), .corner = indicator_half_size.x, .softness = 1.f, .border = user->font_size*0.2f);
 }
}

root_function void
UI_ColorPickerTooltipInfo(Vec3F32 hsv)
{
 Vec3F32 rgb = RGBFromHSV(hsv);
 UI_SetNextPrefWidth(UI_Em(20.f, 1.f));
 UI_PrefHeight(UI_SizeByChildren(1)) UI_Column UI_WidthFill 
 {
  UI_Spacer(UI_Em(0.5f, 1.f));
  UI_Row UI_Padding(UI_Pct(1, 0))
  {
   UI_SetNextBackgroundColor(V4F32(rgb.r, rgb.g, rgb.b, 1));
   UI_SetNextPrefWidth(UI_Em(3.f, 1.f));
   UI_SetNextPrefHeight(UI_Em(3.f, 1.f));
   UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBorder|UI_BoxFlag_DrawBackground, Str8Lit(""));
   UnusedVariable(box);
  }
  UI_Spacer(UI_Em(0.5f, 1.f));
  UI_Row UI_Padding(UI_Em(0.5f, 1.f))
  {
   UI_WidthFill UI_Column UI_PrefHeight(UI_Em(1.5f, 1.f)) UI_TextAlign(UI_TextAlignment_Left)
   {
    UI_LabelF("Red: %.2f", rgb.r);
    UI_LabelF("Green: %.2f", rgb.g);
    UI_LabelF("Blue: %.2f", rgb.b);
   }
   UI_WidthFill UI_Column UI_PrefHeight(UI_Em(1.5f, 1.f)) UI_TextAlign(UI_TextAlignment_Right)
   {
    UI_LabelF("Hue: %.2f", hsv.x);
    UI_LabelF("Sat: %.2f", hsv.y);
    UI_LabelF("Val: %.2f", hsv.z);
   }
  }
  UI_Spacer(UI_Em(0.5f, 1.f));
 }
}

root_function UI_Signal
UI_SatValPicker(F32 hue, F32 *out_sat, F32 *out_val, String8 string)
{
 UI_SetNextHoverCursor(OS_CursorKind_Hand);
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBorder|UI_BoxFlag_Clip|UI_BoxFlag_Clickable, string);
 UI_Signal sig = UI_SignalFromBox(box);
 UI_SatValPickerDrawUserData *user_data = PushArray(UI_FrameArena(), UI_SatValPickerDrawUserData, 1);
 UI_BoxEquipCustomDrawFunction(box, UI_SatValPickerDraw, user_data);
 
 // rjf: handle keyboard usage
 if(UI_KeyboardFocusFromRootKey(box->key))
 {
  // rjf: handle keyboard deactivation
  if(sig.keyboard_pressed)
  {
   UI_PopKeyboardBranch();
  }
  
  // rjf: arrowkeys => change selection
  OS_EventList *events = UI_Events();
  OS_Handle window = UI_Window();
  Vec2F32 delta = {0};
  for(OS_Event *event = events->first, *next = 0; event != 0; event = next)
  {
   next = event->next;
   if(!OS_HandleMatch(event->window, window))
   {
    continue;
   }
   if(event->kind == OS_EventKind_Press)
   {
    B32 taken = 1;
    switch(event->key)
    {
     default:{taken = 0;}break;
     case OS_Key_Right:{delta.x += 1.f/50.f;}break;
     case OS_Key_Left: {delta.x -= 1.f/50.f;}break;
     case OS_Key_Up:   {delta.y -= 1.f/50.f;}break;
     case OS_Key_Down: {delta.y += 1.f/50.f;}break;
    }
    if(taken)
    {
     OS_EatEvent(events, event);
    }
   }
  }
  *out_sat += delta.x;
  *out_val -= delta.y;
  *out_sat = Clamp(0, *out_sat, 1);
  *out_val = Clamp(0, *out_val, 1);
 }
 
 // rjf: handle keyboard deactivation
 else if(sig.keyboard_pressed)
 {
  UI_PushKeyboardBranch(box->key);
 }
 
 // rjf: handle dragging
 if(sig.dragging)
 {
  Vec2F32 box_dim = Dim2F32(box->rect);
  Vec2F32 mouse = UI_Mouse();
  F32 new_sat = (mouse.x - box->rect.x0) / box_dim.x;
  F32 new_val = 1 - (mouse.y - box->rect.y0) / box_dim.y;
  new_sat = Clamp(0, new_sat, 1);
  new_val = Clamp(0, new_val, 1);
  *out_sat = new_sat;
  *out_val = new_val;
  UI_Tooltip UI_ColorPickerTooltipInfo(V3F32(hue, new_sat, new_val));
 }
 
 // rjf: fill user data for draw
 user_data->font_size = UI_TopFontSize();
 user_data->hue = hue;
 user_data->sat = *out_sat;
 user_data->val = *out_val;
 
 return sig;
}

root_function UI_Signal
UI_SatValPickerF(F32 hue, F32 *out_sat, F32 *out_val, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_SatValPicker(hue, out_sat, out_val, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

typedef struct UI_HuePickerDrawUserData UI_HuePickerDrawUserData;
struct UI_HuePickerDrawUserData
{
 F32 hue;
};

function UI_CUSTOM_DRAW_FUNCTION(UI_HuePickerDraw)
{
 UI_HuePickerDrawUserData *user = box->ext_custom_draw->custom_draw_user_data;
 
 // rjf: draw fill
 {
  F32 step = 1.f / 6.f;
  Vec3F32 segments[] =
  {
   RGBFromHSV(V3F32(0*step, 1, 1)),
   RGBFromHSV(V3F32(1*step, 1, 1)),
   RGBFromHSV(V3F32(2*step, 1, 1)),
   RGBFromHSV(V3F32(3*step, 1, 1)),
   RGBFromHSV(V3F32(4*step, 1, 1)),
   RGBFromHSV(V3F32(5*step, 1, 1)),
   RGBFromHSV(V3F32(6*step, 1, 1)),
  };
  Rng2F32 rect = R2F32(V2F32(box->rect.x0, box->rect.y0),
                       V2F32(box->rect.x0 + Dim2F32(box->rect).x * step, box->rect.y1));
  F32 advance = Dim2F32(rect).x;
  for(U64 idx = 0; idx < ArrayCount(segments)-1; idx += 1)
  {
   Vec4F32 c0 = V4F32(segments[idx+0].r, segments[idx+0].g, segments[idx+0].b, 1);
   Vec4F32 c1 = V4F32(segments[idx+1].r, segments[idx+1].g, segments[idx+1].b, 1);
   R_Rect2DInst *r = D_Rect2D(rect);
   r->colors[Corner_00] = r->colors[Corner_01] = c0;
   r->colors[Corner_10] = r->colors[Corner_11] = c1;
   rect.x0 += advance;
   rect.x1 += advance;
  }
 }
 
 // rjf: draw indicator
 {
  F32 indicator_off = user->hue * Dim2F32(box->rect).x;
  Rng2F32 rect = R2F32(V2F32(box->rect.x0 - 2.f + indicator_off, box->rect.y0),
                       V2F32(box->rect.x0 + 2.f + indicator_off, box->rect.y1));
  D_Rect2D(rect, .color = V4F32(1, 1, 1, 1), .softness = 1.f);
 }
}

root_function UI_Signal
UI_HuePicker(F32 *out_hue, F32 sat, F32 val, String8 string)
{
 // rjf: build
 UI_SetNextHoverCursor(OS_CursorKind_Hand);
 UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBorder|UI_BoxFlag_Clickable|UI_BoxFlag_Clip, string);
 UI_Signal sig = UI_SignalFromBox(box);
 UI_HuePickerDrawUserData *user_data = PushArray(UI_FrameArena(), UI_HuePickerDrawUserData, 1);
 UI_BoxEquipCustomDrawFunction(box, UI_HuePickerDraw, user_data);
 
 // rjf: handle keyboard usage
 if(UI_KeyboardFocusFromRootKey(box->key))
 {
  // rjf: keyboard press => deactivate
  if(sig.keyboard_pressed)
  {
   UI_PopKeyboardBranch();
  }
  
  // rjf: left/right => change value
  OS_EventList *events = UI_Events();
  OS_Handle window = UI_Window();
  F32 delta = 0;
  for(OS_Event *event = events->first, *next = 0; event != 0; event = next)
  {
   next = event->next;
   if(!OS_HandleMatch(event->window, window))
   {
    continue;
   }
   if(event->kind == OS_EventKind_Press)
   {
    B32 taken = 1;
    switch(event->key)
    {
     default:{taken = 0;}break;
     case OS_Key_Right:{delta += 1.f/50.f;}break;
     case OS_Key_Left: {delta -= 1.f/50.f;}break;
    }
    if(taken)
    {
     OS_EatEvent(events, event);
    }
   }
  }
  *out_hue += delta;
  *out_hue = Clamp(0, *out_hue, 1);
 }
 
 // rjf: keyboard press => activate
 else if(sig.keyboard_pressed)
 {
  UI_PushKeyboardBranch(box->key);
 }
 
 // rjf: interaction
 if(sig.dragging)
 {
  Vec2F32 mouse = UI_Mouse();
  *out_hue = (mouse.x - box->rect.x0) / Dim2F32(box->rect).x;
  *out_hue = Clamp(0, *out_hue, 1);
  UI_Tooltip UI_ColorPickerTooltipInfo(V3F32(*out_hue, sat, val));
 }
 
 // rjf: fill draw user data
 user_data->hue = *out_hue;
 
 return sig;
}

root_function UI_Signal
UI_HuePickerF(F32 *out_hue, F32 sat, F32 val, char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_Signal result = UI_HuePicker(out_hue, sat, val, string);
 va_end(args);
 ScratchEnd(scratch);
 return result;
}

////////////////////////////////
//~ rjf: Layout Parents

root_function void
UI_NamedColumnBegin(String8 string)
{
 UI_SetNextChildLayoutAxis(Axis2_Y);
 UI_Box *box = UI_BoxMake(0, string);
 UI_PushParent(box);
}

root_function void
UI_NamedColumnBeginF(char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_NamedColumnBegin(string);
 va_end(args);
 ScratchEnd(scratch);
}

root_function void
UI_ColumnBegin(void)
{
 UI_NamedColumnBegin(Str8Lit(""));
}

root_function void
UI_ColumnEnd(void)
{
 UI_PopParent();
}

root_function void
UI_NamedRowBegin(String8 string)
{
 UI_SetNextChildLayoutAxis(Axis2_X);
 UI_Box *box = UI_BoxMake(0, string);
 UI_PushParent(box);
}

root_function void
UI_NamedRowBeginF(char *fmt, ...)
{
 Temp scratch = ScratchBegin(0, 0);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 UI_NamedRowBegin(string);
 va_end(args);
 ScratchEnd(scratch);
}

root_function void
UI_RowBegin(void)
{
 UI_NamedRowBegin(Str8Lit(""));
}

root_function void
UI_RowEnd(void)
{
 UI_PopParent();
}
