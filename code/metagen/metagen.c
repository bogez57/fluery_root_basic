////////////////////////////////
//~ rjf: String Expression Operator Tables

read_only global String8 mg_str_expr_op_symbol_string_table[MG_StrExprOp_COUNT] =
{
 Str8LitComp(""),
 Str8LitComp("."),  // MG_StrExprOp_Dot
 Str8LitComp("??"), // MG_StrExprOp_ExpandIfTrue
 Str8LitComp(".."), // MG_StrExprOp_Concat
 Str8LitComp("=>"), // MG_StrExprOp_BumpToColumn
 Str8LitComp("+"),  // MG_StrExprOp_Add
 Str8LitComp("-"),  // MG_StrExprOp_Subtract
 Str8LitComp("*"),  // MG_StrExprOp_Multiply
 Str8LitComp("/"),  // MG_StrExprOp_Divide
 Str8LitComp("%"),  // MG_StrExprOp_Modulo
 Str8LitComp("<<"), // MG_StrExprOp_LeftShift
 Str8LitComp(">>"), // MG_StrExprOp_RightShift
 Str8LitComp("&"),  // MG_StrExprOp_BitwiseAnd
 Str8LitComp("|"),  // MG_StrExprOp_BitwiseOr
 Str8LitComp("^"),  // MG_StrExprOp_BitwiseXor
 Str8LitComp("~"),  // MG_StrExprOp_BitwiseNegate
 Str8LitComp("&&"), // MG_StrExprOp_BooleanAnd
 Str8LitComp("||"), // MG_StrExprOp_BooleanOr
 Str8LitComp("!"),  // MG_StrExprOp_BooleanNot
 Str8LitComp("=="), // MG_StrExprOp_Equals
 Str8LitComp("!="), // MG_StrExprOp_DoesNotEqual
};

read_only global S8 mg_str_expr_op_precedence_table[MG_StrExprOp_COUNT] =
{
 0,
 20, // MG_StrExprOp_Dot
 1,  // MG_StrExprOp_ExpandIfTrue
 2,  // MG_StrExprOp_Concat
 12, // MG_StrExprOp_BumpToColumn
 5,  // MG_StrExprOp_Add
 5,  // MG_StrExprOp_Subtract
 6,  // MG_StrExprOp_Multiply
 6,  // MG_StrExprOp_Divide
 6,  // MG_StrExprOp_Modulo
 7,  // MG_StrExprOp_LeftShift
 7,  // MG_StrExprOp_RightShift
 8,  // MG_StrExprOp_BitwiseAnd
 10, // MG_StrExprOp_BitwiseOr
 9,  // MG_StrExprOp_BitwiseXor
 11, // MG_StrExprOp_BitwiseNegate
 3,  // MG_StrExprOp_BooleanAnd
 3,  // MG_StrExprOp_BooleanOr
 11, // MG_StrExprOp_BooleanNot
 4,  // MG_StrExprOp_Equals
 4,  // MG_StrExprOp_DoesNotEqual
};

read_only global MG_StrExprOpKind mg_str_expr_op_kind_table[MG_StrExprOp_COUNT] =
{
 MG_StrExprOpKind_Null,
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Dot
 MG_StrExprOpKind_Binary, // MG_StrExprOp_ExpandIfTrue
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Concat
 MG_StrExprOpKind_Prefix, // MG_StrExprOp_BumpToColumn
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Add
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Subtract
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Multiply
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Divide
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Modulo
 MG_StrExprOpKind_Binary, // MG_StrExprOp_LeftShift
 MG_StrExprOpKind_Binary, // MG_StrExprOp_RightShift
 MG_StrExprOpKind_Binary, // MG_StrExprOp_BitwiseAnd
 MG_StrExprOpKind_Binary, // MG_StrExprOp_BitwiseOr
 MG_StrExprOpKind_Binary, // MG_StrExprOp_BitwiseXor
 MG_StrExprOpKind_Prefix, // MG_StrExprOp_BitwiseNegate
 MG_StrExprOpKind_Binary, // MG_StrExprOp_BooleanAnd
 MG_StrExprOpKind_Binary, // MG_StrExprOp_BooleanOr
 MG_StrExprOpKind_Prefix, // MG_StrExprOp_BooleanNot
 MG_StrExprOpKind_Binary, // MG_StrExprOp_Equals
 MG_StrExprOpKind_Binary, // MG_StrExprOp_DoesNotEqual
};

