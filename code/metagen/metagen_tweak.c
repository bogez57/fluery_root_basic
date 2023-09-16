function U64
MG_TWK_HashFromString(String8 string)
{
 U64 result = 5381;
 for(U64 i = 0; i < string.size; i += 1)
 {
  result = ((result << 5) + result) + string.str[i];
 }
 return result;
}

function void
MG_TWK_Generate(MG_CFileList *c_files)
{
 U64 layer_slots = 256;
 MG_TWK_Layer **layers = PushArray(mg_arena, MG_TWK_Layer*, layer_slots);
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
  
  //- rjf: find all TweakB32 tokens
  CL_TokenPtrList tweak_b32_tokens = {0};
  {
   for(CL_Token *token = tokens_first; token < tokens_opl; token += 1)
   {
    String8 token_string = Substr8(data, token->range);
    if(token->kind == CL_TokenKind_Identifier && Str8Match(token_string, Str8Lit("TweakB32"), 0))
    {
     CL_TokenPtrListPush(mg_arena, &tweak_b32_tokens, token);
    }
   }
  }
  
  //- rjf: map -> layer
  MG_TWK_Layer *layer = 0;
  if(tweak_b32_tokens.count != 0)
  {
   U64 layer_name_hash = MG_TWK_HashFromString(layer_name);
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
  
  //- rjf: parse all TweakB32's
  {
   for(CL_TokenPtrNode *tweak_node = tweak_b32_tokens.first;
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
    MG_TWK_Arg *first_arg = 0;
    MG_TWK_Arg *last_arg = 0;
    for(CL_TokenPtrNode *n = args_tokens.first; n != 0; n = n->next)
    {
     MG_TWK_Arg *arg = last_arg;
     CL_Token *token = n->v;
     String8 token_string = Substr8(data, token->range);
     B32 is_comma = (token->kind == CL_TokenKind_Symbol && Str8Match(token_string, Str8Lit(","), 0));
     if(arg == 0 || is_comma)
     {
      arg = PushArray(mg_arena, MG_TWK_Arg, 1);
      QueuePush(first_arg, last_arg, arg);
     }
     if(!is_comma)
     {
      CL_TokenPtrListPush(mg_arena, &arg->tokens, token);
     }
    }
    
    // rjf: first argument -> name of the tweak
    String8 tweak_name = {0};
    if(first_arg != 0 && first_arg->tokens.count != 0)
    {
     MG_TWK_Arg *arg = first_arg;
     tweak_name = Substr8(data, arg->tokens.first->v->range);
    }
    
    // rjf: 2nd argument -> default
    String8 tweak_default = {0};
    if(first_arg != 0 && first_arg->next != 0 && first_arg->tokens.count != 0)
    {
     MG_TWK_Arg *arg = first_arg->next;
     tweak_default = Substr8(data, arg->tokens.first->v->range);
    }
    
    // rjf: push
    MG_TWK_Tweak *tweak = PushArray(mg_arena, MG_TWK_Tweak, 1);
    tweak->name = tweak_name;
    tweak->default_val = tweak_default;
    QueuePush(layer->first_tweak, layer->last_tweak, tweak);
   }
  }
 }
 
 //- rjf: generate all tweak tables
 for(U64 slot_idx = 0; slot_idx < layer_slots; slot_idx += 1)
 {
  for(MG_TWK_Layer *layer = layers[slot_idx]; layer != 0; layer = layer->next)
  {
   MG_FilePair f = MG_FilePairFromLayerPath(layer->path);
   
   //- rjf: generate TweakB32 states
   for(MG_TWK_Tweak *tweak = layer->first_tweak; tweak != 0; tweak = tweak->next)
   {
    fprintf(f.h, "global B32 TWEAK_%.*s = %.*s;\n", Str8VArg(tweak->name), Str8VArg(tweak->default_val));
   }
   
   //- rjf: generate TweakB32 table
   if(layer->first_tweak != 0)
   {
    fprintf(f.h, "global TweakB32Info %.*s_tweak_b32_table[] =\n", Str8VArg(layer->name));
    fprintf(f.h, "{\n");
    for(MG_TWK_Tweak *tweak = layer->first_tweak; tweak != 0; tweak = tweak->next)
    {
     fprintf(f.h, "{ Str8LitComp(\"%.*s\"), %.*s, &TWEAK_%.*s },\n", Str8VArg(tweak->name), Str8VArg(tweak->default_val), Str8VArg(tweak->name));
    }
    fprintf(f.h, "};\n\n");
   }
  }
 }
}
