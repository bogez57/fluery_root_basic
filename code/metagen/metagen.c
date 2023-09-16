////////////////////////////////
//~ rjf: Basic Type Helpers

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
//~ rjf: Layer Bucket Lookups

function String8
MG_LayerPathFromMDNode(MD_Node *node)
{
 MD_CodeLoc loc = MD_CodeLocFromNode(node);
 String8 filename = Str8FromMD(loc.filename);
 String8 layer_path = Str8PathChopLastSlash(filename);
 return layer_path;
}

function String8
MG_LayerPathFromCFile(MG_CFile *file)
{
 String8 path = file->path;
 String8 layer_path = Str8PathChopLastSlash(path);
 return layer_path;
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

function MG_Bucket *
MG_BucketFromLayerPath(String8 layer_path)
{
 MG_Bucket *bucket = 0;
 {
  U64 hash = MG_HashFromString(layer_path);
  U64 slot_idx = hash%mg_bucket_map.slots_count;
  MG_BucketSlot *slot = &mg_bucket_map.slots[slot_idx];
  for(MG_Bucket *b = slot->first; b != 0; b = b->next)
  {
   if(Str8Match(b->layer_path, layer_path, 0))
   {
    bucket = b;
    break;
   }
  }
  if(bucket == 0)
  {
   bucket = PushArray(mg_arena, MG_Bucket, 1);
   QueuePush(slot->first, slot->last, bucket);
   bucket->layer_path = PushStr8Copy(mg_arena, layer_path);
  }
 }
 if(bucket->layer_path.size == 0)
 {
  int x = 0;
 }
 return bucket;
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