////////////////////////////////
//~ rjf: Basic Type Helpers

function U64
MG_HashFromString(String8 string)
{
 U64 result = 5381;
 for(U64 i = 0; i < string.size; i += 1)
 {
  result = ((result << 5) + result) + string.str[i];
 }
 return result;
}

////////////////////////////////
//~ rjf: Tweak Parsing

function MG_TweakList
MG_TweakListFromTextTokensName(Arena *arena, String8 text, CL_TokenArray tokens, String8 name)
{
 MG_TweakList result = {0};
 CL_Token *tokens_first = tokens.v;
 CL_Token *tokens_opl = tokens.v+tokens.count;
 for(CL_Token *token = tokens_first; token < tokens_opl; token += 1)
 {
  String8 token_string = Substr8(text, token->range);
  if(token->kind == CL_TokenKind_Identifier && Str8Match(token_string, name, 0))
  {
   CL_Token *tweak_first = token;
   
   // rjf: extract argument list tokens
   CL_TokenPtrList args_tokens = {0};
   {
    S32 paren_nest = 0;
    B32 done = 0;
    for(CL_Token *token = tweak_first; token < tokens_opl && !done; token += 1)
    {
     String8 token_string = Substr8(text, token->range);
     B32 is_paren = 0;
     if(token->kind == CL_TokenKind_Symbol && Str8Match(token_string, Str8Lit("("), 0))
     {
      is_paren = 1;
      paren_nest += 1;
     }
     if(token->kind == CL_TokenKind_Symbol && Str8Match(token_string, Str8Lit(")"), 0))
     {
      is_paren = 1;
      paren_nest -= 1;
      done = (paren_nest == 0);
     }
     B32 is_whitespace = token->kind == CL_TokenKind_Whitespace || token->kind == CL_TokenKind_Newline;
     if(!done && !is_paren && !is_whitespace && paren_nest > 0)
     {
      CL_TokenPtrListPush(mg_arena, &args_tokens, token);
     }
    }
   }
   
   // rjf: separate args tokens into args
   CL_TokenPtrListList args = {0};
   {
    CL_TokenPtrList arg_parts = {0};
    for(CL_TokenPtrNode *n = args_tokens.first; n != 0; n = n->next)
    {
     CL_Token *token = n->v;
     String8 token_string = Substr8(text, token->range);
     B32 is_comma = (token->kind == CL_TokenKind_Symbol && Str8Match(token_string, Str8Lit(","), 0));
     if(!is_comma)
     {
      CL_TokenPtrListPush(mg_arena, &arg_parts, token);
     }
     if(is_comma || n->next == 0)
     {
      CL_TokenPtrListListPush(mg_arena, &args, &arg_parts);
      MemoryZeroStruct(&arg_parts);
     }
    }
   }
   
   // rjf: first argument -> name of the tweak
   String8 tweak_name = {0};
   if(args.first != 0 && args.first->v.count != 0)
   {
    CL_TokenPtrList parts = args.first->v;
    tweak_name = Substr8(text, parts.first->v->range);
   }
   
   // rjf: 2nd argument -> default
   String8 tweak_default = {0};
   if(args.first != 0 &&
      args.first->next != 0 && args.first->next->v.count != 0)
   {
    CL_TokenPtrList parts = args.first->next->v;
    tweak_default = Substr8(text, Union1U64(parts.first->v->range, parts.last->v->range));
   }
   
   // rjf: 3nd argument -> range min
   String8 tweak_min = {0};
   if(args.first != 0 &&
      args.first->next != 0 &&
      args.first->next->next != 0 && args.first->next->next->v.count != 0)
   {
    CL_TokenPtrList parts = args.first->next->next->v;
    tweak_min = Substr8(text, Union1U64(parts.first->v->range, parts.last->v->range));
   }
   
   // rjf: 4th argument -> range max
   String8 tweak_max = {0};
   if(args.first != 0 &&
      args.first->next != 0 &&
      args.first->next->next != 0 &&
      args.first->next->next->next != 0 && args.first->next->next->next->v.count != 0)
   {
    CL_TokenPtrList parts = args.first->next->next->next->v;
    tweak_max = Substr8(text, Union1U64(parts.first->v->range, parts.last->v->range));
   }
   
   // rjf: push
   MG_Tweak *tweak = PushArray(mg_arena, MG_Tweak, 1);
   tweak->name = tweak_name;
   tweak->default_val = tweak_default;
   tweak->min = tweak_min;
   tweak->max = tweak_max;
   QueuePush(result.first, result.last, tweak);
   result.count += 1;
  }
 }
 return result;
}

