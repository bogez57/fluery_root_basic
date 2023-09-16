////////////////////////////////
//~ rjf: Token Type Functions

function void
CL_TokenListPush(Arena *arena, CL_TokenList *list, CL_Token *token)
{
 CL_TokenNode *n = PushArray(arena, CL_TokenNode, 1);
 MemoryCopyStruct(&n->v, token);
 QueuePush(list->first, list->last, n);
 list->count += 1;
}

function void
CL_TokenPtrListPush(Arena *arena, CL_TokenPtrList *list, CL_Token *token)
{
 CL_TokenPtrNode *n = PushArray(arena, CL_TokenPtrNode, 1);
 n->v = token;
 QueuePush(list->first, list->last, n);
 list->count += 1;
}

function void
CL_TokenPtrListListPush(Arena *arena, CL_TokenPtrListList *list, CL_TokenPtrList *v)
{
 CL_TokenPtrListNode *n = PushArray(arena, CL_TokenPtrListNode, 1);
 MemoryCopyStruct(&n->v, v);
 QueuePush(list->first, list->last, n);
 list->node_count += 1;
 list->total_count += v->count;
}

function void
CL_TokenChunkListPush(Arena *arena, CL_TokenChunkList *list, U64 cap, CL_Token *token)
{
 CL_TokenChunkNode *node = list->last;
 if(node == 0 || node->count >= node->cap)
 {
  node = PushArray(arena, CL_TokenChunkNode, 1);
  node->cap = cap;
  node->v = PushArrayNoZero(arena, CL_Token, node->cap);
  QueuePush(list->first, list->last, node);
  list->chunk_count += 1;
 }
 MemoryCopyStruct(&node->v[node->count], token);
 node->count += 1;
 list->token_count += 1;
}

function CL_TokenArray
CL_TokenArrayFromChunkList(Arena *arena, CL_TokenChunkList *list)
{
 CL_TokenArray array = {0};
 array.count = list->token_count;
 array.v = PushArrayNoZero(arena, CL_Token, array.count);
 U64 idx = 0;
 for(CL_TokenChunkNode *n = list->first; n != 0; n = n->next)
 {
  MemoryCopy(array.v+idx, n->v, sizeof(CL_Token)*n->count);
  idx += n->count;
 }
 return array;
}

////////////////////////////////
//~ rjf: Lexing Functions

