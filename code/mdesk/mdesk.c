////////////////////////////////
//~ rjf: Globals

#if BUILD_ROOT
MD_Node md_nil_node =
{
 &md_nil_node,
 &md_nil_node,
 &md_nil_node,
 &md_nil_node,
 &md_nil_node,
 &md_nil_node,
 &md_nil_node,
};
#endif

////////////////////////////////
//~ rjf: Message Type Functions

core_function void
MD_MsgListPush(Arena *arena, MD_MsgList *msgs, MD_Node *node, MD_MsgKind kind, String8 string)
{
 MD_Msg *msg = PushArray(arena, MD_Msg, 1);
 msg->node = node;
 msg->kind = kind;
 msg->string = string;
 QueuePush(msgs->first, msgs->last, msg);
 msgs->count += 1;
 msgs->worst_message_kind = Max(kind, msgs->worst_message_kind);
}

////////////////////////////////
//~ rjf: Token Type Functions

core_function String8List
MD_StringListFromTokenFlags(Arena *arena, MD_TokenFlags flags)
{
 String8List strs = {0};
 if(flags & MD_TokenFlag_Identifier          ){Str8ListPush(arena, &strs, Str8Lit("Identifier"));}
 if(flags & MD_TokenFlag_Numeric             ){Str8ListPush(arena, &strs, Str8Lit("Numeric"));}
 if(flags & MD_TokenFlag_StringLiteral       ){Str8ListPush(arena, &strs, Str8Lit("StringLiteral"));}
 if(flags & MD_TokenFlag_Symbol              ){Str8ListPush(arena, &strs, Str8Lit("Symbol"));}
 if(flags & MD_TokenFlag_Reserved            ){Str8ListPush(arena, &strs, Str8Lit("Reserved"));}
 if(flags & MD_TokenFlag_Comment             ){Str8ListPush(arena, &strs, Str8Lit("Comment"));}
 if(flags & MD_TokenFlag_Whitespace          ){Str8ListPush(arena, &strs, Str8Lit("Whitespace"));}
 if(flags & MD_TokenFlag_Newline             ){Str8ListPush(arena, &strs, Str8Lit("Newline"));}
 if(flags & MD_TokenFlag_BrokenComment       ){Str8ListPush(arena, &strs, Str8Lit("BrokenComment"));}
 if(flags & MD_TokenFlag_BrokenStringLiteral ){Str8ListPush(arena, &strs, Str8Lit("BrokenStringLiteral"));}
 if(flags & MD_TokenFlag_BadCharacter        ){Str8ListPush(arena, &strs, Str8Lit("BadCharacter"));}
 return strs;
}

core_function void
MD_TokenChunkListPush(Arena *arena, MD_TokenChunkList *list, U64 cap, MD_Token token)
{
 MD_TokenChunkNode *node = list->last;
 if(node == 0 || node->count >= node->cap)
 {
  node = PushArray(arena, MD_TokenChunkNode, 1);
  node->cap = cap;
  node->v = PushArrayNoZero(arena, MD_Token, cap);
  QueuePush(list->first, list->last, node);
  list->chunk_count += 1;
 }
 MemoryCopyStruct(&node->v[node->count], &token);
 node->count += 1;
 list->total_token_count += 1;
}

core_function MD_TokenArray
MD_TokenArrayFromChunkList(Arena *arena, MD_TokenChunkList *chunks)
{
 MD_TokenArray result = {0};
 result.count = chunks->total_token_count;
 result.v = PushArrayNoZero(arena, MD_Token, result.count);
 U64 write_idx = 0;
 for(MD_TokenChunkNode *n = chunks->first; n != 0; n = n->next)
 {
  MemoryCopy(result.v+write_idx, n->v, sizeof(MD_Token)*n->count);
  write_idx += n->count;
 }
 return result;
}

core_function String8
MD_ContentStringFromTokenFlagsStr8(MD_TokenFlags flags, String8 string)
{
 U64 num_chop = 0;
 U64 num_skip = 0;
 {
  num_skip += 3*!!(flags & MD_TokenFlag_StringTriplet);
  num_chop += 3*!!(flags & MD_TokenFlag_StringTriplet);
  num_skip += 1*(!(flags & MD_TokenFlag_StringTriplet) && flags & MD_TokenFlag_StringLiteral);
  num_chop += 1*(!(flags & MD_TokenFlag_StringTriplet) && flags & MD_TokenFlag_StringLiteral);
 }
 String8 result = string;
 result = Str8Chop(result, num_chop);
 result = Str8Skip(result, num_skip);
 return result;
}

