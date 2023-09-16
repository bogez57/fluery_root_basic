#ifndef METAPROGRAM_TABLE_H
#define METAPROGRAM_TABLE_H

typedef enum MG_TableOp
{
 MG_TableOp_Null,
 
 MG_TableOp_BeginStringOps,
 MG_TableOp_Dot,
 MG_TableOp_Bump,
 MG_TableOp_CheckIfTrue,
 MG_TableOp_Concat,
 MG_TableOp_EndStringOps,
 
 MG_TableOp_BeginNumericOps,
 MG_TableOp_Equal,
 MG_TableOp_IsNotEqual,
 MG_TableOp_BooleanAnd,
 MG_TableOp_BooleanOr,
 MG_TableOp_EndNumericOps,
 
 MG_TableOp_COUNT
}
MG_TableOp;

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

typedef struct MG_ExpandIter MG_ExpandIter;
struct MG_ExpandIter
{
 MG_ExpandIter *next;
 MG_NodeGrid *grid;
 MG_TableHeader *header;
 String8 label;
 U64 idx;
 U64 count;
};

typedef struct MG_ExpandInfo MG_ExpandInfo;
struct MG_ExpandInfo
{
 String8 strexpr;
 MG_ExpandIter *first_expand_iter;
 MD_ExprOprTable expr_op_table;
};

function MG_NodeArray MG_NodeArrayMake(U64 count);
function MG_NodeGrid MG_GridFromNode(MD_Node *node);
function MG_TableHeader MG_TableHeaderFromTag(MD_Node *tag);
function U64 MG_RowChildIndexFromColumnName(MG_TableHeader *header, String8 column_name);
function S64 MG_TableExprEvaluate_Numeric(MG_ExpandInfo *info, MD_Expr *expr);
function void MG_TableExprEvaluate_String(MG_ExpandInfo *info, MD_Expr *expr, String8List *out);
function void MG_LoopExpansionDimension(MG_ExpandIter *it, MG_ExpandInfo *info, String8List *out);
function String8List MG_GenStringListFromNode(MD_ExprOprTable expr_op_table, MD_Node *gen);
function void MG_TBL_Generate(MD_Node *file_list);

#endif // METAPROGRAM_TABLE_H