function CL_TokenArray
CL_TokenArrayFromString(Arena *arena, String8 string)
{
 Temp scratch = ScratchBegin(&arena, 1);
 CL_TokenChunkList tokens = {0};
 {
  B32 comment_is_single_line = 0;
  CL_TokenKind active_token_kind = CL_TokenKind_Null;
  U64 active_token_start_off = 0;
  U64 off = 0;
  B32 escaped = 0;
  for(U64 advance = 0; off <= string.size; off += advance)
  {
   U8 byte      = (off+0 < string.size) ? string.str[off+0] : 0;
   U8 next_byte = (off+1 < string.size) ? string.str[off+1] : 0;
   B32 ender_found = 0;
   advance = (active_token_kind != CL_TokenKind_Null ? 1 : 0);
   if(off == string.size && active_token_kind != CL_TokenKind_Null)
   {
    ender_found = 1;
    advance = 1;
   }
   switch(active_token_kind)
   {
    default:
    case CL_TokenKind_Null:
    {
     if((byte == '\r' && next_byte == '\n') || byte == '\n')
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_Newline;
      advance = 0;
     }
     else if(byte == ' ' || byte == '\t' || byte == '\v' || byte == '\f')
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_Whitespace;
      advance = 1;
     }
     else if(('a' <= byte && byte <= 'z') || ('A' <= byte && byte <= 'Z') || byte == '_')
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_Identifier;
      advance = 1;
     }
     else if(byte == '\'')
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_CharLiteral;
      advance = 1;
     }
     else if(byte == '"')
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_StringLiteral;
      advance = 1;
     }
     else if(('0' <= byte && byte <= '9') || (byte == '.' && '0' <= next_byte && next_byte <= '9'))
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_NumericLiteral;
      advance = 1;
     }
     else if((byte == '/' && next_byte == '/') ||
             (byte == '/' && next_byte == '*'))
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_Comment;
      comment_is_single_line = (next_byte == '/');
      advance = 2;
     }
     else if(byte == '~' || byte == '!' || byte == '%' || byte == '^' ||
             byte == '&' || byte == '*' || byte == '(' || byte == ')' ||
             byte == '-' || byte == '=' || byte == '+' || byte == '[' ||
             byte == ']' || byte == '{' || byte == '}' || byte == ';' ||
             byte == ':' || byte == '?' || byte == '/' || byte == '<' ||
             byte == '>' || byte == ',' || byte == '.')
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_Symbol;
      advance = 1;
     }
     else if(byte == '#')
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_Preprocessor;
      advance = 1;
     }
     else
     {
      active_token_start_off = off;
      active_token_kind = CL_TokenKind_Error;
      advance = 1;
     }
    }break;
    case CL_TokenKind_Newline:
    {
     if(byte == '\r' && next_byte == '\n')
     {
      advance = 2;
      ender_found = 1;
     }
     else if(byte == '\n')
     {
      advance = 1;
      ender_found = 1;
     }
    }break;
    case CL_TokenKind_Whitespace:
    if(byte != ' ' && byte != '\t' && byte != '\v' && byte != '\f')
    {
     ender_found = 1;
     advance = 0;
    }break;
    case CL_TokenKind_Identifier:
    if((byte < 'a' || 'z' < byte) && (byte < 'A' || 'Z' < byte) && (byte < '0' || '9' < byte) && byte != '_')
    {
     ender_found = 1;
     advance = 0;
    }break;
    case CL_TokenKind_CharLiteral:
    {
     if(!escaped && byte == '\'')
     {
      ender_found = 1;
      advance = 1;
     }
     else if(escaped)
     {
      escaped = 0;
      advance = 1;
     }
     else if(byte == '\\')
     {
      escaped = 1;
      advance = 1;
     }
     else
     {
      U8 byte_class = utf8_class[byte>>3];
      if(byte_class > 1)
      {
       advance = (U64)byte_class;
      }
     }
    }break;
    case CL_TokenKind_StringLiteral:
    {
     if(!escaped && byte == '"')
     {
      ender_found = 1;
      advance = 1;
     }
     else if(escaped)
     {
      escaped = 0;
      advance = 1;
     }
     else if(byte == '\\')
     {
      escaped = 1;
      advance = 1;
     }
     else
     {
      U8 byte_class = utf8_class[byte>>3];
      if(byte_class > 1)
      {
       advance = (U64)byte_class;
      }
     }
    }break;
    case CL_TokenKind_NumericLiteral:
    if((byte < 'a' || 'z' < byte) && (byte < 'A' || 'Z' < byte) && (byte < '0' || '9' < byte) && byte != '.')
    {
     ender_found = 1;
     advance = 0;
    }break;
    case CL_TokenKind_Symbol:
    if(1)
    {
     // NOTE(rjf): avoiding maximum munch rule for now
     ender_found = 1;
     advance = 0;
    }
    else if(byte != '~' && byte != '!' && byte != '#' && byte != '%' &&
            byte != '^' && byte != '&' && byte != '*' && byte != '(' &&
            byte != ')' && byte != '-' && byte != '=' && byte != '+' &&
            byte != '[' && byte != ']' && byte != '{' && byte != '}' &&
            byte != ';' && byte != ':' && byte != '?' && byte != '/' &&
            byte != '<' && byte != '>' && byte != ',' && byte != '.')
    {
     ender_found = 1;
     advance = 0;
    }break;
    case CL_TokenKind_Preprocessor:
    {
     if(!escaped && (byte == '\n' || byte == '\r'))
     {
      ender_found = 1;
      advance = 0;
     }
     else if(escaped)
     {
      escaped = 0;
      advance = 1;
     }
     else if(byte == '\\')
     {
      escaped = 1;
      advance = 1;
     }
    }break;
    case CL_TokenKind_Comment:
    {
     if(!escaped && comment_is_single_line && (byte == '\n' || byte == '\r'))
     {
      ender_found = 1;
      advance = 0;
     }
     else if(!escaped && !comment_is_single_line && byte == '*' && next_byte == '/')
     {
      ender_found = 1;
      advance = 2;
     }
     else if(escaped)
     {
      escaped = 0;
      advance = 1;
     }
     else if(byte == '\\')
     {
      escaped = 1;
      advance = 1;
     }
     else
     {
      U8 byte_class = utf8_class[byte>>3];
      if(byte_class > 1)
      {
       advance = (U64)byte_class;
      }
     }
    }break;
    case CL_TokenKind_Error:
    {
     ender_found = 1;
     advance = 0;
    }break;
   }
   if(ender_found != 0)
   {
    CL_Token token = {active_token_kind, R1U64(active_token_start_off, off+advance)};
    CL_TokenChunkListPush(arena, &tokens, 1024, &token);
    active_token_kind = CL_TokenKind_Null;
    active_token_start_off = token.range.max;
   }
  }
 }
 CL_TokenArray result = CL_TokenArrayFromChunkList(arena, &tokens);
 ScratchEnd(scratch);
 return result;
}