function void
MG_TweakListConcatInPlace(MG_TweakList *dst, MG_TweakList *src)
{
 if(dst->first == 0)
 {
  MemoryCopyStruct(dst, src);
 }
 else if(src->first != 0)
 {
  dst->last->next = src->first;
  dst->last = src->last;
  dst->count += src->count;
 }
 MemoryZeroStruct(src);
}

////////////////////////////////
//~ rjf: String Expression Parsing

function MG_StrExpr *
MG_PushStrExpr(Arena *arena, MG_StrExprOp op, MD_Node *node)
{
 MG_StrExpr *expr = PushArray(arena, MG_StrExpr, 1);
 MemoryCopyStruct(expr, &mg_str_expr_nil);
 expr->op = op;
 expr->node = node;
 return expr;
}

function MG_StrExprParseResult
MG_StrExprParseFromFirstOPL_MinPrec(Arena *arena, MD_Node *first, MD_Node *opl, S8 min_prec)
{
 MG_StrExprParseResult parse = {&mg_str_expr_nil};
 {
  MD_Node *it = first;
  
  //- rjf: consume prefix operators
  MG_StrExpr *leafmost_op = &mg_str_expr_nil;
  for(;it < opl && !MD_NodeIsNil(it);)
  {
   MG_StrExprOp found_op = MG_StrExprOp_Null;
   for(MG_StrExprOp op = (MG_StrExprOp)(MG_StrExprOp_Null+1);
       op < MG_StrExprOp_COUNT;
       op = (MG_StrExprOp)(op+1))
   {
    if(mg_str_expr_op_kind_table[op] == MG_StrExprOpKind_Prefix &&
       Str8Match(it->string, mg_str_expr_op_symbol_string_table[op], 0) &&
       mg_str_expr_op_precedence_table[op] >= min_prec)
    {
     found_op = op;
     break;
    }
   }
   if(found_op != MG_StrExprOp_Null)
   {
    MG_StrExpr *op_expr = MG_PushStrExpr(arena, found_op, it);
    if(leafmost_op == &mg_str_expr_nil)
    {
     leafmost_op = op_expr;
    }
    op_expr->left = parse.root;
    parse.root = op_expr;
    it = it->next;
   }
   else
   {
    break;
   }
  }
  
  //- rjf: parse atom
  {
   MG_StrExpr *atom = &mg_str_expr_nil;
   if(it->flags & MD_NodeFlag_Identifier|MD_NodeFlag_Numeric|MD_NodeFlag_StringLiteral &&
      MD_NodeIsNil(it->first))
   {
    atom = MG_PushStrExpr(arena, MG_StrExprOp_Null, it);
    it = it->next;
   }
   else if(!MD_NodeIsNil(it->first))
   {
    MG_StrExprParseResult subparse = MG_StrExprParseFromFirstOPL_MinPrec(arena, it->first, &md_nil_node, 0);
    atom = subparse.root;
    MD_MsgListConcatInPlace(&parse.msgs, &subparse.msgs);
    it = it->next;
   }
   if(leafmost_op != &mg_str_expr_nil)
   {
    leafmost_op->left = atom;
   }
   else
   {
    parse.root = atom;
   }
  }
  
  //- rjf: parse binary operator extensions at this precedence level
  for(;it < opl && !MD_NodeIsNil(it);)
  {
   // rjf: find binary op kind of `it`
   MG_StrExprOp found_op = MG_StrExprOp_Null;
   for(MG_StrExprOp op = (MG_StrExprOp)(MG_StrExprOp_Null+1);
       op < MG_StrExprOp_COUNT;
       op = (MG_StrExprOp)(op+1))
   {
    if(mg_str_expr_op_kind_table[op] == MG_StrExprOpKind_Binary &&
       Str8Match(it->string, mg_str_expr_op_symbol_string_table[op], 0) &&
       mg_str_expr_op_precedence_table[op] >= min_prec)
    {
     found_op = op;
     break;
    }
   }
   
   // rjf: good found_op -> build binary expr
   if(found_op != MG_StrExprOp_Null)
   {
    MG_StrExpr *op_expr = MG_PushStrExpr(arena, found_op, it);
    if(leafmost_op == &mg_str_expr_nil)
    {
     leafmost_op = op_expr;
    }
    op_expr->left = parse.root;
    parse.root = op_expr;
    it = it->next;
   }
   else
   {
    break;
   }
   
   // rjf: parse right hand side of binary operator
   MG_StrExprParseResult subparse = MG_StrExprParseFromFirstOPL_MinPrec(arena, it, opl, mg_str_expr_op_precedence_table[found_op]+1);
   parse.root->right = subparse.root;
   MD_MsgListConcatInPlace(&parse.msgs, &subparse.msgs);
   if(subparse.root == &mg_str_expr_nil)
   {
    MD_MsgListPushF(arena, &parse.msgs, it, MD_MsgKind_Error, "Missing right-hand-side of '%S'.", mg_str_expr_op_symbol_string_table[found_op]);
   }
   it = subparse.next_node;
  }
  
  // rjf: store next node for more caller-side parsing
  parse.next_node = it;
 }
 return parse;
}

