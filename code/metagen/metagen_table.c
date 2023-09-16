global MD_Map mg_tbl_top_level_node_grid_map    = {0};
global MD_Map mg_tbl_top_level_table_header_map = {0};

global MD_Map mg_tbl_layer_map_gen = {0};
global MD_Map mg_tbl_layer_map_gen_enum = {0};
global MD_Map mg_tbl_layer_map_gen_data = {0};

global MD_String8 mg_tbl_tag__table          = MD_S8LitComp("table");
global MD_String8 mg_tbl_tag__table_gen      = MD_S8LitComp("table_gen");
global MD_String8 mg_tbl_tag__table_gen_enum = MD_S8LitComp("table_gen_enum");
global MD_String8 mg_tbl_tag__table_gen_data = MD_S8LitComp("table_gen_data");
global MD_String8 mg_tbl_tag__core           = MD_S8LitComp("core");

function MG_NodeArray
MG_NodeArrayMake(U64 count)
{
 MG_NodeArray result = {0};
 result.count = count;
 result.v = PushArray(mg_arena, MD_Node *, result.count);
 for(U64 idx = 0; idx < result.count; idx += 1)
 {
  result.v[idx] = MD_NilNode();
 }
 return result;
}

function MG_NodeGrid
MG_GridFromNode(MD_Node *node)
{
 MG_NodeGrid grid = {0};
 
 //- rjf: determine dimensions
 U64 row_count = 0;
 U64 column_count = 0;
 {
  for(MD_EachNode(row, node->first_child))
  {
   row_count += 1;
   U64 cell_count_this_row = MD_ChildCountFromNode(row);
   column_count = MD_Max(cell_count_this_row, column_count);
  }
 }
 
 //- rjf: allocate cells / row parents
 {
  grid.cells = MG_NodeArrayMake(row_count * column_count);
  grid.row_parents = MG_NodeArrayMake(row_count);
 }
 
 //- rjf: fill cells
 {
  U64 row_idx = 0;
  for(MD_EachNode(row, node->first_child))
  {
   U64 col_idx = 0;
   grid.row_parents.v[row_idx] = row;
   for(MD_EachNode(cell, row->first_child))
   {
    grid.cells.v[row_idx * column_count + col_idx] = cell;
    col_idx += 1;
   }
   row_idx += 1;
  }
 }
 
 return grid;
}

function MG_TableHeader
MG_TableHeaderFromTag(MD_Node *tag)
{
 MG_TableHeader result = {0};
 result.column_count = MD_ChildCountFromNode(tag);
 result.column_descs = PushArray(mg_arena, MG_ColumnDesc, result.column_count);
 U64 idx = 0;
 for(MD_EachNode(column_node, tag->first_child))
 {
  result.column_descs[idx].kind = MG_ColumnKind_Default;
  result.column_descs[idx].name = Str8FromMD(column_node->string);
  MD_Node *check_for_tag = MD_TagFromString(column_node, MD_S8Lit("check_for_tag"), 0);
  if(!MD_NodeIsNil(check_for_tag))
  {
   result.column_descs[idx].kind = MG_ColumnKind_CheckForTag;
   result.column_descs[idx].tag_string = Str8FromMD(check_for_tag->first_child->string);
  }
  MD_Node *default_value = MD_TagFromString(column_node, MD_S8Lit("default"), 0);
  if(!MD_NodeIsNil(default_value))
  {
   result.column_descs[idx].default_value = Str8FromMD(default_value->first_child->string);
  }
  idx += 1;
 }
 return result;
}

