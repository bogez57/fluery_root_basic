static void
CG_EMBED_Generate(MD_Node *file_list)
{
 for(MD_EachNode(file_ref, file_list->first_child))
 {
  MD_Node *file = MD_ResolveNodeFromReference(file_ref);
  for(MD_EachNode(node, file->first_child))
  {
   CG_FilePair f = CG_FilePairFromNode(node);
   if(MD_NodeHasTag(node, MD_S8Lit("embed_string"), MD_StringMatchFlag_CaseInsensitive))
   {
    FILE *file = CG_FileFromNodePair(node, &f);
    fprintf(file, "read_only global String8 %.*s =\nStr8LitComp(", MD_S8VArg(node->string));
    CG_GenerateMultilineStringAsCLiteral(file, node->first_child->string);
    fprintf(file, ");\n\n");
   }
   if(MD_NodeHasTag(node, MD_S8Lit("embed_file"), MD_StringMatchFlag_CaseInsensitive))
   {
    MD_String8 path = node->first_child->string;
    MD_String8 file_data = MD_LoadEntireFile(cg_arena, path);
    meow_u128 file_data_hash = MeowHash(MeowDefaultSeed, file_data.size, file_data.str);
    FILE *file = CG_FileFromNodePair(node, &f);
    fprintf(file, "read_only global U8 %.*s_data[] =\n{\n", MD_S8VArg(node->string));
    MD_u64 col = 0;
    for(MD_u64 idx = 0; idx < file_data.size; idx += 1, col += 1)
    {
     fprintf(file, "%i,", (int)file_data.str[idx]);
     if(col == 32)
     {
      fprintf(file,"\n");
      col = 0;
     }
    }
    fprintf(file, "};\n\n");
    fprintf(file, "read_only global String8 %.*s = {%.*s_data, sizeof(%.*s_data)};\n\n", MD_S8VArg(node->string), MD_S8VArg(node->string), MD_S8VArg(node->string));
    fprintf(file, "read_only global U64 %.*s_hash[2] = {0x%" PRIx64 ", 0x%" PRIx64 "};\n\n", MD_S8VArg(node->string), MeowU64From(file_data_hash, 0), MeowU64From(file_data_hash, 1));
   }
  }
 }
}
