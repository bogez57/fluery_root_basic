/* date = September 10th 2023 3:26 pm */

#ifndef METAGEN_H
#define METAGEN_H

////////////////////////////////
//~ rjf: String Expression Types

typedef enum MG_StrExprOpKind
{
 MG_StrExprOpKind_Null,
 MG_StrExprOpKind_Prefix,
 MG_StrExprOpKind_Postfix,
 MG_StrExprOpKind_Binary,
 MG_StrExprOpKind_COUNT
}
MG_StrExprOpKind;

typedef enum MG_StrExprOp
{
 MG_StrExprOp_Null,
 
#define MG_StrExprOp_FirstString MG_StrExprOp_Dot
 MG_StrExprOp_Dot,
 MG_StrExprOp_ExpandIfTrue,
 MG_StrExprOp_Concat,
 MG_StrExprOp_BumpToColumn,
#define MG_StrExprOp_LastString MG_StrExprOp_BumpToColumn
 
#define MG_StrExprOp_FirstNumeric MG_StrExprOp_Add
 MG_StrExprOp_Add,
 MG_StrExprOp_Subtract,
 MG_StrExprOp_Multiply,
 MG_StrExprOp_Divide,
 MG_StrExprOp_Modulo,
 MG_StrExprOp_LeftShift,
 MG_StrExprOp_RightShift,
 MG_StrExprOp_BitwiseAnd,
 MG_StrExprOp_BitwiseOr,
 MG_StrExprOp_BitwiseXor,
 MG_StrExprOp_BitwiseNegate,
 MG_StrExprOp_BooleanAnd,
 MG_StrExprOp_BooleanOr,
 MG_StrExprOp_BooleanNot,
 MG_StrExprOp_Equals,
 MG_StrExprOp_DoesNotEqual,
#define MG_StrExprOp_LastNumeric MG_StrExprOp_DoesNotEqual
 
 MG_StrExprOp_COUNT,
}
MG_StrExprOp;

typedef struct MG_StrExpr MG_StrExpr;
struct MG_StrExpr
{
 MG_StrExpr *parent;
 MG_StrExpr *left;
 MG_StrExpr *right;
 MG_StrExprOp op;
 MD_Node *node;
};

typedef struct MG_StrExprParseResult MG_StrExprParseResult;
struct MG_StrExprParseResult
{
 MG_StrExpr *root;
 MD_MsgList msgs;
 MD_Node *next_node;
};

////////////////////////////////
//~ rjf: Node Table Types

typedef struct MG_NodeArray MG_NodeArray;
struct MG_NodeArray
{
 U64 count;
 MD_Node **v;
};

typedef struct MG_NodeGrid MG_NodeGrid;
struct MG_NodeGrid
{
 MG_NodeArray cells;
 MG_NodeArray row_parents;
};

typedef enum MG_ColumnKind
{
 MG_ColumnKind_Default,
 MG_ColumnKind_CheckForTag,
 MG_ColumnKind_COUNT
}
MG_ColumnKind;

typedef struct MG_ColumnDesc MG_ColumnDesc;
struct MG_ColumnDesc
{
 MG_ColumnKind kind;
 String8 name;
 String8 tag_string;
 String8 default_value;
};

typedef struct MG_TableHeader MG_TableHeader;
struct MG_TableHeader
{
 U64 column_count;
 MG_ColumnDesc *column_descs;
};

typedef struct MG_Table MG_Table;
struct MG_Table
{
 MD_Node *root;
 MG_TableHeader header;
 MG_NodeGrid grid;
};

typedef struct MG_TableNode MG_TableNode;
struct MG_TableNode
{
 MG_TableNode *next;
 MG_Table v;
};

typedef struct MG_TableSlot MG_TableSlot;
struct MG_TableSlot
{
 MG_TableNode *first;
 MG_TableNode *last;
};

////////////////////////////////
//~ rjf: Table Expansion Types

typedef struct MG_ExpandIter MG_ExpandIter;
struct MG_ExpandIter
{
 MG_ExpandIter *next;
 MG_Table *table;
 String8 label;
 U64 idx;
 U64 count;
};

typedef struct MG_ExpandInfo MG_ExpandInfo;
struct MG_ExpandInfo
{
 String8 strexpr;
 MG_ExpandIter *first_expand_iter;
};

////////////////////////////////
//~ rjf: Tweak Types