function U64
MG_RowChildIndexFromColumnName(MG_TableHeader *header, String8 column_name)
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
MG_TableExprEvaluate_Numeric(MG_ExpandInfo *info, MD_Expr *expr)
{
 S64 result = 0;
 MG_TableOp op = expr->op ? expr->op->op_id : MG_TableOp_Null;
 switch(op)
 {
  case MG_TableOp_Equal:
  case MG_TableOp_IsNotEqual:
  {
   Temp scratch = ScratchBegin(0, 0);
   String8List left_strs = {0};
   String8List right_strs = {0};
   MG_TableExprEvaluate_String(info, expr->left, &left_strs);
   MG_TableExprEvaluate_String(info, expr->right, &right_strs);
   String8 left_str = Str8ListJoin(scratch.arena, left_strs, 0);
   String8 right_str = Str8ListJoin(scratch.arena, right_strs, 0);
   result = Str8Match(left_str, right_str, 0);
   if(op == MG_TableOp_IsNotEqual)
   {
    result = !result;
   }
   ScratchEnd(scratch);
  }break;
  
  case MG_TableOp_BooleanAnd:
  case MG_TableOp_BooleanOr:
  {
   S64 left = MG_TableExprEvaluate_Numeric(info, expr->left);
   S64 right = MG_TableExprEvaluate_Numeric(info, expr->right);
   switch(op)
   {
    case MG_TableOp_BooleanAnd: result = left && right; break;
    case MG_TableOp_BooleanOr:  result = left || right; break;
   }
  }break;
 }
 return result;
}

function void
MG_TableExprEvaluate_String(MG_ExpandInfo *info, MD_Expr *expr, String8List *out)
{
 MG_TableOp op = expr->op ? expr->op->op_id : MG_TableOp_Null;
 switch(op)
 {
  default:
  case MG_TableOp_Null:
  {
   Str8ListPush(mg_arena, out, Str8FromMD(expr->md_node->string));
  }break;
  
  case MG_TableOp_Dot:
  {
   MD_Expr *label_expr = expr->left;
   MD_Expr *column_query_expr = expr->right;
   MD_Node *label_node = label_expr->md_node;
   MD_Node *column_query_node = column_query_expr->md_node;
   String8 label = Str8FromMD(label_node->string);
   String8 column_query = Str8FromMD(column_query_node->string);
   B32 column_query_is_by_expand_idx = Str8Match(Str8FromMD(column_query_node->string), Str8Lit("_it"), 0);
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
    MD_PrintMessageFmt(stderr, MD_CodeLocFromNode(label_node), MD_MessageKind_Error, "Expansion label \"%S\" was not found as referring to a valid @expand tag.", label);
   }
   
   // rjf: generate strings from iterator's table
   if(iter != 0)
   {
    MG_NodeGrid *grid = iter->grid;
    MG_TableHeader *header = iter->header;
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
      U64 row_child_idx = MG_RowChildIndexFromColumnName(header, column_query);
      
      // rjf: error on invalid column
      if(column == 0)
      {
       MD_PrintMessageFmt(stderr, MD_CodeLocFromNode(column_query_node), MD_MessageKind_Error, "Column query \"%S\" did not map to a valid column for expansion label \"%S\".", column_query, label);
      }
      
      if(column != 0)
      {
       switch(column->kind)
       {
        default:
        case MG_ColumnKind_Default:
        {
         MD_Node *cell_node = MD_ChildFromIndex(row, row_child_idx);
         cell_string = Str8FromMD(cell_node->string);
         if(Str8Match(Str8FromMD(cell_node->raw_string), Str8Lit("."), 0))
         {
          cell_string = column->default_value;
         }
        }break;
        
        case MG_ColumnKind_CheckForTag:
        {
         B32 has_tag = MD_NodeHasTag(row, MDFromStr8(column->tag_string), 0);
         cell_string = has_tag ? Str8Lit("1") : Str8Lit("0");
        }break;
       }
      }
     }
     
     // NOTE(rjf): by-index (grab nth child of row)
     else if(column_query_is_by_index)
     {
      S64 index = CStyleIntFromStr8(column_query);
      cell_string = Str8FromMD(MD_ChildFromIndex(row, index)->string);
     }
    }
    
    Str8ListPush(mg_arena, out, cell_string);
   }
   
  }break;
  
  case MG_TableOp_Bump:
  {
   U64 dst = MD_CStyleIntFromString(expr->unary_operand->md_node->string);
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
  
  case MG_TableOp_CheckIfTrue:
  {
   S64 check_val = MG_TableExprEvaluate_Numeric(info, expr->left);
   if(check_val)
   {
    MG_TableExprEvaluate_String(info, expr->right, out);
   }
  }break;
  
  case MG_TableOp_Concat:
  {
   MG_TableExprEvaluate_String(info, expr->left, out);
   MG_TableExprEvaluate_String(info, expr->right, out);
  }break;
 }
}