////////////////////////////////
//~ rjf: Node Type Functions

//- rjf: flag conversions

core_function MD_NodeFlags
MD_NodeFlagsFromTokenFlags(MD_TokenFlags flags)
{
 MD_NodeFlags result = 0;
 result |=         MD_NodeFlag_Identifier*!!(flags&MD_TokenFlag_Identifier);
 result |=            MD_NodeFlag_Numeric*!!(flags&MD_TokenFlag_Numeric);
 result |=      MD_NodeFlag_StringLiteral*!!(flags&MD_TokenFlag_StringLiteral);
 result |=             MD_NodeFlag_Symbol*!!(flags&MD_TokenFlag_Symbol);
 result |= MD_NodeFlag_StringSingleQuote	*!!(flags&MD_TokenFlag_StringSingleQuote);
 result |= MD_NodeFlag_StringDoubleQuote	*!!(flags&MD_TokenFlag_StringDoubleQuote);
 result |=         MD_NodeFlag_StringTick*!!(flags&MD_TokenFlag_StringTick);
 result |=      MD_NodeFlag_StringTriplet*!!(flags&MD_TokenFlag_StringTriplet);
 return result;
}

//- rjf: nil

core_function B32
MD_NodeIsNil(MD_Node *node)
{
 return (node == 0 || node == &md_nil_node || node->kind == MD_NodeKind_Nil);
}

//- rjf: iteration

core_function MD_NodeRec
MD_NodeRecDepthFirst(MD_Node *node, MD_Node *subtree_root, MemberOffset child_off, MemberOffset sib_off)
{
 MD_NodeRec rec = {0};
 rec.next = &md_nil_node;
 if(!MD_NodeIsNil(MemberFromOff(node, MD_Node *, child_off)))
 {
  rec.next = MemberFromOff(node, MD_Node *, child_off);
  rec.push_count = 1;
 }
 else for(MD_Node *p = node; !MD_NodeIsNil(p) && p != subtree_root; p = p->parent, rec.pop_count += 1)
 {
  if(!MD_NodeIsNil(MemberFromOff(p, MD_Node *, sib_off)))
  {
   rec.next = MemberFromOff(p, MD_Node *, sib_off);
   break;
  }
 }
 return rec;
}

//- rjf: tree building

core_function MD_Node *
MD_PushNode(Arena *arena, MD_NodeKind kind, MD_NodeFlags flags, String8 string, String8 raw_string, U64 src_offset)
{
 MD_Node *node = PushArray(arena, MD_Node, 1);
 node->first = node->last = node->parent = node->next = node->prev = node->first_tag = node->last_tag = &md_nil_node;
 node->kind       = kind;
 node->flags      = flags;
 node->string     = string;
 node->raw_string = raw_string;
 node->src_offset = src_offset;
 return node;
}

core_function void
MD_NodePushChild(MD_Node *parent, MD_Node *node)
{
 node->parent = parent;
 DLLPushBack_NPZ(parent->first, parent->last, node, next, prev, MD_NodeIsNil, MD_NodeSetNil);
}

core_function void
MD_NodePushTag(MD_Node *parent, MD_Node *node)
{
 node->parent = parent;
 DLLPushBack_NPZ(parent->first_tag, parent->last_tag, node, next, prev, MD_NodeIsNil, MD_NodeSetNil);
}

//- rjf: tree introspection

core_function MD_Node *
MD_NodeFromChainString(MD_Node *first, MD_Node *opl, String8 string, MatchFlags flags)
{
 MD_Node *result = &md_nil_node;
 for(MD_Node *n = first; !MD_NodeIsNil(n) && n != opl; n = n->next)
 {
  if(Str8Match(n->string, string, flags))
  {
   result = n;
   break;
  }
 }
 return result;
}

core_function MD_Node *
MD_NodeFromChainIndex(MD_Node *first, MD_Node *opl, U64 index)
{
 MD_Node *result = &md_nil_node;
 S64 idx = 0;
 for(MD_Node *n = first; !MD_NodeIsNil(n) && n != opl; n = n->next, idx += 1)
 {
  if(index == idx)
  {
   result = n;
   break;
  }
 }
 return result;
}