function MG_StrExprParseResult
MG_StrExprParseFromFirstOPL(Arena *arena, MD_Node *first, MD_Node *opl)
{
 MG_StrExprParseResult parse = MG_StrExprParseFromFirstOPL_MinPrec(arena, first, opl, 0);
 return parse;
}

function MG_StrExprParseResult
MG_StrExprParseFromRoot(Arena *arena, MD_Node *root)
{
 MG_StrExprParseResult parse = MG_StrExprParseFromFirstOPL_MinPrec(arena, root->first, &md_nil_node, 0);
 return parse;
}

////////////////////////////////
//~ rjf: String Transforms

function String8
MG_CStringLiteralFromMultilineString(String8 string)
{
 String8List strings = {0};
 {
  Str8ListPush(mg_arena, &strings, Str8Lit("\"\"\n"));
  U64 active_line_start_off = 0;
  for(U64 off = 0; off <= string.size; off += 1)
  {
   B32 is_newline = (off < string.size && (string.str[off] == '\n' || string.str[off] == '\r'));
   B32 is_ender = (off >= string.size || is_newline);
   if(is_ender)
   {
    String8 line = Substr8(string, R1U64(active_line_start_off, off));
    Str8ListPush(mg_arena, &strings, Str8Lit("\""));
    Str8ListPush(mg_arena, &strings, line);
    if(is_newline)
    {
     Str8ListPush(mg_arena, &strings, Str8Lit("\\n\"\n"));
    }
    else
    {
     Str8ListPush(mg_arena, &strings, Str8Lit("\"\n"));
    }
    active_line_start_off = off+1;
   }
   if(is_newline && string.str[off] == '\r')
   {
    active_line_start_off += 1;
    off += 1;
   }
  }
 }
 String8 result = Str8ListJoin(mg_arena, strings, 0);
 return result;
}