function void
MG_LoopExpansionDimension(MG_ExpandIter *it, MG_ExpandInfo *info, String8List *out)
{
 if(it->next)
 {
  for(U64 idx = 0; idx < it->count; idx += 1)
  {
   it->idx = idx;
   MG_LoopExpansionDimension(it->next, info, out);
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
     MD_ParseResult parse = MD_ParseOneNode(mg_arena, MDFromStr8(expr_string), 0);
     MD_Node *node = parse.node;
     MD_ExprParseResult expr_parse = MD_ExprParse(mg_arena, &info->expr_op_table, node->first_child, MD_NilNode());
     MD_Expr *expr = expr_parse.expr;
     MG_TableExprEvaluate_String(info, expr, &expansion_strs);
     String8 parsed_string = Substr8(info->strexpr, R1U64(char_idx+1, char_idx+1+parse.string_advance));
     parsed_string = Str8ChopWhitespace(parsed_string);
     start_idx = char_idx+1+parsed_string.size;
    }
   }
   
   // rjf: push expansion string to output list
   String8 expansion_str = Str8ListJoin(mg_arena, expansion_strs, 0);
   Str8ListPush(mg_arena, out, expansion_str);
  }
 }
}

function String8List
MG_GenStringListFromNode(MD_ExprOprTable expr_op_table, MD_Node *gen)
{
 String8List result = {0};
 Temp scratch = ScratchBegin(0, 0);
 for(MD_EachNode(strexpr, gen->first_child))
 {
  //- rjf: build expansion iterator list
  MG_ExpandIter *first_iter = 0;
  MG_ExpandIter *last_iter = 0;
  {
   for(MD_EachNode(tag, strexpr->first_tag))
   {
    if(Str8Match(Str8FromMD(tag->string), Str8Lit("expand"), 0))
    {
     MD_Node *table_name_node = MD_ChildFromIndex(tag, 0);
     MD_Node *label_node = MD_ChildFromIndex(tag, 1);
     MD_String8 table_name = table_name_node->string;
     MD_String8 label = label_node->string;
     
     // rjf: grab the table associated with table_name
     MG_NodeGrid *grid = 0;
     {
      MD_MapSlot *slot = MD_MapLookup(&mg_tbl_top_level_node_grid_map, MD_MapKeyStr(table_name));
      if(slot != 0)
      {
       grid = slot->val;
      }
     }
     
     // rjf: grab the table header associated with table_name
     MG_TableHeader *header = 0;
     {
      MD_MapSlot *slot = MD_MapLookup(&mg_tbl_top_level_table_header_map, MD_MapKeyStr(table_name));
      if(slot != 0)
      {
       header = slot->val;
      }
     }
     
     // rjf: make iterator node if we got a grid
     if(grid != 0)
     {
      MG_ExpandIter *iter = MD_PushArrayZero(scratch.arena, MG_ExpandIter, 1);
      MD_QueuePush(first_iter, last_iter, iter);
      iter->grid   = grid;
      iter->header = header;
      iter->label  = Str8FromMD(label);
      iter->count  = grid->row_parents.count;
     }
     
     // rjf: print out an error if grid is 0
     if(grid == 0)
     {
      MD_PrintMessageFmt(stderr, MD_CodeLocFromNode(tag), MD_MessageKind_Error, "Table \"%S\" was not found.", table_name);
     }
     
    }
   }
  }
  
  //- rjf: generate string list for this strexpr & push to result
  if(first_iter != 0)
  {
   MG_ExpandInfo info = {0};
   {
    info.strexpr = Str8FromMD(strexpr->string);
    info.first_expand_iter = first_iter;
    info.expr_op_table = expr_op_table;
   }
   MG_LoopExpansionDimension(first_iter, &info, &result);
  }
  //- rjf: generate non-expansion strings
  else
  {
   String8 escaped = MG_EscapedFromString(mg_arena, Str8FromMD(strexpr->string));
   Str8ListPush(mg_arena, &result, escaped);
  }
 }
 ScratchEnd(scratch);
 return result;
}

