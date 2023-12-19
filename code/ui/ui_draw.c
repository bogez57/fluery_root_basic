root_function void
UI_Draw(void)
{
 OS_Handle window = UI_Window();
 F32 dpi = OS_DPIFromWindow(window);
 UI_CursorVizData cursor_viz_data = UI_GetCursorVizData();
 for(UI_Box *box = UI_Root(), *next_box = &ui_g_nil_box; !UI_BoxIsNil(box); box = next_box)
 {
  UI_BoxRec rec = UI_BoxRecurseDepthFirstPost(box, &ui_g_nil_box);
  next_box = rec.next;
  
  //- rjf: push opacity
  if(box->opacity != 1.f)
  {
   D_PushTransparency(1.f-box->opacity);
  }
  
  //- rjf: draw drop shadow
  if(box->flags & UI_BoxFlag_DrawDropShadow)
  {
   F32 shift = dpi * 0.03f;
   Rng2F32 shadow_rect = Pad2F32(Shift2F32(box->rect, V2F32(shift, shift)), shift*2.f);
   R_Rect2DInst *rect = D_Rect2D(shadow_rect,
                                 .color = V4F32(0, 0, 0, 0.8f),
                                 .corner = dpi*0.02f,
                                 .softness = 6.f);
  }
  
  //- rjf: draw background
  if(box->flags & UI_BoxFlag_DrawBackground)
  {
   if(!R_HandleMatch(box->ext_rect_style->slice.texture, R_HandleZero()))
   {
    D_PushTex2DSampleKind(R_Tex2DSampleKind_Linear);
   }
   R_Rect2DInst *rect = D_Rect2D(Pad2F32(box->rect, 1), .slice = box->ext_rect_style->slice);
   if(!R_HandleMatch(box->ext_rect_style->slice.texture, R_HandleZero()))
   {
    D_PopTex2DSampleKind();
   }
   rect->colors[Corner_00] = rect->colors[Corner_01] = rect->colors[Corner_10] = rect->colors[Corner_11] = box->ext_rect_style->background_color;
   MemoryCopyArray(rect->corner_radii, box->ext_rect_style->corner_radii);
   rect->softness = 1.f;
   
   if(box->flags & UI_BoxFlag_DrawHotEffects)
   {
    F32 active_destroyer_f = (box->flags & UI_BoxFlag_DrawActiveEffects) ? box->active_t : 0;
    F32 effective_hot_t = box->hot_t * (1-active_destroyer_f);
    F32 edge_thickness = dpi*0.1f;
    Rng2F32 left_edge = R2F32(V2F32(box->rect.x0, box->rect.y0),
                              V2F32(box->rect.x0 + edge_thickness, box->rect.y1));
    Rng2F32 right_edge = R2F32(V2F32(box->rect.x1 - edge_thickness, box->rect.y0),
                               V2F32(box->rect.x1, box->rect.y1));
    
    R_Rect2DInst *h1 = D_Rect2D(box->rect, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h1->colors[Corner_00] = h1->colors[Corner_10] = V4(1, 1, 1, 0.2f*effective_hot_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h1->corner_radii, box->ext_rect_style->corner_radii);
    
    R_Rect2DInst *h2 = D_Rect2D(left_edge, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h2->colors[Corner_00] = h2->colors[Corner_01] = V4(1, 1, 1, 0.1f*effective_hot_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h2->corner_radii, box->ext_rect_style->corner_radii);
    
    R_Rect2DInst *h3 = D_Rect2D(right_edge, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h3->colors[Corner_10] = h3->colors[Corner_11] = V4(1, 1, 1, 0.1f*effective_hot_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h3->corner_radii, box->ext_rect_style->corner_radii);
    
    R_Rect2DInst *h4 = D_Rect2D(box->rect, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h4->colors[Corner_01] = h4->colors[Corner_11] = V4(0, 0, 0, 0.2f*effective_hot_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h4->corner_radii, box->ext_rect_style->corner_radii);
   }
   
   if(box->flags & UI_BoxFlag_DrawActiveEffects)
   {
    F32 edge_thickness = dpi*0.3f;
    Rng2F32 left_edge = R2F32(V2F32(box->rect.x0, box->rect.y0),
                              V2F32(box->rect.x0 + edge_thickness, box->rect.y1));
    Rng2F32 right_edge = R2F32(V2F32(box->rect.x1 - edge_thickness, box->rect.y0),
                               V2F32(box->rect.x1, box->rect.y1));
    
    R_Rect2DInst *h1 = D_Rect2D(box->rect, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h1->colors[Corner_00] = h1->colors[Corner_10] = V4(0, 0, 0, 0.5f*box->active_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h1->corner_radii, box->ext_rect_style->corner_radii);
    
    R_Rect2DInst *h2 = D_Rect2D(left_edge, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h2->colors[Corner_00] = h2->colors[Corner_01] = V4(0, 0, 0, 0.5f*box->active_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h2->corner_radii, box->ext_rect_style->corner_radii);
    
    R_Rect2DInst *h3 = D_Rect2D(right_edge, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h3->colors[Corner_10] = h3->colors[Corner_11] = V4(0, 0, 0, 0.5f*box->active_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h3->corner_radii, box->ext_rect_style->corner_radii);
    
    R_Rect2DInst *h4 = D_Rect2D(box->rect, .color = V4(0, 0, 0, 0), .softness = 1.f);
    h1->colors[Corner_01] = h1->colors[Corner_11] = V4(1, 1, 1, 0.08f*box->active_t * box->ext_rect_style->background_color.a);
    MemoryCopyArray(h4->corner_radii, box->ext_rect_style->corner_radii);
   }
   
   if(box->focus_hot_t >= 0.005f)
   {
    Rng2F32 rect = box->rect;
    Vec2F32 dim = Dim2F32(rect);
    rect.x1 = rect.x0 + dim.x*box->focus_hot_t;
    R_Rect2DInst *r = D_Rect2D(rect, .color = V4(1, 0.94f, 0.7f, 0.2f*box->focus_hot_t), .softness = 1.f);
    MemoryCopyArray(r->corner_radii, box->ext_rect_style->corner_radii);
   }
  }
  
  //- rjf: draw text
  if(box->flags & UI_BoxFlag_DrawText)
  {
   Arena *draw_arena = D_ActiveArena();
   Temp scratch = ScratchBegin(&draw_arena, 1);
   Vec2F32 text_pos = UI_TextPosFromBox(box);
   String8 display_string = UI_DisplayStringFromBox(box);
   String8 trailer = Str8Lit("...");
   B32 truncated = 0;
   if(!(box->flags & UI_BoxFlag_DisableTextTruncate))
   {
    F32 trailer_advance = F_AdvanceFromFontSizeString(box->ext_text->font_tag, box->ext_text->font_size, trailer);
    F32 max_size_for_string = box->calc_size.x - box->ext_text->text_edge_padding*2;
    if(trailer_advance > max_size_for_string)
    {
     display_string = Str8Lit("");
    }
    else
    {
     String8 truncated_string = F_TruncatedStringFromFontSizeStringMax(box->ext_text->font_tag,
                                                                       box->ext_text->font_size,
                                                                       display_string,
                                                                       max_size_for_string,
                                                                       trailer_advance);
     truncated = (truncated_string.size < display_string.size);
     display_string = truncated_string;
    }
   }
   if(box->flags & UI_BoxFlag_DrawActiveEffects)
   {
    text_pos.y += 0.1f * box->ext_text->font_size * box->active_t;
   }
   F32 off = D_Text2D(text_pos, box->ext_text->font_tag, box->ext_text->font_size, box->ext_text->text_color, display_string);
   if(truncated)
   {
    D_Text2D(V2(text_pos.x + off, text_pos.y), box->ext_text->font_tag, box->ext_text->font_size, box->ext_text->text_color, trailer);
   }
   ScratchEnd(scratch);
  }
  
  //- rjf: draw cursor information
  if(cursor_viz_data.is_active && UI_KeyMatch(box->key, cursor_viz_data.key))
  {
   Vec4F32 color = UI_TopCursorColor();
   F32 cursor_blink_t = UI_CursorBlinkT();
   F32 cursor_thickness = UI_TopFontSize()*0.5f;
   cursor_thickness = ClampBot(cursor_thickness, 3.f);
   color.alpha = color.alpha * (0.6f + Cos(0.5f * cursor_blink_t / OS_CaretBlinkTime())*0.4f);
   
   F32 velocity_factor = cursor_viz_data.velocity * 0.8f;
   F32 min_velocity_factor = ClampTop(0, velocity_factor);
   F32 max_velocity_factor = ClampBot(0, velocity_factor);
   
   Vec2F32 box_text_pos = UI_TextPosFromBox(box);
   Rng2F32 rect = {0};
   {
    rect.x0 = box_text_pos.x + cursor_viz_data.p.x - cursor_thickness/2 + min_velocity_factor;
    rect.y0 = box->rect.y0 + box->ext_text->font_size/4.f - AbsoluteValueF32(velocity_factor)*box->ext_text->font_size*0.035f;
    rect.x1 = box_text_pos.x + cursor_viz_data.p.x + cursor_thickness/2 + max_velocity_factor;
    rect.y1 = box->rect.y1 - box->ext_text->font_size/4.f + AbsoluteValueF32(velocity_factor)*box->ext_text->font_size*0.035f;
   };
   D_Rect2D(rect, .color = color, .corner = 1.f, .softness = 1.f);
  }
  
  //- rjf: draw border
  if(box->flags & UI_BoxFlag_DrawBorder)
  {
   Rng2F32 border_rect = Pad2F32(box->rect, 1);
   R_Rect2DInst *rect = D_Rect2D(border_rect);
   rect->colors[Corner_00] = rect->colors[Corner_01] = rect->colors[Corner_10] = rect->colors[Corner_11] = box->ext_rect_style->border_color;
   MemoryCopyArray(rect->corner_radii, box->ext_rect_style->corner_radii);
   rect->border_thickness = box->ext_rect_style->border_thickness;
   rect->softness = 1.f;
   if(box->flags & UI_BoxFlag_DrawHotEffects)
   {
    R_Rect2DInst *rect = D_Rect2D(border_rect);
    rect->colors[Corner_00] = rect->colors[Corner_01] = rect->colors[Corner_10] = rect->colors[Corner_11] = V4(1, 1, 1, 1.f * box->hot_t * box->ext_rect_style->border_color.a);
    MemoryCopyArray(rect->corner_radii, box->ext_rect_style->corner_radii);
    rect->border_thickness = 2.f;
    rect->softness = 1.f;
   }
  }
  
  //- rjf: push clip if necessary
  if(box->flags & UI_BoxFlag_Clip)
  {
   Rng2F32 top_clip = D_TopClip();
   Vec2F32 top_clip_dim = Dim2F32(top_clip);
   Rng2F32 new_clip = box->rect;
   if(top_clip_dim.x != 0 || top_clip_dim.y != 0)
   {
    new_clip = Intersection2F32(new_clip, top_clip);
   }
   D_PushClip(new_clip);
  }
  
  //- rjf: draw custom rendering bucket
  if(box->flags & UI_BoxFlag_DrawBucket)
  {
   F32 anchor_weight_sum = box->ext_bucket->bucket_anchor_weights[Corner_00] + box->ext_bucket->bucket_anchor_weights[Corner_01] + box->ext_bucket->bucket_anchor_weights[Corner_10] + box->ext_bucket->bucket_anchor_weights[Corner_11];
   Vec2F32 adjustment_delta =
   {
    (box->ext_bucket->bucket_anchor_weights[Corner_00] * box->rel_corner_delta[Corner_00].x +
     box->ext_bucket->bucket_anchor_weights[Corner_01] * box->rel_corner_delta[Corner_01].x +
     box->ext_bucket->bucket_anchor_weights[Corner_10] * box->rel_corner_delta[Corner_10].x +
     box->ext_bucket->bucket_anchor_weights[Corner_11] * box->rel_corner_delta[Corner_11].x) / anchor_weight_sum,
    (box->ext_bucket->bucket_anchor_weights[Corner_00] * box->rel_corner_delta[Corner_00].y +
     box->ext_bucket->bucket_anchor_weights[Corner_01] * box->rel_corner_delta[Corner_01].y +
     box->ext_bucket->bucket_anchor_weights[Corner_10] * box->rel_corner_delta[Corner_10].y +
     box->ext_bucket->bucket_anchor_weights[Corner_11] * box->rel_corner_delta[Corner_11].y) / anchor_weight_sum
   };
   D_Transform2D(MakeTranslate3x3F32(adjustment_delta))
    D_BucketConcatInPlace(box->ext_bucket->bucket);
  }
  
  //- rjf: draw with custom callback
  if(box->flags & UI_BoxFlag_DrawCustomFunction)
  {
   box->ext_custom_draw->CustomDraw(box);
  }
  
  //- rjf: pop clips
  if(rec.push_count == 0)
  {
   int pop_idx = 0;
   for(UI_Box *p = box;
       !UI_BoxIsNil(p) && p != next_box && pop_idx <= rec.pop_count;
       p = p->parent, pop_idx += 1)
   {
    if(p->flags & UI_BoxFlag_Clip)
    {
     D_PopClip();
    }
    
    // rjf: draw disabled overlay
    if(p->disabled_t > 0.01f)
    {
     R_Rect2DInst *rect = D_Rect2D(p->rect);
     MemoryCopyArray(rect->corner_radii, p->ext_rect_style->corner_radii);
     rect->softness = 1.f;
     rect->colors[Corner_00] =
      rect->colors[Corner_01] =
      rect->colors[Corner_10] =
      rect->colors[Corner_11] =
      V4F32(0, 0, 0, 0.6f*p->disabled_t);
    }
    
    // rjf: draw focus indicator
#if 0
    if(p->flags & UI_BoxFlag_Focus)
    {
     UI_FocusVizData kb_viz = UI_GetFocusVizData();
     
     // rjf: top border
     {
      R_Rect2DInst *rect = D_Rect2D(kb_viz.rect);
      rect->colors[Corner_00] = rect->colors[Corner_10] = V4(1, 0.4f, 0.2f, 1.f*kb_viz.alpha);
      rect->colors[Corner_01] = rect->colors[Corner_11] = V4(1, 0.4f, 0.2f, 0.2f*kb_viz.alpha);
      MemoryCopyArray(rect->corner_radii, p->ext_rect_style->corner_radii);
      rect->border_thickness = 2.f;
      rect->softness = 1.f;
     }
     
     // rjf: fill
     {
      R_Rect2DInst *rect = D_Rect2D(kb_viz.rect);
      rect->colors[Corner_00] = rect->colors[Corner_10] = V4(1, 0.4f, 0.2f, 0.1f*kb_viz.alpha);
      rect->colors[Corner_01] = rect->colors[Corner_11] = V4(1, 0.4f, 0.2f, 0.05f*kb_viz.alpha);
      MemoryCopyArray(rect->corner_radii, p->ext_rect_style->corner_radii);
      rect->softness = 1.f;
     }
    }
#endif
    
    // rjf: pop opacity
    if(p->opacity != 1.f)
    {
     D_PopTransparency();
    }
   }
  }
  
 }
}