function String8
MG_CArrayLiteralContentsFromData(String8 data)
{
 Temp scratch = ScratchBegin(0, 0);
 String8List strings = {0};
 {
  for(U64 off = 0; off < data.size;)
  {
   U64 chunk_size = Min(data.size-off, 64);
   U8 *chunk_bytes = data.str+off;
   String8 chunk_text_string = PushStr8FillByte(mg_arena, chunk_size*5, 0);
   for(U64 byte_idx = 0; byte_idx < chunk_size; byte_idx += 1)
   {
    String8 byte_str = PushStr8F(scratch.arena, "0x%02x,", chunk_bytes[byte_idx]);
    MemoryCopy(chunk_text_string.str+byte_idx*5, byte_str.str, byte_str.size);
   }
   off += chunk_size;
   Str8ListPush(mg_arena, &strings, chunk_text_string);
   Str8ListPush(mg_arena, &strings, Str8Lit("\n"));
  }
 }
 String8 result = Str8ListJoin(mg_arena, strings, 0);
 ScratchEnd(scratch);
 return result;
}

function String8
MG_EscapedFromString(Arena *arena, String8 string)
{
 Temp scratch = ScratchBegin(&arena, 1);
 String8List strs = {0};
 B32 escaped = 0;
 U64 start = 0;
 for(U64 idx = 0; idx <= string.size; idx += 1)
 {
  if(idx < string.size && escaped)
  {
   escaped = 0;
   start = idx+1;
   U8 replace_char = 0;
   switch(string.str[idx])
   {
    default: break;
    case 'a':  replace_char = 0x07; break;
    case 'b':  replace_char = 0x08; break;
    case 'e':  replace_char = 0x1b; break;
    case 'f':  replace_char = 0x0c; break;
    case 'n':  replace_char = 0x0a; break;
    case 'r':  replace_char = 0x0d; break;
    case 't':  replace_char = 0x09; break;
    case 'v':  replace_char = 0x0b; break;
    case '\\': replace_char = 0x5c; break;
    case '\'': replace_char = 0x27; break;
    case '\"': replace_char = 0x22; break;
    case '\?': replace_char = 0x3f; break;
   }
   if(replace_char)
   {
    String8 string = PushStr8Copy(scratch.arena, Str8(&replace_char, 1));
    Str8ListPush(scratch.arena, &strs, string);
   }
  }
  else if(idx == string.size || string.str[idx] == '\\')
  {
   escaped = (string.str[idx] == '\\');
   String8 part = Substr8(string, R1U64(start, idx));
   Str8ListPush(scratch.arena, &strs, part);
   start = idx;
  }
 }
 String8 result = Str8ListJoin(arena, strs, 0);
 ScratchEnd(scratch);
 return result;
}

////////////////////////////////
//~ rjf: Layer Lookup Functions

function String8
MG_LayerKeyFromPath(String8 path)
{
 Temp scratch = ScratchBegin(0, 0);
 U64 src_folder_pos = 0;
 for(U64 next_src_folder_pos = 0;
     next_src_folder_pos < path.size;
     next_src_folder_pos = FindSubstr8(path, Str8Lit("code"), next_src_folder_pos+1, 0))
 {
  src_folder_pos = next_src_folder_pos;
 }
 String8List path_parts = PathPartsFromStr8(scratch.arena, Str8PathChopLastSlash(Str8Skip(path, src_folder_pos+5)));
 StringJoin join = {0};
 join.sep = Str8Lit("/");
 String8 key = Str8ListJoin(mg_arena, path_parts, &join);
 ScratchEnd(scratch);
 return key;
}

function MG_Layer *
MG_LayerFromKey(String8 key)
{
 U64 hash = MG_HashFromString(key);
 U64 slot_idx = hash%mg_state->layer_slots_count;
 MG_LayerSlot *slot = &mg_state->layer_slots[slot_idx];
 MG_Layer *layer = 0;
 for(MG_LayerNode *n = slot->first; n != 0; n = n->next)
 {
  if(Str8Match(n->v.key, key, 0))
  {
   layer = &n->v;
   break;
  }
 }
 if(layer == 0)
 {
  MG_LayerNode *n = PushArray(mg_arena, MG_LayerNode, 1);
  QueuePush(slot->first, slot->last, n);
  n->v.key = PushStr8Copy(mg_arena, key);
  layer = &n->v;
 }
 return layer;
}

