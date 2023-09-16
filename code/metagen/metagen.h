/* date = September 10th 2023 3:26 pm */

#ifndef METAGEN_H
#define METAGEN_H

////////////////////////////////
//~ rjf: Generation Buckets

typedef struct MG_Bucket MG_Bucket;
struct MG_Bucket
{
 MG_Bucket *next;
 String8 layer_path;
 String8List enums;
 String8List structs;
 String8List h_functions;
 String8List c_functions;
 String8List h_tables;
 String8List c_tables;
};

typedef struct MG_BucketSlot MG_BucketSlot;
struct MG_BucketSlot
{
 MG_Bucket *first;
 MG_Bucket *last;
};

typedef struct MG_BucketMap MG_BucketMap;
struct MG_BucketMap
{
 U64 slots_count;
 MG_BucketSlot *slots;
};

////////////////////////////////
//~ rjf: Parsed C File Types

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
global MG_BucketMap mg_bucket_map = {0};

////////////////////////////////
//~ rjf: Basic Type Helpers

function String8 Str8FromMD(MD_String8 string);
function MD_String8 MDFromStr8(String8 string);
function U64 MG_HashFromString(String8 string);

////////////////////////////////
//~ rjf: Layer Bucket Lookups

function String8 MG_LayerPathFromMDNode(MD_Node *node);
function String8 MG_LayerPathFromCFile(MG_CFile *file);
function String8 MG_LayerNameFromPath(String8 layer_path);
function MG_Bucket *MG_BucketFromLayerPath(String8 layer_path);

////////////////////////////////
//~ rjf: String Transforms

function String8 MG_CStringLiteralFromMultilineString(String8 string);
function String8 MG_CArrayLiteralContentsFromData(String8 data);
function String8 MG_EscapedFromString(Arena *arena, String8 string);

#endif // METAGEN_H
