////////////////////////////////
//~ rjf: Helpers

function String8
Str8FromMD(MD_String8 string)
{
 String8 result = Str8(string.str, string.size);
 return result;
}

function MD_String8
MDFromStr8(String8 string)
{
 MD_String8 result = MD_S8(string.str, string.size);
 return result;
}

function String8
MG_LayerNameFromPath(String8 layer_path)
{
 String8 splits[] = {Str8Lit("/"), Str8Lit("\\")};
 String8List parts = StrSplit8(mg_arena, layer_path, ArrayCount(splits), splits);
 String8List real_parts = {0};
 B32 is_relative_part = 0;
 for(String8Node *n = parts.first; n != 0; n = n->next)
 {
  if(is_relative_part)
  {
   Str8ListPush(mg_arena, &real_parts, n->string);
  }
  else if(Str8Match(n->string, Str8Lit("code"), 0))
  {
   is_relative_part = 1;
  }
 }
 StringJoin join = {0};
 join.sep = Str8Lit("_");
 String8 name = Str8ListJoin(mg_arena, real_parts, &join);
 return name;
}

function MG_FilePair
MG_FilePairFromLayerPath(String8 layer_path)
{
 MG_FilePair result = {0};
 B32 found = 0;
 for(U64 i = 0; i < mg_file_pair_count; i += 1)
 {
  if(Str8Match(layer_path, mg_file_pairs[i].layer_path, 0))
  {
   result = mg_file_pairs[i];
   found = 1;
   break;
  }
 }
 if(found == 0)
 {
  String8 layer_name = MG_LayerNameFromPath(layer_path);
  String8 gen_folder = PushStr8F(mg_arena, "%S/generated", layer_path);
  String8 h_filename = PushStr8F(mg_arena, "%S/%S.meta.h", gen_folder, layer_name);
  String8 c_filename = PushStr8F(mg_arena, "%S/%S.meta.c", gen_folder, layer_name);
  result.layer_path = layer_path;
  result.h = fopen((char *)h_filename.str, "w");
  result.c = fopen((char *)c_filename.str, "w");
  if(result.h == 0)
  {
   int x = 0;
  }
  mg_file_pairs[mg_file_pair_count] = result;
  mg_file_pair_count += 1;
 }
 return result;
}

function MG_FilePair
MG_FilePairFromNode(MD_Node *node)
{
 MD_CodeLoc loc = MD_CodeLocFromNode(node);
 String8 filename = Str8FromMD(loc.filename);
 String8 layer_path = Str8PathChopLastSlash(filename);
 MG_FilePair result = MG_FilePairFromLayerPath(layer_path);
 return result;
}

function MG_FilePair
MG_FilePairFromCFile(MG_CFile *file)
{
 String8 path = file->path;
 String8 layer_path = Str8PathChopLastSlash(path);
 MG_FilePair result = MG_FilePairFromLayerPath(layer_path);
 return result;
}

function FILE *
MG_FileFromNodePair(MD_Node *node, MG_FilePair *pair)
{
 FILE *result = pair->h;
 if(MD_NodeHasTag(node, MD_S8Lit("c"), MD_StringMatchFlag_CaseInsensitive))
 {
  result = pair->c;
 }
 return result;
}

function void
MG_CloseAllFiles(void)
{
 for(int i = 0; i < mg_file_pair_count; i += 1)
 {
  fclose(mg_file_pairs[i].h);
  fclose(mg_file_pairs[i].c);
 }
}

function void
MG_GenerateMultilineStringAsCLiteral(FILE *file, String8 string)
{
 fprintf(file, "\"\"\n\"");
 for(U64 i = 0; i < string.size; i += 1)
 {
  if(string.str[i] == '\n')
  {
   fprintf(file, "\\n\"\n\"");
  }
  else if(string.str[i] == '\r')
  {
   continue;
  }
  else
  {
   fprintf(file, "%c", string.str[i]);
  }
 }
 fprintf(file, "\"\n");
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
