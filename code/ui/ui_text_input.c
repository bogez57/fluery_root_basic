
root_function B32
UI_ASCIICharIsBoundary(U8 c)
{
 return CharIsSpace(c) || (CharToForwardSlash(c) == '/');
}

root_function void
UI_TxtActionListPush(Arena *arena, UI_TxtActionList *list, UI_TxtAction action)
{
 UI_TxtActionNode *n = PushArray(arena, UI_TxtActionNode, 1);
 n->action = action;
 QueuePush(list->first, list->last, n);
 list->count += 1;
}

root_function UI_TxtAction
UI_TxtActionFromEvent(OS_Event *event)
{
 UI_TxtAction action = {0};
 if(event->kind == OS_EventKind_Press || event->kind == OS_EventKind_Text)
 {
  // rjf: press => use default mapping to actions
  if(event->kind == OS_EventKind_Press)
  {
   if(event->modifiers & OS_Modifier_Ctrl)
   {
    action.flags |= UI_TxtActionFlag_WordScan;
   }
   if(event->modifiers & OS_Modifier_Shift)
   {
    action.flags |= UI_TxtActionFlag_KeepMark;
   }
   switch(event->key)
   {
    default: break;
    
    // rjf: navigation
    case OS_Key_Right:
    {
     action.flags |= UI_TxtActionFlag_Good;
     action.delta.x = +1;
     if((event->modifiers & (OS_Modifier_Ctrl|OS_Modifier_Shift)) == 0)
     {
      action.flags |= UI_TxtActionFlag_ZeroDeltaOnSelect|UI_TxtActionFlag_PickSideOnSelect;
     }
    }break;
    case OS_Key_Left:
    {
     action.flags |= UI_TxtActionFlag_Good;
     action.delta.x = -1;
     if((event->modifiers & (OS_Modifier_Ctrl|OS_Modifier_Shift)) == 0)
     {
      action.flags |= UI_TxtActionFlag_ZeroDeltaOnSelect|UI_TxtActionFlag_PickSideOnSelect;
     }
    }break;
    case OS_Key_Home:     {action.flags |= UI_TxtActionFlag_Good; action.delta.x = S64Min;}break;
    case OS_Key_End:      {action.flags |= UI_TxtActionFlag_Good; action.delta.x = S64Max;}break;
    
    // rjf: deletion
    case OS_Key_Delete:   {action.delta.x = +1; action.flags |= UI_TxtActionFlag_Good|UI_TxtActionFlag_Delete|UI_TxtActionFlag_ZeroDeltaOnSelect;}break;
    case OS_Key_Backspace:{action.delta.x = -1; action.flags |= UI_TxtActionFlag_Good|UI_TxtActionFlag_Delete|UI_TxtActionFlag_ZeroDeltaOnSelect;}break;
    
    // rjf: copy
    case OS_Key_C:
    if(event->modifiers & OS_Modifier_Ctrl)
    {
     action.flags |= UI_TxtActionFlag_Good|UI_TxtActionFlag_Copy|UI_TxtActionFlag_KeepMark;
    }break;
    
    // rjf: cut
    case OS_Key_X:
    if(event->modifiers & OS_Modifier_Ctrl)
    {
     action.flags |= UI_TxtActionFlag_Good|UI_TxtActionFlag_Copy|UI_TxtActionFlag_Delete;
    }break;
    
    // rjf: paste
    case OS_Key_V:
    if(event->modifiers & OS_Modifier_Ctrl)
    {
     action.flags |= UI_TxtActionFlag_Good|UI_TxtActionFlag_Paste;
    }break;
   }
  }
  
  // rjf: text => text insertion
  if(event->kind == OS_EventKind_Text)
  {
   action.flags |= UI_TxtActionFlag_Good;
   action.codepoint = event->character;
  }
 }
 return action;
}

root_function UI_TxtActionList
UI_TxtActionListFromEatenEvents(Arena *arena, OS_Handle window, OS_EventList *events, B32 multiline)
{
 UI_TxtActionList result = {0};
 
 // rjf: get txt actions
 for(OS_Event *event = events->first, *next = 0; event != 0; event = next)
 {
  next = event->next;
  if(!OS_HandleMatch(event->window, window))
  {
   continue;
  }
  
  // rjf: map event to text action
  UI_TxtAction action = UI_TxtActionFromEvent(event);
  B32 good_action = !!(action.flags & UI_TxtActionFlag_Good) && (multiline || action.delta.y == 0);
  if(multiline == 0 && action.codepoint == '\n')
  {
   good_action = 0;
  }
  
  // rjf: push to result
  if(good_action)
  {
   UI_TxtActionListPush(arena, &result, action);
  }
  
  // rjf: eat event
  if(good_action)
  {
   OS_EatEvent(events, event);
  }
 }
 
 // rjf: consume events with valid codepoints
 for(OS_Event *event = events->first, *next = 0; event != 0; event = next)
 {
  next = event->next;
  if(!OS_HandleMatch(event->window, window))
  {
   continue;
  }
  
  U64 character = OS_CharacterFromModifiersAndKey(event->modifiers, event->key);
  if(character != 0 && (multiline || character != '\n'))
  {
   OS_EatEvent(events, event);
  }
 }
 
 return result;
}