core_function MD_Node *
MD_NodeFromChainFlags(MD_Node *first, MD_Node *opl, MD_NodeFlags flags)
{
 MD_Node *result = &md_nil_node;
 for(MD_Node *n = first; !MD_NodeIsNil(n) && n != opl; n = n->next)
 {
  if(n->flags & flags)
  {
   result = n;
   break;
  }
 }
 return result;
}

core_function U64
MD_IndexFromNode(MD_Node *node)
{
 U64 index = 0;
 for(MD_Node *n = node->prev; !MD_NodeIsNil(n); n = n->prev)
 {
  index += 1;
 }
 return index;
}

core_function MD_Node *
MD_RootFromNode(MD_Node *node)
{
 MD_Node *result = node;
 for(MD_Node *p = node->parent; (p->kind == MD_NodeKind_Main || p->kind == MD_NodeKind_Tag) && !MD_NodeIsNil(p); p = p->parent)
 {
  result = p;
 }
 return result;
}

core_function MD_Node *
MD_ChildFromString(MD_Node *node, String8 child_string, MatchFlags flags)
{
 return MD_NodeFromChainString(node->first, &md_nil_node, child_string, flags);
}

core_function MD_Node *
MD_TagFromString(MD_Node *node, String8 tag_string, MatchFlags flags)
{
 return MD_NodeFromChainString(node->first_tag, &md_nil_node, tag_string, flags);
}

core_function MD_Node *
MD_ChildFromIndex(MD_Node *node, U64 index)
{
 return MD_NodeFromChainIndex(node->first, &md_nil_node, index);
}

core_function MD_Node *
MD_TagFromIndex(MD_Node *node, U64 index)
{
 return MD_NodeFromChainIndex(node->first_tag, &md_nil_node, index);
}

core_function MD_Node *
MD_TagArgFromIndex(MD_Node *node, String8 tag_string, MatchFlags flags, U64 index)
{
 MD_Node *tag = MD_TagFromString(node, tag_string, flags);
 return MD_ChildFromIndex(tag, index);
}

core_function MD_Node *
MD_TagArgFromString(MD_Node *node, String8 tag_string, MatchFlags tag_str_flags, String8 arg_string, MatchFlags arg_str_flags)
{
 MD_Node *tag = MD_TagFromString(node, tag_string, tag_str_flags);
 MD_Node *arg = MD_ChildFromString(tag, arg_string, arg_str_flags);
 return arg;
}

core_function B32
MD_NodeHasChild(MD_Node *node, String8 string, MatchFlags flags)
{
 return !MD_NodeIsNil(MD_ChildFromString(node, string, flags));
}

core_function B32
MD_NodeHasTag(MD_Node *node, String8 string, MatchFlags flags)
{
 return !MD_NodeIsNil(MD_TagFromString(node, string, flags));
}

core_function U64
MD_ChildCountFromNode(MD_Node *node)
{
 U64 result = 0;
 for(MD_Node *child = node->first; !MD_NodeIsNil(child); child = child->next)
 {
  result += 1;
 }
 return result;
}

core_function U64
MD_TagCountFromNode(MD_Node *node)
{
 U64 result = 0;
 for(MD_Node *child = node->first_tag; !MD_NodeIsNil(child); child = child->next)
 {
  result += 1;
 }
 return result;
}

////////////////////////////////
//~ rjf: Text -> Tokens Functions