////////////////////////////////
//~ rjf: Table Insertion/Lookup Functions

function MG_Table *
MG_TableFromName(String8 name)
{
 MG_Table *table = 0;
 {
  U64 hash = MG_HashFromString(name);
  U64 slot_idx = hash%mg_state->table_slots_count;
  MG_TableSlot *slot = &mg_state->table_slots[slot_idx];
  for(MG_TableNode *n = slot->first; n != 0; n = n->next)
  {
   if(Str8Match(n->v.root->string, name, 0))
   {
    table = &n->v;
    break;
   }
  }
 }
 return table;
}

////////////////////////////////
//~ rjf: String Expression Functions

function U64
MG_TableRowChildIndexFromColumnName(MG_TableHeader *header, String8 column_name)
{
 U64 result = 0;
 for(U64 idx = 0; idx < header->column_count; idx += 1)
 {
  if(Str8Match(header->column_descs[idx].name, column_name, 0))
  {
   break;
  }
  if(header->column_descs[idx].kind == MG_ColumnKind_Default)
  {
   result += 1;
  }
 }
 return result;
}

function S64
MG_StrExprEvaluate_Numeric(MG_ExpandInfo *info, MG_StrExpr *expr)
{
 S64 result = 0;
 MG_StrExprOp op = expr->op;
 switch(op)
 {
  case MG_StrExprOp_Equals:
  case MG_StrExprOp_DoesNotEqual:
  {
   Temp scratch = ScratchBegin(0, 0);
   String8List left_strs = {0};
   String8List right_strs = {0};
   MG_StrExprEvaluate_String(info, expr->left, &left_strs);
   MG_StrExprEvaluate_String(info, expr->right, &right_strs);
   String8 left_str = Str8ListJoin(scratch.arena, left_strs, 0);
   String8 right_str = Str8ListJoin(scratch.arena, right_strs, 0);
   result = Str8Match(left_str, right_str, 0);
   if(op == MG_StrExprOp_DoesNotEqual)
   {
    result = !result;
   }
   ScratchEnd(scratch);
  }break;
  
  case MG_StrExprOp_BooleanAnd:
  case MG_StrExprOp_BooleanOr:
  {
   S64 left = MG_StrExprEvaluate_Numeric(info, expr->left);
   S64 right = MG_StrExprEvaluate_Numeric(info, expr->right);
   switch(op)
   {
    case MG_StrExprOp_BooleanAnd: result = left && right; break;
    case MG_StrExprOp_BooleanOr:  result = left || right; break;
   }
  }break;
 }
 return result;
}