typedef struct MG_Tweak MG_Tweak;
struct MG_Tweak
{
 MG_Tweak *next;
 String8 name;
 String8 default_val;
 String8 min;
 String8 max;
};

typedef struct MG_TweakList MG_TweakList;
struct MG_TweakList
{
 MG_Tweak *first;
 MG_Tweak *last;
 U64 count;
};

////////////////////////////////
//~ rjf: Parsed Metadesk File Types

typedef struct MG_MDeskFile MG_MDeskFile;
struct MG_MDeskFile
{
 MG_MDeskFile *next;
 String8 path;
 String8 data;
 MD_ParseResult parse;
};

typedef struct MG_MDeskFileList MG_MDeskFileList;
struct MG_MDeskFileList
{
 MG_MDeskFile *first;
 MG_MDeskFile *last;
 U64 count;
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
//~ rjf: Layer Output Bundles

typedef struct MG_Layer MG_Layer;
struct MG_Layer
{
 String8 key;
 MG_TweakList tweak_b32s;
 MG_TweakList tweak_f32s;
 String8List enums;
 String8List structs;
 String8List h_functions;
 String8List h_tables;
 String8List h_catchall;
 String8List c_functions;
 String8List c_tables;
 String8List c_catchall;
};

typedef struct MG_LayerNode MG_LayerNode;
struct MG_LayerNode
{
 MG_LayerNode *next;
 MG_Layer v;
};

typedef struct MG_LayerSlot MG_LayerSlot;
struct MG_LayerSlot
{
 MG_LayerNode *first;
 MG_LayerNode *last;
};

////////////////////////////////
//~ rjf: Main State Bundle

typedef struct MG_State MG_State;
struct MG_State
{
 U64 layer_slots_count;
 MG_LayerSlot *layer_slots;
 U64 table_slots_count;
 MG_TableSlot *table_slots;
 MD_MsgList msgs;
};

////////////////////////////////
//~ rjf: Globals

global Arena *mg_arena = 0;
global MG_State *mg_state = 0;
read_only global MG_StrExpr mg_str_expr_nil = {&mg_str_expr_nil, &mg_str_expr_nil, &mg_str_expr_nil};

////////////////////////////////
//~ rjf: Basic Type Helpers

function U64 MG_HashFromString(String8 string);

////////////////////////////////
//~ rjf: Tweak Parsing

function MG_TweakList MG_TweakListFromTextTokensName(Arena *arena, String8 text, CL_TokenArray tokens, String8 name);
function void MG_TweakListConcatInPlace(MG_TweakList *dst, MG_TweakList *src);

////////////////////////////////
//~ rjf: String Expression Parsing

function MG_StrExpr *MG_PushStrExpr(Arena *arena, MG_StrExprOp op, MD_Node *node);
function MG_StrExprParseResult MG_StrExprParseFromFirstOPL_MinPrec(Arena *arena, MD_Node *first, MD_Node *opl, S8 min_prec);
function MG_StrExprParseResult MG_StrExprParseFromFirstOPL(Arena *arena, MD_Node *first, MD_Node *opl);
function MG_StrExprParseResult MG_StrExprParseFromRoot(Arena *arena, MD_Node *root);

////////////////////////////////
//~ rjf: String Transforms

function String8 MG_CStringLiteralFromMultilineString(String8 string);
function String8 MG_CArrayLiteralContentsFromData(String8 data);
function String8 MG_EscapedFromString(Arena *arena, String8 string);

////////////////////////////////
//~ rjf: Layer Lookup Functions

function String8 MG_LayerKeyFromPath(String8 path);
function MG_Layer *MG_LayerFromKey(String8 key);

////////////////////////////////
//~ rjf: Table Insertion/Lookup Functions

function MG_Table *MG_TableFromName(String8 name);

////////////////////////////////
//~ rjf: String Expression Functions

function U64 MG_TableRowChildIndexFromColumnName(MG_TableHeader *header, String8 column_name);
function S64 MG_StrExprEvaluate_Numeric(MG_ExpandInfo *info, MG_StrExpr *expr);
function void MG_StrExprEvaluate_String(MG_ExpandInfo *info, MG_StrExpr *expr, String8List *out);
function void MG_StrExprLoopExpansionDimension(MG_ExpandIter *it, MG_ExpandInfo *info, String8List *out);
function String8List MG_GenStringListFromNode(MD_Node *gen);
function String8List *MG_StringBucketFromLayerNode(MG_Layer *layer, MD_Node *gen);

#endif // METAGEN_H