core_function MD_TokenizeResult
MD_TokenizeFromText(Arena *arena, String8 filename, String8 text)
{
 ArenaTemp scratch = GetScratch(&arena, 1);
 MD_TokenChunkList tokens = {0};
 MD_MsgList msgs = {0};
 U8 *byte_first = text.str;
 U8 *byte_opl = byte_first + text.size;
 U8 *byte = byte_first;
 
 //- rjf: scan string & produce tokens
 for(;byte < byte_opl;)
 {
  MD_TokenFlags token_flags = 0;
  U8 *token_start = 0;
  U8 *token_opl = 0;
  
  //- rjf: whitespace
  if(token_flags == 0 && (*byte == ' ' || *byte == '\t' || *byte == '\v' || *byte == '\r'))
  {
   token_flags = MD_TokenFlag_Whitespace;
   token_start = byte;
   token_opl = byte;
   byte += 1;
   for(;byte <= byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl || (*byte != ' ' && *byte != '\t' && *byte != '\v' && *byte != '\r'))
    {
     break;
    }
   }
  }
  
  //- rjf: newlines
  if(token_flags == 0 && *byte == '\n')
  {
   token_flags = MD_TokenFlag_Newline;
   token_start = byte;
   token_opl = byte+1;
   byte += 1;
  }
  
  //- rjf: single-line comments
  if(token_flags == 0 && (byte+1 < byte_opl && *byte == '/' && byte[1] == '/'))
  {
   token_flags = MD_TokenFlag_Comment;
   token_start = byte;
   token_opl = byte+2;
   byte += 2;
   B32 escaped = 0;
   for(;byte <= byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl)
    {
     break;
    }
    if(escaped)
    {
     escaped = 0;
    }
    else
    {
     if(*byte == '\n')
     {
      break;
     }
     else if(*byte == '\\')
     {
      escaped = 1;
     }
    }
   }
  }
  
  //- rjf: multi-line comments
  if(token_flags == 0 && (byte+1 < byte_opl && *byte == '/' && byte[1] == '*'))
  {
   token_flags = MD_TokenFlag_Comment;
   token_start = byte;
   token_opl = byte+2;
   byte += 2;
   for(;byte <= byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl)
    {
     token_flags |= MD_TokenFlag_BrokenComment;
     break;
    }
    if(byte+1 < byte_opl && byte[0] == '*' && byte[1] == '/')
    {
     token_opl += 2;
     break;
    }
   }
  }
  
  //- rjf: identifiers
  if(token_flags == 0 && (('A' <= *byte && *byte <= 'Z') ||
                          ('a' <= *byte && *byte <= 'z') ||
                          *byte == '_' ||
                          utf8_class[*byte>>3] >= 2 ))
  {
   token_flags = MD_TokenFlag_Identifier;
   token_start = byte;
   token_opl = byte;
   byte += 1;
   for(;byte <= byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl ||
       (!('A' <= *byte && *byte <= 'Z') &&
        !('a' <= *byte && *byte <= 'z') &&
        !('0' <= *byte && *byte <= '9') &&
        *byte != '_' &&
        utf8_class[*byte>>3] < 2))
    {
     break;
    }
   }
  }
  
  //- rjf: numerics
  if(token_flags == 0 && (('0' <= *byte && *byte <= '9') ||
                          *byte == '.' ||
                          (*byte == '-' && byte+1 < byte_opl && '0' <= byte[1] && byte[1] <= '9') ||
                          *byte == '_'))
  {
   token_flags = MD_TokenFlag_Numeric;
   token_start = byte;
   token_opl = byte;
   byte += 1;
   for(;byte <= byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl ||
       (!('A' <= *byte && *byte <= 'Z') &&
        !('a' <= *byte && *byte <= 'z') &&
        !('0' <= *byte && *byte <= '9') &&
        *byte != '_'))
    {
     break;
    }
   }
  }
  
  //- rjf: triplet string literals
  if(token_flags == 0 && byte+2 < byte_opl &&
     ((byte[0] == '"' && byte[1] == '"' && byte[2] == '"') ||
      (byte[0] == '\''&& byte[1] == '\''&& byte[2] == '\'') ||
      (byte[0] == '`' && byte[1] == '`' && byte[2] == '`')))
  {
   U8 literal_style = byte[0];
   token_flags = MD_TokenFlag_StringLiteral|MD_TokenFlag_StringTriplet;
   token_flags |= (literal_style == '\'')*MD_TokenFlag_StringSingleQuote;
   token_flags |= (literal_style ==  '"')*MD_TokenFlag_StringDoubleQuote;
   token_flags |= (literal_style ==  '`')*MD_TokenFlag_StringTick;
   token_start = byte;
   token_opl = byte+3;
   byte += 3;
   for(;byte <= byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl)
    {
     token_flags |= MD_TokenFlag_BrokenStringLiteral;
     break;
    }
    if(byte+2 < byte_opl && (byte[0] == literal_style && byte[1] == literal_style && byte[2] == literal_style))
    {
     byte += 3;
     token_opl += 3;
     break;
    }
   }
  }
  
  //- rjf: singlet string literals
  if(token_flags == 0 && (byte[0] == '"' || byte[0] == '\'' || byte[0] == '`'))
  {
   U8 literal_style = byte[0];
   token_flags = MD_TokenFlag_StringLiteral;
   token_flags |= (literal_style == '\'')*MD_TokenFlag_StringSingleQuote;
   token_flags |= (literal_style ==  '"')*MD_TokenFlag_StringDoubleQuote;
   token_flags |= (literal_style ==  '`')*MD_TokenFlag_StringTick;
   token_start = byte;
   token_opl = byte+1;
   byte += 1;
   for(;byte <= byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl || *byte == '\n')
    {
     token_flags |= MD_TokenFlag_BrokenStringLiteral;
     break;
    }
    if(byte[0] == literal_style)
    {
     byte += 1;
     break;
    }
   }
  }
  
  //- rjf: non-reserved symbols
  if(token_flags == 0 && (*byte == '~' || *byte == '!' || *byte == '$' || *byte == '%' || *byte == '^' ||
                          *byte == '&' || *byte == '*' || *byte == '-' || *byte == '=' || *byte == '+' ||
                          *byte == '<' || *byte == '.' || *byte == '>' || *byte == '/' || *byte == '?' ||
                          *byte == '|'))
  {
   token_flags = MD_TokenFlag_Symbol;
   token_start = byte;
   token_opl = byte;
   byte += 1;
   for(;byte <=byte_opl; byte += 1)
   {
    token_opl += 1;
    if(byte == byte_opl ||
       (*byte != '~' && *byte != '!' && *byte != '$' && *byte != '%' && *byte != '^' &&
        *byte != '&' && *byte != '*' && *byte != '-' && *byte != '=' && *byte != '+' &&
        *byte != '<' && *byte != '.' && *byte != '>' && *byte != '/' && *byte != '?' &&
        *byte != '|'))
    {
     break;
    }
   }
  }
  
  //- rjf: reserved symbols
  if(token_flags == 0 && (*byte == '{' || *byte == '}' || *byte == '(' || *byte == ')' ||
                          *byte == '[' || *byte == ']' || *byte == '#' || *byte == ',' ||
                          *byte == '\\'|| *byte == ':' || *byte == ';' || *byte == '@'))
  {
   token_flags = MD_TokenFlag_Reserved;
   token_start = byte;
   token_opl = byte+1;
   byte += 1;
  }
  
  //- rjf: bad characters in all other cases
  if(token_flags == 0)
  {
   token_flags = MD_TokenFlag_BadCharacter;
   token_start = byte;
   token_opl = byte+1;
   byte += 1;
  }
  
  //- rjf; push token if formed
  if(token_flags != 0 && token_start != 0 && token_opl > token_start)
  {
   MD_Token token = {{token_start - byte_first, token_opl - byte_first}, token_flags};
   MD_TokenChunkListPush(scratch.arena, &tokens, 4096, token);
  }
  
  //- rjf: push errors on unterminated comments
  if(token_flags & MD_TokenFlag_BrokenComment)
  {
   MD_Node *error = MD_PushNode(arena, MD_NodeKind_ErrorMarker, 0, Str8Lit(""), Str8Lit(""), token_start - byte_first);
   String8 error_string = Str8Lit("Unterminated comment.");
   MD_MsgListPush(arena, &msgs, error, MD_MsgKind_Error, error_string);
  }
  
  //- rjf: push errors on unterminated strings
  if(token_flags & MD_TokenFlag_BrokenStringLiteral)
  {
   MD_Node *error = MD_PushNode(arena, MD_NodeKind_ErrorMarker, 0, Str8Lit(""), Str8Lit(""), token_start - byte_first);
   String8 error_string = Str8Lit("Unterminated string literal.");
   MD_MsgListPush(arena, &msgs, error, MD_MsgKind_Error, error_string);
  }
 }
 
 //- rjf: bake, fill & return
 MD_TokenizeResult result = {0};
 {
  result.tokens = MD_TokenArrayFromChunkList(arena, &tokens);
  result.msgs = msgs;
 }
 ReleaseScratch(scratch);
 return result;
}

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

