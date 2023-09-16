/* date = September 11th 2023 8:45 pm */

#ifndef C_LEX_H
#define C_LEX_H

////////////////////////////////
//~ rjf: Token Types

typedef U32 CL_TokenKind;
typedef enum CL_TokenKindEnum
{
 CL_TokenKind_Null,
 CL_TokenKind_Error,
 CL_TokenKind_Newline,
 CL_TokenKind_Whitespace,
 CL_TokenKind_Identifier,
 CL_TokenKind_CharLiteral,
 CL_TokenKind_StringLiteral,
 CL_TokenKind_NumericLiteral,
 CL_TokenKind_Symbol,
 CL_TokenKind_Preprocessor,
 CL_TokenKind_Comment,
 CL_TokenKind_COUNT
}
CL_TokenKindEnum;

typedef struct CL_Token CL_Token;
struct CL_Token
{
 CL_TokenKind kind;
 Rng1U64 range;
};

typedef struct CL_TokenNode CL_TokenNode;
struct CL_TokenNode
{
 CL_TokenNode *next;
 CL_Token v;
};

typedef struct CL_TokenList CL_TokenList;
struct CL_TokenList
{
 CL_TokenNode *first;
 CL_TokenNode *last;
 U64 count;
};

typedef struct CL_TokenChunkNode CL_TokenChunkNode;
struct CL_TokenChunkNode
{
 CL_TokenChunkNode *next;
 CL_Token *v;
 U64 count;
 U64 cap;
};

typedef struct CL_TokenChunkList CL_TokenChunkList;
struct CL_TokenChunkList
{
 CL_TokenChunkNode *first;
 CL_TokenChunkNode *last;
 U64 chunk_count;
 U64 token_count;
};

typedef struct CL_TokenArray CL_TokenArray;
struct CL_TokenArray
{
 CL_Token *v;
 U64 count;
};

typedef struct CL_TokenPtrNode CL_TokenPtrNode;
struct CL_TokenPtrNode
{
 CL_TokenPtrNode *next;
 CL_Token *v;
};

typedef struct CL_TokenPtrList CL_TokenPtrList;
struct CL_TokenPtrList
{
 CL_TokenPtrNode *first;
 CL_TokenPtrNode *last;
 U64 count;
};

typedef struct CL_TokenPtrListNode CL_TokenPtrListNode;
struct CL_TokenPtrListNode
{
 CL_TokenPtrListNode *next;
 CL_TokenPtrList v;
};

typedef struct CL_TokenPtrListList CL_TokenPtrListList;
struct CL_TokenPtrListList
{
 CL_TokenPtrListNode *first;
 CL_TokenPtrListNode *last;
 U64 node_count;
 U64 total_count;
};

////////////////////////////////
//~ rjf: Token Type Functions

function void CL_TokenListPush(Arena *arena, CL_TokenList *list, CL_Token *token);
function void CL_TokenPtrListPush(Arena *arena, CL_TokenPtrList *list, CL_Token *token);
function void CL_TokenPtrListListPush(Arena *arena, CL_TokenPtrListList *list, CL_TokenPtrList *v);
function void CL_TokenChunkListPush(Arena *arena, CL_TokenChunkList *list, U64 cap, CL_Token *token);
function CL_TokenArray CL_TokenArrayFromChunkList(Arena *arena, CL_TokenChunkList *list);

////////////////////////////////
//~ rjf: Lexing Functions

function CL_TokenArray CL_TokenArrayFromString(Arena *arena, String8 string);

#endif //C_LEX_H