function void
MG_StrExprEvaluate_String(MG_ExpandInfo *info, MG_StrExpr *expr, String8List *out)
{
 MG_StrExprOp op = expr->op;
 switch(op)
 {
  default:
  case MG_StrExprOp_Null:
  {
   Str8ListPush(mg_arena, out, expr->node->string);
  }break;
  
  case MG_StrExprOp_Dot:
  {
   MG_StrExpr *label_expr = expr->left;
   MG_StrExpr *column_query_expr = expr->right;
   MD_Node *label_node = label_expr->node;
   MD_Node *column_query_node = column_query_expr->node;
   String8 label = label_node->string;
   String8 column_query = column_query_node->string;
   B32 column_query_is_by_expand_idx = Str8Match(column_query_node->string, Str8Lit("_it"), 0);
   B32 column_query_is_by_name       = !column_query_is_by_expand_idx && column_query_node->flags & MD_NodeFlag_Identifier;
   B32 column_query_is_by_index      = column_query_node->flags & MD_NodeFlag_Numeric;
   
   // rjf: find which expansion this label refers to, grab its iterator
   MG_ExpandIter *iter = 0;
   for(MG_ExpandIter *it = info->first_expand_iter; it != 0; it = it->next)
   {
    if(Str8Match(it->label, label, 0))
    {
     iter = it;
     break;
    }
   }
   
   // rjf: error on invalid label
   if(iter == 0)
   {
#if 0
    // TODO(rjf)
    MD_PrintMessageFmt(stderr, MD_CodeLocFromNode(label_node), MD_MessageKind_Error, "Expansion label \"%S\" was not found as referring to a valid @expand tag.", label);
#endif
   }
   
   // rjf: generate strings from iterator's table
   if(iter != 0)
   {
    MG_Table *table = iter->table;
    MG_NodeGrid *grid = &table->grid;
    MG_TableHeader *header = &table->header;
    MD_Node *row = grid->row_parents.v[iter->idx];
    
    // rjf: grab the cell string given the row & column_query
    String8 cell_string = {0};
    {
     // NOTE(rjf): expansion index counter
     if(column_query_is_by_expand_idx)
     {
      S64 index = iter->idx;
      cell_string = PushStr8F(mg_arena, "%I64u", index);
     }
     
     // NOTE(rjf): by-name index (look into table header)
     else if(column_query_is_by_name && header != 0)
     {
      U64 column_idx = 0;
      MG_ColumnDesc *column = 0;
      for(U64 col_idx = 0; col_idx < header->column_count; col_idx += 1)
      {
       if(Str8Match(header->column_descs[col_idx].name, column_query, 0))
       {
        column = &header->column_descs[col_idx];
        column_idx = col_idx;
        break;
       }
      }
      U64 row_child_idx = MG_TableRowChildIndexFromColumnName(header, column_query);
      
      // rjf: error on invalid column
      if(column == 0)
      {
#if 0
       // TODO(rjf)
       MD_PrintMessageFmt(stderr, MD_CodeLocFromNode(column_query_node), MD_MessageKind_Error, "Column query \"%S\" did not map to a valid column for expansion label \"%S\".", column_query, label);
#endif
      }
      
      if(column != 0)
      {
       switch(column->kind)
       {
        default:
        case MG_ColumnKind_Default:
        {
         MD_Node *cell_node = MD_ChildFromIndex(row, row_child_idx);
         cell_string = cell_node->string;
         if(Str8Match(cell_node->raw_string, Str8Lit("."), 0))
         {
          cell_string = column->default_value;
         }
        }break;
        
        case MG_ColumnKind_CheckForTag:
        {
         B32 has_tag = MD_NodeHasTag(row, column->tag_string, 0);
         cell_string = has_tag ? Str8Lit("1") : Str8Lit("0");
        }break;
       }
      }
     }
     
     // NOTE(rjf): by-index (grab nth child of row)
     else if(column_query_is_by_index)
     {
      S64 index = CStyleIntFromStr8(column_query);
      cell_string = MD_ChildFromIndex(row, index)->string;
     }
    }
    
    Str8ListPush(mg_arena, out, cell_string);
   }
   
  }break;
  
  case MG_StrExprOp_BumpToColumn:
  {
   U64 dst = CStyleIntFromStr8(expr->left->node->string);
   U64 src = out->total_size;
   U64 spaces_to_print = dst - src;
   if(dst > src)
   {
    for(U64 space_idx = 0; space_idx < spaces_to_print; space_idx += 1)
    {
     Str8ListPush(mg_arena, out, Str8Lit(" "));
    }
   }
  }break;
  
  case MG_StrExprOp_ExpandIfTrue:
  {
   S64 check_val = MG_StrExprEvaluate_Numeric(info, expr->left);
   if(check_val)
   {
    MG_StrExprEvaluate_String(info, expr->right, out);
   }
  }break;
  
  case MG_StrExprOp_Concat:
  {
   MG_StrExprEvaluate_String(info, expr->left, out);
   MG_StrExprEvaluate_String(info, expr->right, out);
  }break;
 }
}