function void
MG_TBL_Generate(MD_Node *file_list)
{
 //- rjf: initialize all maps
 mg_tbl_top_level_node_grid_map    = MD_MapMake(mg_arena);
 mg_tbl_top_level_table_header_map = MD_MapMake(mg_arena);
 mg_tbl_layer_map_gen              = MD_MapMake(mg_arena);
 mg_tbl_layer_map_gen_enum         = MD_MapMake(mg_arena);
 mg_tbl_layer_map_gen_data         = MD_MapMake(mg_arena);
 
 //- rjf: build table expression operator table
 MD_ExprOprTable table_expr_op_table = {0};
 {
  MD_ExprOprList ops_list = {0};
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Binary, 10, MD_S8Lit("."),  MG_TableOp_Dot, 0);
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Prefix, 9,  MD_S8Lit("=>"), MG_TableOp_Bump, 0);
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Binary, 6,  MD_S8Lit("??"), MG_TableOp_CheckIfTrue, 0);
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Binary, 7,  MD_S8Lit(".."), MG_TableOp_Concat, 0);
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Binary, 8,  MD_S8Lit("=="), MG_TableOp_Equal, 0);
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Binary, 8,  MD_S8Lit("!="), MG_TableOp_IsNotEqual, 0);
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Binary, 5,  MD_S8Lit("&&"), MG_TableOp_BooleanAnd, 0);
  MD_ExprOprPush(mg_arena, &ops_list, MD_ExprOprKind_Binary, 4,  MD_S8Lit("||"), MG_TableOp_BooleanOr, 0);
  table_expr_op_table = MD_ExprBakeOprTableFromList(mg_arena, &ops_list);
 }
 
 //- rjf: gather phase
 for(MD_EachNode(file_ref, file_list->first_child))
 {
  MD_Node *file = MD_ResolveNodeFromReference(file_ref);
  MD_String8 layer_name = file->string;
  MD_MapKey layer_key = MD_MapKeyStr(layer_name);
  for(MD_EachNode(node, file->first_child))
  {
   MD_Node *table_tag = MD_TagFromString(node, mg_tbl_tag__table, 0);
   if(!MD_NodeIsNil(table_tag))
   {
    MG_NodeGrid *grid = MD_PushArrayZero(mg_arena, MG_NodeGrid, 1);
    *grid = MG_GridFromNode(node);
    MD_MapOverwrite(mg_arena, &mg_tbl_top_level_node_grid_map, MD_MapKeyStr(node->string), grid);
    MG_TableHeader *header = MD_PushArrayZero(mg_arena, MG_TableHeader, 1);
    *header = MG_TableHeaderFromTag(table_tag);
    MD_MapOverwrite(mg_arena, &mg_tbl_top_level_table_header_map, MD_MapKeyStr(node->string), header);
   }
   if(MD_NodeHasTag(node, mg_tbl_tag__table_gen, 0))
   {
    MD_MapInsert(mg_arena, &mg_tbl_layer_map_gen, layer_key, node);
   }
   if(MD_NodeHasTag(node, mg_tbl_tag__table_gen_enum, 0))
   {
    MD_MapInsert(mg_arena, &mg_tbl_layer_map_gen_enum, layer_key, node);
   }
   if(MD_NodeHasTag(node, mg_tbl_tag__table_gen_data, 0))
   {
    MD_MapInsert(mg_arena, &mg_tbl_layer_map_gen_data, layer_key, node);
   }
  }
 }
 
 //- rjf: generation phase
 for(MD_EachNode(file_ref, file_list->first_child))
 {
  MD_Node *file = MD_ResolveNodeFromReference(file_ref);
  MD_String8 layer_name = file->string;
  MD_MapKey layer_key = MD_MapKeyStr(layer_name);
  
  //- rjf: generate all table enums
  for(MD_MapSlot *slot = MD_MapLookup(&mg_tbl_layer_map_gen_enum, layer_key);
      slot != 0;
      slot = MD_MapScan(slot->next, layer_key))
  {
   MD_Node *gen = (MD_Node *)slot->val;
   String8 layer_path = MG_LayerPathFromMDNode(gen);
   MG_Bucket *bucket = MG_BucketFromLayerPath(layer_path);
   String8List gen_strings = MG_GenStringListFromNode(table_expr_op_table, gen);
   StringJoin join = { Str8Lit(""), Str8Lit("\n"), Str8Lit("") };
   String8 gen_string = Str8ListJoin(mg_arena, gen_strings, &join);
   Str8ListPushF(mg_arena, &bucket->enums, "typedef enum %S\n{\n", gen->string);
   Str8ListPush (mg_arena, &bucket->enums, gen_string);
   Str8ListPushF(mg_arena, &bucket->enums, "\n}\n%S;\n\n", gen->string);
  }
  
  //- rjf: generate all data tables
  for(MD_MapSlot *slot = MD_MapLookup(&mg_tbl_layer_map_gen_data, layer_key);
      slot != 0;
      slot = MD_MapScan(slot->next, layer_key))
  {
   MD_Node *gen = (MD_Node *)slot->val;
   String8 layer_path = MG_LayerPathFromMDNode(gen);
   MG_Bucket *bucket = MG_BucketFromLayerPath(layer_path);
   MD_Node *tag = MD_TagFromString(gen, mg_tbl_tag__table_gen_data, 0);
   MD_Node *data_table_type_node = tag->first_child;
   String8 data_table_type = Str8FromMD(data_table_type_node->string);
   String8List gen_strings = MG_GenStringListFromNode(table_expr_op_table, gen);
   B32 is_core = MD_NodeHasTag(gen, mg_tbl_tag__core, MD_StringMatchFlag_CaseInsensitive);
   String8 h_decl_specifier = is_core ? Str8Lit("root_global") : Str8Lit("extern");
   StringJoin join = { Str8Lit(""), Str8Lit("\n"), Str8Lit("") };
   String8 gen_string = Str8ListJoin(mg_arena, gen_strings, &join);
   Str8ListPushF(mg_arena, &bucket->h_tables, "%S %S %S[%I64u];\n\n", h_decl_specifier, data_table_type, gen->string, gen_strings.node_count);
   if(is_core)
   {
    Str8ListPushF(mg_arena, &bucket->c_tables, "#if BUILD_CORE\n");
   }
   Str8ListPushF(mg_arena, &bucket->c_tables, "%S %S[%I64u] =\n{\n", data_table_type, gen->string, gen_strings.node_count);
   Str8ListPush(mg_arena, &bucket->c_tables, gen_string);
   Str8ListPush(mg_arena, &bucket->c_tables, Str8Lit("\n};\n"));
   if(is_core)
   {
    Str8ListPushF(mg_arena, &bucket->c_tables, "#endif // BUILD_CORE\n");
   }
   Str8ListPush(mg_arena, &bucket->c_tables, Str8Lit("\n"));
  }
  
  //- rjf: generate all general generations
  for(MD_MapSlot *slot = MD_MapLookup(&mg_tbl_layer_map_gen, layer_key);
      slot != 0;
      slot = MD_MapScan(slot->next, layer_key))
  {
   MD_Node *gen = (MD_Node *)slot->val;
   String8 layer_path = MG_LayerPathFromMDNode(gen);
   MG_Bucket *bucket = MG_BucketFromLayerPath(layer_path);
   String8List gen_strings = MG_GenStringListFromNode(table_expr_op_table, gen);
   StringJoin join = { Str8Lit(""), Str8Lit("\n"), Str8Lit("") };
   String8 gen_string = Str8ListJoin(mg_arena, gen_strings, &join);
   B32 c_file = MD_NodeHasTag(gen, MD_S8Lit("c"), 0);
   String8List *out = c_file ? &bucket->c_functions : &bucket->h_functions;
   Str8ListPush(mg_arena, out, gen_string);
   Str8ListPush(mg_arena, out, Str8Lit("\n\n"));
  }
 }
}