root_function UI_TxtOp
UI_SingleLineTxtOpFromTxtAction(Arena *arena, TxtPt cursor, TxtPt mark, String8 string, UI_TxtAction *action)
{
 UI_TxtOp op = {0};
 op.cursor = cursor;
 op.mark   = mark;
 
 //- rjf: high-level delta
 S64 delta = action->delta.x;
 
 //- rjf: zero delta on selection
 if(action->flags & UI_TxtActionFlag_ZeroDeltaOnSelect && !TxtPtMatch(cursor, mark))
 {
  delta = 0;
 }
 
 //- rjf: resolve high-level delta into byte delta
 S64 byte_delta = 0;
 {
  if(action->flags & UI_TxtActionFlag_WordScan)
  {
   S64 inc = (delta > 0) ? +1 : (delta < 0) ? -1 : 0;
   if(inc != 0)
   {
    S64 start_byte_off = cursor.column-1; 
    for(S64 byte_off = start_byte_off + inc;
        0 <= byte_off && byte_off <= string.size;
        byte_off += inc)
    {
     if(byte_off == 0 || byte_off == string.size)
     {
      byte_delta = byte_off - start_byte_off;
      break;
     }
     if(!UI_ASCIICharIsBoundary(string.str[byte_off]) && UI_ASCIICharIsBoundary(string.str[byte_off-1]))
     {
      byte_delta = byte_off - start_byte_off;
      break;
     }
    }
   }
  }
  else if(delta == S64Max || delta == S64Min)
  {
   byte_delta = delta;
  }
  else if(delta > 0)
  {
   TxtPt p = cursor;
   for(S64 moves_left = +delta; moves_left > 0; moves_left -= 1)
   {
    U64 p_byte_off = p.column-1;
    if(p_byte_off >= string.size)
    {
     break;
    }
    DecodedCodepoint decode = DecodeCodepointFromUtf8(string.str + p_byte_off, string.size - p_byte_off);
    byte_delta += decode.advance;
    p.column += decode.advance;
   }
   byte_delta = p.column - cursor.column;
  }
  else if(delta < 0)
  {
   TxtPt p = cursor;
   for(S64 moves_left = -delta; moves_left > 0; moves_left -= 1)
   {
    U64 p_byte_off = p.column-1;
    U64 p_last_cp_off = p_byte_off - ClampTop(4, p_byte_off);
    U64 p_prev_cp_off = p_byte_off;
    for(U64 decode_off = p_last_cp_off; decode_off < p_byte_off;)
    {
     DecodedCodepoint decode = DecodeCodepointFromUtf8(string.str + decode_off, string.size - decode_off);
     if(decode_off <= p_byte_off && p_byte_off < decode_off+decode.advance)
     {
      break;
     }
     p_prev_cp_off = decode_off;
     decode_off += decode.advance;
    }
    p.column = (S64)(p_prev_cp_off+1);
   }
   byte_delta = p.column - cursor.column;
  }
  
 }
 
 //- rjf: pick selection side
 if(action->flags & UI_TxtActionFlag_PickSideOnSelect && !TxtPtMatch(cursor, mark))
 {
  TxtRng selection_range = TxtRngMake(cursor, mark);
  TxtPt dst_pt = action->delta.x > 0 ? selection_range.max : selection_range.min;
  byte_delta = dst_pt.column - cursor.column;
 }
 
 //- rjf: apply byte delta to op's cursor
 {
  byte_delta = Clamp((-op.cursor.column)+1, byte_delta, (S64)string.size+1-op.cursor.column);
  op.cursor.column += byte_delta;
 }
 
 //- rjf: insertions
 {
  B32 inserting = 0;
  if(action->flags & UI_TxtActionFlag_Paste)
  {
   inserting = 1;
   op.string = OS_GetClipboardText(arena);
  }
  else if(action->codepoint != 0 && action->codepoint != '\n')
  {
   U32 codepoint = action->codepoint;
   String32 str32 = Str32(&codepoint, 1);
   String8 str8 = Str8From32(arena, str32);
   op.string = str8;
   inserting = 1;
  }
  if(inserting)
  {
   TxtRng selection_range = TxtRngMake(op.cursor, op.mark);
   TxtPt end_of_insertion_pt = TxtPtMake(1, selection_range.min.column + op.string.size);
   op.replaced_range = selection_range;
   op.cursor = op.mark = end_of_insertion_pt;
  }
 }
 
 //- rjf: apply copy
 if(action->flags & UI_TxtActionFlag_Copy)
 {
  TxtRng selection_range = TxtRngMake(op.cursor, op.mark);
  op.copy_range = selection_range;
 }
 
 //- rjf: apply deletion
 if(action->flags & UI_TxtActionFlag_Delete)
 {
  TxtRng selection_range = TxtRngMake(op.cursor, op.mark);
  op.mark = op.cursor = selection_range.min;
  op.replaced_range = selection_range;
 }
 
 //- rjf: apply keep-mark
 if(!(action->flags & UI_TxtActionFlag_KeepMark))
 {
  op.mark = op.cursor;
 }
 
 return op;
}

root_function String8
UI_PushStringReplaceRange(Arena *arena, String8 string, Rng1U64 range, String8 replace)
{
 String8 result = {0};
 range.min = ClampTop(range.min, string.size);
 range.max = ClampTop(range.max, string.size);
 range = R1U64(range.min, range.max);
 U64 result_size = string.size - Dim1U64(range) + replace.size;
 result.str = PushArray(arena, U8, result_size+1);
 result.size = result_size;
 String8 before_range = Prefix8(string, range.min);
 String8 after_range = Str8Skip(string, range.max);
 if(before_range.size != 0)
 {
  MemoryCopy(result.str, before_range.str, before_range.size);
 }
 if(replace.size != 0)
 {
  MemoryCopy(result.str + range.min, replace.str, replace.size);
 }
 if(after_range.size != 0)
 {
  MemoryCopy(result.str + range.min + replace.size, after_range.str, after_range.size);
 }
 return result;
}
