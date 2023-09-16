static void
MG_EMBED_Generate(MD_Node *file_list)
{
 for(MD_EachNode(file_ref, file_list->first_child))
 {
  MD_Node *file = MD_ResolveNodeFromReference(file_ref);
  String8 layer_path = MG_LayerPathFromMDNode(file->first_child);
  MG_Bucket *bucket = MG_BucketFromLayerPath(layer_path);
  for(MD_EachNode(node, file->first_child))
  {
   if(MD_NodeHasTag(node, MD_S8Lit("embed_string"), MD_StringMatchFlag_CaseInsensitive))
   {
    String8 c_literal_text = MG_CStringLiteralFromMultilineString(Str8FromMD(node->first_child->string));
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern String8 %S;", node->string);
    Str8ListPushF(mg_arena, &bucket->c_tables, "read_only String8 %S =\nStr8LitComp(", node->string);
    Str8ListPush (mg_arena, &bucket->c_tables, c_literal_text);
    Str8ListPushF(mg_arena, &bucket->c_tables, ");\n\n");
   }
   if(MD_NodeHasTag(node, MD_S8Lit("embed_file"), MD_StringMatchFlag_CaseInsensitive))
   {
    String8 path = Str8FromMD(node->first_child->string);
    String8 data = OS_DataFromFilePath(mg_arena, path);
    meow_u128 hash = MeowHash(MeowDefaultSeed, data.size, data.str);
    String8 data_textified = MG_CArrayLiteralContentsFromData(data);
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern String8 %S;", node->string);
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern U8 %S_data[%I64u];", node->string, data.size);
    Str8ListPushF(mg_arena, &bucket->h_tables, "extern U128 %S_hash;", node->string);
    Str8ListPushF(mg_arena, &bucket->h_tables, "read_only String8 %S = {%S_data, sizeof(%S_data)};\n\n", node->string, node->string, node->string);
    Str8ListPushF(mg_arena, &bucket->c_tables, "read_only U8 %S_data[%I64u] =\n{\n", node->string, data.size);
    Str8ListPush (mg_arena, &bucket->c_tables, data_textified);
    Str8ListPushF(mg_arena, &bucket->c_tables, "};\n\n");
    Str8ListPushF(mg_arena, &bucket->c_tables, "read_only U128 %S_hash = {0x%I64x, 0x%I64x};\n\n", node->string, MeowU64From(hash, 0), MeowU64From(hash, 1));
   }
  }
 }
}