function void
MG_StrExprLoopExpansionDimension(MG_ExpandIter *it, MG_ExpandInfo *info, String8List *out)
{
 if(it->next)
 {
  for(U64 idx = 0; idx < it->count; idx += 1)
  {
   it->idx = idx;
   MG_StrExprLoopExpansionDimension(it->next, info, out);
  }
 }
 else
 {
  for(U64 idx = 0; idx < it->count; idx += 1)
  {
   it->idx = idx;
   String8List expansion_strs = {0};
   U64 start_idx = 0;
   for(U64 char_idx = 0; char_idx <= info->strexpr.size; char_idx += 1)
   {
    B32 is_expr_marker = info->strexpr.str[char_idx] == '$';
    
    // rjf: push regular string contents
    if(char_idx == info->strexpr.size || is_expr_marker)
    {
     String8 normal_string_chunk = Substr8(info->strexpr, R1U64(start_idx, char_idx));
     String8 escaped = MG_EscapedFromString(mg_arena, normal_string_chunk);
     Str8ListPush(mg_arena, &expansion_strs, escaped);
    }
    
    // rjf: handle expansion
    if(is_expr_marker)
    {
     // rjf: grab expression string
     String8 expr_string = Str8Skip(info->strexpr, char_idx+1);
     {
      S64 paren_nest = 0;
      for(U64 expr_str_char_idx = 0; expr_str_char_idx < expr_string.size; expr_str_char_idx += 1)
      {
       if(expr_string.str[expr_str_char_idx] == '(')
       {
        paren_nest += 1;
       }
       else if(expr_string.str[expr_str_char_idx] == ')')
       {
        paren_nest -= 1;
        if(paren_nest == 0)
        {
         expr_string.size = expr_str_char_idx+1;
         break;
        }
       }
      }
     }
     
     // rjf: parse expression
     MD_TokenizeResult expr_tokenize = MD_TokenizeFromText(mg_arena, expr_string);
     MD_ParseResult expr_base_parse = MD_ParseFromTextTokens(mg_arena, Str8Lit(""), expr_string, expr_tokenize.tokens);
     MG_StrExprParseResult expr_parse = MG_StrExprParseFromRoot(mg_arena, expr_base_parse.root);
     MG_StrExprEvaluate_String(info, expr_parse.root, &expansion_strs);
     start_idx = char_idx+1+expr_string.size;
    }
   }
   
   // rjf: push expansion string to output list
   String8 expansion_str = Str8ListJoin(mg_arena, expansion_strs, 0);
   Str8ListPush(mg_arena, out, expansion_str);
  }
 }
}

function String8List
MG_GenStringListFromNode(MD_Node *gen)
{
 String8List result = {0};
 Temp scratch = ScratchBegin(0, 0);
 for(MD_EachNode(strexpr, gen->first))
 {
  //- rjf: build expansion iterator list
  MG_ExpandIter *first_iter = 0;
  MG_ExpandIter *last_iter = 0;
  {
   for(MD_EachNode(tag, strexpr->first_tag))
   {
    if(Str8Match(tag->string, Str8Lit("expand"), 0))
    {
     MD_Node *table_name_node = MD_ChildFromIndex(tag, 0);
     MD_Node *label_node = MD_ChildFromIndex(tag, 1);
     String8 table_name = table_name_node->string;
     String8 label = label_node->string;
     MG_Table *table = MG_TableFromName(table_name);
     
     // rjf: make iterator node if we got a grid
     if(table != 0)
     {
      MG_ExpandIter *iter = PushArray(scratch.arena, MG_ExpandIter, 1);
      QueuePush(first_iter, last_iter, iter);
      iter->table  = table;
      iter->label  = label;
      iter->count  = table->grid.row_parents.count;
     }
     
     // rjf: print out an error if table is 0
     if(table == 0)
     {
#if 0
      // TODO(rjf)
      MD_PrintMessageFmt(stderr, MD_CodeLocFromNode(tag), MD_MessageKind_Error, "Table \"%S\" was not found.", table_name);
#endif
     }
     
    }
   }
  }
  
  //- rjf: generate string list for this strexpr & push to result
  if(first_iter != 0)
  {
   MG_ExpandInfo info = {0};
   {
    info.strexpr = strexpr->string;
    info.first_expand_iter = first_iter;
   }
   MG_StrExprLoopExpansionDimension(first_iter, &info, &result);
  }
  //- rjf: generate non-expansion strings
  else
  {
   String8 escaped = MG_EscapedFromString(mg_arena, strexpr->string);
   Str8ListPush(mg_arena, &result, escaped);
  }
 }
 ScratchEnd(scratch);
 return result;
}
