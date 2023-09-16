static void
MG_EMBED_Generate(MD_Node *file_list)
{
 for(MD_EachNode(file_ref, file_list->first_child))
 {
  MD_Node *file = MD_ResolveNodeFromReference(file_ref);
  for(MD_EachNode(node, file->first_child))
  {
   MG_FilePair f = MG_FilePairFromNode(node);
   if(MD_NodeHasTag(node, MD_S8Lit("embed_string"), MD_StringMatchFlag_CaseInsensitive))
   {
    FILE *file = MG_FileFromNodePair(node, &f);
    fprintf(file, "read_only global String8 %.*s =\nStr8LitComp(", Str8VArg(node->string));
    MG_GenerateMultilineStringAsCLiteral(file, Str8FromMD(node->first_child->string));
    fprintf(file, ");\n\n");
   }
   if(MD_NodeHasTag(node, MD_S8Lit("embed_file"), MD_StringMatchFlag_CaseInsensitive))
   {
    String8 path = Str8FromMD(node->first_child->string);
    String8 file_data = OS_DataFromFilePath(mg_arena, path);
    meow_u128 file_data_hash = MeowHash(MeowDefaultSeed, file_data.size, file_data.str);
    FILE *file = MG_FileFromNodePair(node, &f);
    fprintf(file, "read_only global U8 %.*s_data[] =\n{\n", Str8VArg(node->string));
    U64 col = 0;
    for(U64 idx = 0; idx < file_data.size; idx += 1, col += 1)
    {
     fprintf(file, "%i,", (int)file_data.str[idx]);
     if(col == 32)
     {
      fprintf(file,"\n");
      col = 0;
     }
    }
    fprintf(file, "};\n\n");
    fprintf(file, "read_only global String8 %.*s = {%.*s_data, sizeof(%.*s_data)};\n\n", Str8VArg(node->string), Str8VArg(node->string), Str8VArg(node->string));
    fprintf(file, "read_only global U64 %.*s_hash[2] = {0x%I64x, 0x%I64x};\n\n", Str8VArg(node->string), MeowU64From(file_data_hash, 0), MeowU64From(file_data_hash, 1));
   }
  }
 }
}
