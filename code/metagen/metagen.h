/* date = September 10th 2023 3:26 pm */

#ifndef METAGEN_H
#define METAGEN_H

////////////////////////////////
//~ rjf: Types

typedef struct MG_FilePair MG_FilePair;
struct MG_FilePair
{
 String8 layer_path;
 FILE *h;
 FILE *c;
};

typedef struct MG_CFile MG_CFile;
struct MG_CFile
{
 MG_CFile *next;
 String8 path;
 String8 data;
 CL_TokenArray tokens;
};

typedef struct MG_CFileList MG_CFileList;
struct MG_CFileList
{
 MG_CFile *first;
 MG_CFile *last;
 U64 count;
};

////////////////////////////////
//~ rjf: Globals

global Arena *mg_arena = 0;
global String8 mg_project_path = {0};
global U64 mg_file_pair_count = 0;
global MG_FilePair mg_file_pairs[4096] = {0};

////////////////////////////////
//~ rjf: Helpers

function String8 Str8FromMD(MD_String8 string);
function MD_String8 MDFromStr8(String8 string);

function String8 MG_LayerNameFromPath(String8 layer_path);
function MG_FilePair MG_FilePairFromLayerPath(String8 layer_path);
function MG_FilePair MG_FilePairFromNode(MD_Node *node);
function MG_FilePair MG_FilePairFromCFile(MG_CFile *file);

function FILE *MG_FileFromNodePair(MD_Node *node, MG_FilePair *pair);
function void MG_CloseAllFiles(void);
function void MG_GenerateMultilineStringAsCLiteral(FILE *file, String8 string);
function String8 MG_EscapedFromString(Arena *arena, String8 string);

#endif // METAGEN_H
