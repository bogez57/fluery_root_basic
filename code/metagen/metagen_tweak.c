function void
MG_TWK_Generate(MG_CFileList *c_files)
{
 //- rjf: set up layer map
 U64 layer_slots = 256;
 MG_TWK_Layer **layers = PushArray(mg_arena, MG_TWK_Layer*, layer_slots);
 
 //- rjf: do all generations for all C files
 for(MG_CFile *c_file = c_files->first;
     c_file != 0;
     c_file = c_file->next)
 {
  //- rjf: unpack
  String8 path = c_file->path;
  String8 layer_name = MG_LayerNameFromPath(Str8PathChopLastSlash(path));
  String8 data = c_file->data;
  CL_TokenArray tokens = c_file->tokens;
  CL_Token *tokens_first = tokens.v;
  CL_Token *tokens_opl = tokens_first+tokens.count;
  
  //- rjf: find all tweak tokens
  CL_TokenPtrList tweak_tokens = {0};
  {
   for(CL_Token *token = tokens_first; token < tokens_opl; token += 1)
   {
    String8 token_string = Substr8(data, token->range);
    if(token->kind == CL_TokenKind_Identifier &&
       (Str8Match(token_string, Str8Lit("TweakB32"), 0) ||
        Str8Match(token_string, Str8Lit("TweakF32"), 0)))
    {
     CL_TokenPtrListPush(mg_arena, &tweak_tokens, token);
    }
   }
  }
  
  //- rjf: map -> layer
  MG_TWK_Layer *layer = 0;
  if(tweak_tokens.count != 0)
  {
   U64 layer_name_hash = MG_HashFromString(layer_name);
   MG_TWK_Layer **slot = &layers[layer_name_hash%layer_slots];
   for(MG_TWK_Layer *n = *slot; n != 0; n = n->next)
   {
    if(Str8Match(n->name, layer_name, 0))
    {
     layer = n;
     break;
    }
   }
   if(layer == 0)
   {
    layer = PushArray(mg_arena, MG_TWK_Layer, 1);
    layer->name = layer_name;
    layer->path = Str8PathChopLastSlash(path);
    StackPush(*slot, layer);
   }
  }
  
  //- rjf: parse all tweaks
  {
   for(CL_TokenPtrNode *tweak_node = tweak_tokens.first;
       tweak_node != 0;
       tweak_node = tweak_node->next)
   {
    CL_Token *tweak_first = tweak_node->v;
    
    // rjf: extract argument list tokens
    CL_TokenPtrList args_tokens = {0};
    {
     S32 paren_nest = 0;
     B32 done = 0;
     for(CL_Token *token = tweak_first; token < tokens_opl && !done; token += 1)
     {
      String8 token_string = Substr8(data, token->range);
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
      String8 token_string = Substr8(data, token->range);
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
     tweak_name = Substr8(data, parts.first->v->range);
    }
    
    // rjf: 2nd argument -> default
    String8 tweak_default = {0};
    if(args.first != 0 &&
       args.first->next != 0 && args.first->next->v.count != 0)
    {
     CL_TokenPtrList parts = args.first->next->v;
     tweak_default = Substr8(data, Union1U64(parts.first->v->range, parts.last->v->range));
    }
    
    // rjf: 3nd argument -> range min
    String8 tweak_min = {0};
    if(args.first != 0 &&
       args.first->next != 0 &&
       args.first->next->next != 0 && args.first->next->next->v.count != 0)
    {
     CL_TokenPtrList parts = args.first->next->next->v;
     tweak_min = Substr8(data, Union1U64(parts.first->v->range, parts.last->v->range));
    }
    
    // rjf: 4th argument -> range max
    String8 tweak_max = {0};
    if(args.first != 0 &&
       args.first->next != 0 &&
       args.first->next->next != 0 &&
       args.first->next->next->next != 0 && args.first->next->next->next->v.count != 0)
    {
     CL_TokenPtrList parts = args.first->next->next->next->v;
     tweak_max = Substr8(data, Union1U64(parts.first->v->range, parts.last->v->range));
    }
    
    // rjf: kind -> list
    String8 tweak_kind = Substr8(data, tweak_first->range);
    MG_TWK_TweakList *kind_list = &layer->tweak_b32s;
    if(Str8Match(tweak_kind, Str8Lit("TweakF32"), 0)) { kind_list = &layer->tweak_f32s; }
    
    // rjf: push
    MG_TWK_Tweak *tweak = PushArray(mg_arena, MG_TWK_Tweak, 1);
    tweak->kind = tweak_kind;
    tweak->name = tweak_name;
    tweak->default_val = tweak_default;
    tweak->min = tweak_min;
    tweak->max = tweak_max;
    QueuePush(kind_list->first, kind_list->last, tweak);
    kind_list->count += 1;
   }
  }
 }
 
 //- rjf: generate all tweak text
 for(U64 slot_idx = 0; slot_idx < layer_slots; slot_idx += 1)
 {
  for(MG_TWK_Layer *layer = layers[slot_idx]; layer != 0; layer = layer->next)
  {
   MG_Bucket *bucket = MG_BucketFromLayerPath(layer->path);
   
   //- rjf: generate TweakB32 states
   for(MG_TWK_Tweak *tweak = layer->tweak_b32s.first; tweak != 0; tweak = tweak->next)
   {
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern B32 TWEAK_%S;\n", tweak->name);
    Str8ListPushF(mg_arena, &bucket->c_tables, "B32 TWEAK_%S = %S;\n", tweak->name, tweak->default_val);
   }
   
   //- rjf: generate TweakF32 states
   for(MG_TWK_Tweak *tweak = layer->tweak_f32s.first; tweak != 0; tweak = tweak->next)
   {
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern F32 TWEAK_%S;\n", tweak->name);
    Str8ListPushF(mg_arena, &bucket->c_tables, "F32 TWEAK_%S = %S;\n", tweak->name, tweak->default_val);
   }
   
   //- rjf: generate TweakB32 table
   if(layer->tweak_b32s.count != 0)
   {
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern TweakB32Info %S_tweak_b32_table[%I64u];\n", layer->name, layer->tweak_b32s.count);
    Str8ListPushF(mg_arena, &bucket->c_tables, "TweakB32Info %S_tweak_b32_table[%I64u] =\n", layer->name, layer->tweak_b32s.count);
    Str8ListPushF(mg_arena, &bucket->c_tables, "{\n");
    for(MG_TWK_Tweak *tweak = layer->tweak_b32s.first; tweak != 0; tweak = tweak->next)
    {
     Str8ListPushF(mg_arena, &bucket->c_tables, "{ Str8LitComp(\"%S\"), %S, &TWEAK_%S },\n", tweak->name, tweak->default_val, tweak->name);
    }
    Str8ListPushF(mg_arena, &bucket->c_tables, "};\n\n");
   }
   
   //- rjf: generate TweakF32 table
   if(layer->tweak_f32s.count != 0)
   {
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern TweakF32Info %S_tweak_f32_table[%I64u];\n", layer->name, layer->tweak_f32s.count);
    Str8ListPushF(mg_arena, &bucket->c_tables, "TweakF32Info %S_tweak_f32_table[%I64u] =\n", layer->name, layer->tweak_f32s.count);
    Str8ListPushF(mg_arena, &bucket->c_tables, "{\n");
    for(MG_TWK_Tweak *tweak = layer->tweak_f32s.first; tweak != 0; tweak = tweak->next)
    {
     Str8ListPushF(mg_arena, &bucket->c_tables, "{ Str8LitComp(\"%S\"), %S, {%S, %S}, &TWEAK_%S },\n", tweak->name, tweak->default_val, tweak->min, tweak->max, tweak->name);
    }
    Str8ListPushF(mg_arena, &bucket->c_tables, "};\n\n");
   }
   
  }
 }
}