core_function MD_ParseResult
MD_ParseFromTextTokens(Arena *arena, String8 filename, String8 text, MD_TokenArray tokens)
{
 ArenaTemp scratch = GetScratch(&arena, 1);
 
 //- rjf: set up outputs
 MD_MsgList msgs = {0};
 MD_Node *root = MD_PushNode(arena, MD_NodeKind_File, 0, filename, text, 0);
 
 //- rjf: set up parse rule stack
 typedef enum MD_ParseWorkKind
 {
  MD_ParseWorkKind_Main,
  MD_ParseWorkKind_MainImplicit,
  MD_ParseWorkKind_NodeOptionalFollowUp,
  MD_ParseWorkKind_NodeChildrenStyleScan,
 }
 MD_ParseWorkKind;
 typedef struct MD_ParseWorkNode MD_ParseWorkNode;
 struct MD_ParseWorkNode
 {
  MD_ParseWorkNode *next;
  MD_ParseWorkKind kind;
  MD_Node *parent;
  MD_Node *first_gathered_tag;
  MD_Node *last_gathered_tag;
  MD_NodeFlags gathered_node_flags;
  S32 counted_newlines;
 };
 MD_ParseWorkNode first_work =
 {
  0,
  MD_ParseWorkKind_Main,
  root,
 };
 MD_ParseWorkNode broken_work = { 0, MD_ParseWorkKind_Main, root,};
 MD_ParseWorkNode *work_top = &first_work;
 MD_ParseWorkNode *work_free = 0;
#define MD_ParseWorkPush(work_kind, work_parent) do\
{\
MD_ParseWorkNode *work_node = work_free;\
if(work_node == 0) {work_node = PushArray(scratch.arena, MD_ParseWorkNode, 1);}\
else { StackPop(work_free); }\
work_node->kind = (work_kind);\
work_node->parent = (work_parent);\
StackPush(work_top, work_node);\
}while(0)
#define MD_ParseWorkPop() do\
{\
StackPop(work_top);\
if(work_top == 0) {work_top = &broken_work;}\
}while(0)
 
 //- rjf: parse
 MD_Token *tokens_first = tokens.v;
 MD_Token *tokens_opl = tokens_first + tokens.count;
 MD_Token *token = tokens_first;
 for(;token < tokens_opl;)
 {
  //- rjf: unpack token
  String8 token_string = Substr8(text, token[0].range);
  
  //- rjf: whitespace -> always no-op & inc
  if(token->flags & MD_TokenFlag_Whitespace)
  {
   token += 1;
   goto end_consume;
  }
  
  //- rjf: comments -> always no-op & inc
  if(token->flags & MD_TokenGroup_Comment)
  {
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [node follow up] : following label -> work top parent has children. we need
  // to scan for explicit delimiters, else parse an implicitly delimited set of children
  if(work_top->kind == MD_ParseWorkKind_NodeOptionalFollowUp && Str8Match(token_string, Str8Lit(":"), 0))
  {
   MD_Node *parent = work_top->parent;
   MD_ParseWorkPop();
   MD_ParseWorkPush(MD_ParseWorkKind_NodeChildrenStyleScan, parent);
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [node follow up] anything but : following label -> node has no children. just
  // pop & move on
  if(work_top->kind == MD_ParseWorkKind_NodeOptionalFollowUp)
  {
   MD_ParseWorkPop();
   goto end_consume;
  }
  
  //- rjf: [main] separators -> mark & inc
  if(work_top->kind == MD_ParseWorkKind_Main && token->flags & MD_TokenFlag_Reserved &&
     (Str8Match(token_string, Str8Lit(","), 0) ||
      Str8Match(token_string, Str8Lit(";"), 0)))
  {
   MD_Node *parent = work_top->parent;
   if(!MD_NodeIsNil(parent->last))
   {
    parent->last->flags |=     MD_NodeFlag_IsBeforeComma*!!Str8Match(token_string, Str8Lit(","), 0);
    parent->last->flags |= MD_NodeFlag_IsBeforeSemicolon*!!Str8Match(token_string, Str8Lit(";"), 0);
    work_top->gathered_node_flags |=     MD_NodeFlag_IsAfterComma*!!Str8Match(token_string, Str8Lit(","), 0);
    work_top->gathered_node_flags |= MD_NodeFlag_IsAfterSemicolon*!!Str8Match(token_string, Str8Lit(";"), 0);
   }
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [main_implicit] separators -> pop
  if(work_top->kind == MD_ParseWorkKind_MainImplicit && token->flags & MD_TokenFlag_Reserved &&
     (Str8Match(token_string, Str8Lit(","), 0) ||
      Str8Match(token_string, Str8Lit(";"), 0)))
  {
   MD_ParseWorkPop();
   goto end_consume;
  }
  
  //- rjf: [main, main_implicit] unexpected reserved tokens
  if((work_top->kind == MD_ParseWorkKind_Main || work_top->kind == MD_ParseWorkKind_MainImplicit) &&
     token->flags & MD_TokenFlag_Reserved &&
     (Str8Match(token_string, Str8Lit("#"), 0) ||
      Str8Match(token_string, Str8Lit("\\"), 0) ||
      Str8Match(token_string, Str8Lit(":"), 0)))
  {
   MD_Node *error = MD_PushNode(arena, MD_NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
   String8 error_string = PushStr8F(arena, "Unexpected reserved symbol \"%S\".", token_string);
   MD_MsgListPush(arena, &msgs, error, MD_MsgKind_Error, error_string);
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [main, main_implicit] tag signifier -> create new tag
  if((work_top->kind == MD_ParseWorkKind_Main || work_top->kind == MD_ParseWorkKind_MainImplicit) &&
     token[0].flags & MD_TokenFlag_Reserved && Str8Match(token_string, Str8Lit("@"), 0))
  {
   if(token+1 >= tokens_opl ||
      !(token[1].flags & MD_TokenGroup_Label))
   {
    MD_Node *error = MD_PushNode(arena, MD_NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
    String8 error_string = Str8Lit("Tag label expected after @ symbol.");
    MD_MsgListPush(arena, &msgs, error, MD_MsgKind_Error, error_string);
    token += 1;
    goto end_consume;
   }
   else
   {
    String8 tag_name_raw = Substr8(text, token[1].range);
    String8 tag_name = MD_ContentStringFromTokenFlagsStr8(token[1].flags, tag_name_raw);
    MD_Node *node = MD_PushNode(arena, MD_NodeKind_Tag, MD_NodeFlagsFromTokenFlags(token[1].flags), tag_name, tag_name_raw, token[0].range.min);
    DLLPushBack_NPZ(work_top->first_gathered_tag, work_top->last_gathered_tag, node, next, prev, MD_NodeIsNil, MD_NodeSetNil);
    if(token+2 < tokens_opl && token[2].flags & MD_TokenFlag_Reserved && Str8Match(Substr8(text, token[2].range), Str8Lit("("), 0))
    {
     token += 3;
     MD_ParseWorkPush(MD_ParseWorkKind_Main, node);
    }
    else
    {
     token += 2;
    }
    goto end_consume;
   }
  }
  
  //- rjf: [main, main_implicit] label -> create new main
  if((work_top->kind == MD_ParseWorkKind_Main || work_top->kind == MD_ParseWorkKind_MainImplicit) &&
     token->flags & MD_TokenGroup_Label)
  {
   String8 node_string_raw = token_string;
   String8 node_string = MD_ContentStringFromTokenFlagsStr8(token->flags, node_string_raw);
   MD_NodeFlags flags = MD_NodeFlagsFromTokenFlags(token->flags)|work_top->gathered_node_flags;
   work_top->gathered_node_flags = 0;
   MD_Node *node = MD_PushNode(arena, MD_NodeKind_Main, flags, node_string, node_string_raw, token[0].range.min);
   node->first_tag = work_top->first_gathered_tag;
   node->last_tag = work_top->last_gathered_tag;
   for(MD_Node *tag = work_top->first_gathered_tag; !MD_NodeIsNil(tag); tag = tag->next)
   {
    tag->parent = node;
   }
   work_top->first_gathered_tag = work_top->last_gathered_tag = &md_nil_node;
   MD_NodePushChild(work_top->parent, node);
   MD_ParseWorkPush(MD_ParseWorkKind_NodeOptionalFollowUp, node);
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [main] {s, [s, and (s -> create new main
  if(work_top->kind == MD_ParseWorkKind_Main && token->flags & MD_TokenFlag_Reserved &&
     (Str8Match(token_string, Str8Lit("{"), 0) ||
      Str8Match(token_string, Str8Lit("["), 0) ||
      Str8Match(token_string, Str8Lit("("), 0)))
  {
   MD_NodeFlags flags = MD_NodeFlagsFromTokenFlags(token->flags)|work_top->gathered_node_flags;
   flags |=   MD_NodeFlag_HasBraceLeft*!!Str8Match(token_string, Str8Lit("{"), 0);
   flags |= MD_NodeFlag_HasBracketLeft*!!Str8Match(token_string, Str8Lit("["), 0);
   flags |=   MD_NodeFlag_HasParenLeft*!!Str8Match(token_string, Str8Lit("("), 0);
   work_top->gathered_node_flags = 0;
   MD_Node *node = MD_PushNode(arena, MD_NodeKind_Main, flags, Str8Lit(""), Str8Lit(""), token[0].range.min);
   node->first_tag = work_top->first_gathered_tag;
   node->last_tag = work_top->last_gathered_tag;
   for(MD_Node *tag = work_top->first_gathered_tag; !MD_NodeIsNil(tag); tag = tag->next)
   {
    tag->parent = node;
   }
   work_top->first_gathered_tag = work_top->last_gathered_tag = &md_nil_node;
   MD_NodePushChild(work_top->parent, node);
   MD_ParseWorkPush(MD_ParseWorkKind_Main, node);
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [node children style scan] {s, [s, and (s -> explicitly delimited children
  if(work_top->kind == MD_ParseWorkKind_NodeChildrenStyleScan && token->flags & MD_TokenFlag_Reserved &&
     (Str8Match(token_string, Str8Lit("{"), 0) ||
      Str8Match(token_string, Str8Lit("["), 0) ||
      Str8Match(token_string, Str8Lit("("), 0)))
  {
   MD_Node *parent = work_top->parent;
   parent->flags |=   MD_NodeFlag_HasBraceLeft*!!Str8Match(token_string, Str8Lit("{"), 0);
   parent->flags |= MD_NodeFlag_HasBracketLeft*!!Str8Match(token_string, Str8Lit("["), 0);
   parent->flags |=   MD_NodeFlag_HasParenLeft*!!Str8Match(token_string, Str8Lit("("), 0);
   MD_ParseWorkPop();
   MD_ParseWorkPush(MD_ParseWorkKind_Main, parent);
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [node children style scan] count newlines
  if(work_top->kind == MD_ParseWorkKind_NodeChildrenStyleScan && token->flags & MD_TokenFlag_Newline)
  {
   work_top->counted_newlines += 1;
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [main_implicit] newline -> pop
  if(work_top->kind == MD_ParseWorkKind_MainImplicit && token->flags & MD_TokenFlag_Newline)
  {
   MD_ParseWorkPop();
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [all but main_implicit] newline -> no-op & inc
  if(work_top->kind != MD_ParseWorkKind_MainImplicit && token->flags & MD_TokenFlag_Newline)
  {
   token += 1;
   goto end_consume;
  }
  
  //- rjf: [node children style scan] anything causing implicit set -> <2 newlines, all good,
  // >=2 newlines, houston we have a problem
  if(work_top->kind == MD_ParseWorkKind_NodeChildrenStyleScan)
  {
   if(work_top->counted_newlines >= 2)
   {
    MD_Node *node = work_top->parent;
    MD_Node *error = MD_PushNode(arena, MD_NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
    String8 error_string = PushStr8F(arena, "More than two newlines following \"%S\", which has implicitly-delimited children, resulting in an empty list of children.", node->string);
    MD_MsgListPush(arena, &msgs, error, MD_MsgKind_Warning, error_string);
    MD_ParseWorkPop();
   }
   else
   {
    MD_Node *parent = work_top->parent;
    MD_ParseWorkPop();
    MD_ParseWorkPush(MD_ParseWorkKind_MainImplicit, parent);
   }
   goto end_consume;
  }
  
  //- rjf: [main] }s, ]s, and )s -> pop
  if(work_top->kind == MD_ParseWorkKind_Main && token->flags & MD_TokenFlag_Reserved &&
     (Str8Match(token_string, Str8Lit("}"), 0) ||
      Str8Match(token_string, Str8Lit("]"), 0) ||
      Str8Match(token_string, Str8Lit(")"), 0)))
  {
   MD_Node *parent = work_top->parent;
   parent->flags |=   MD_NodeFlag_HasBraceRight*!!Str8Match(token_string, Str8Lit("}"), 0);
   parent->flags |= MD_NodeFlag_HasBracketRight*!!Str8Match(token_string, Str8Lit("]"), 0);
   parent->flags |=   MD_NodeFlag_HasParenRight*!!Str8Match(token_string, Str8Lit(")"), 0);
   MD_ParseWorkPop();
   token += 1;
   goto end_consume;
  }
  
  //- rjf: no consumption -> unexpected token! we don't know what to do with this.
  {
   MD_Node *error = MD_PushNode(arena, MD_NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
   String8 error_string = PushStr8F(arena, "Unexpected \"%S\" token.", token_string);
   MD_MsgListPush(arena, &msgs, error, MD_MsgKind_Error, error_string);
   token += 1;
  }
  
  end_consume:;
 }
 
 //- rjf: fill & return
 MD_ParseResult result = {0};
 result.root = root;
 result.msgs = msgs;
 ReleaseScratch(scratch);
 return result;
}
