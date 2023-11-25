/* date = April 6th 2023 4:17 pm */

#ifndef MDESK_H
#define MDESK_H

////////////////////////////////
//~ rjf: Messages

typedef enum MD_MsgKind
{
 MD_MsgKind_Null,
 MD_MsgKind_Note,
 MD_MsgKind_Warning,
 MD_MsgKind_Error,
 MD_MsgKind_FatalError,
}
MD_MsgKind;

typedef struct MD_Msg MD_Msg;
struct MD_Msg
{
 MD_Msg *next;
 struct MD_Node *node;
 MD_MsgKind kind;
 String8 string;
};

typedef struct MD_MsgList MD_MsgList;
struct MD_MsgList
{
 MD_Msg *first;
 MD_Msg *last;
 U64 count;
 MD_MsgKind worst_message_kind;
};

////////////////////////////////
//~ rjf: Token Types

typedef U32 MD_TokenFlags;
enum
{
 // rjf: base kind info
 MD_TokenFlag_Identifier          = (1<<0),
 MD_TokenFlag_Numeric             = (1<<1),
 MD_TokenFlag_StringLiteral       = (1<<2),
 MD_TokenFlag_Symbol              = (1<<3),
 MD_TokenFlag_Reserved            = (1<<4),
 MD_TokenFlag_Comment             = (1<<5),
 MD_TokenFlag_Whitespace          = (1<<6),
 MD_TokenFlag_Newline             = (1<<7),
 
 // rjf: decoration info
 MD_TokenFlag_StringSingleQuote   = (1<<8),
 MD_TokenFlag_StringDoubleQuote   = (1<<9),
 MD_TokenFlag_StringTick          = (1<<10),
 MD_TokenFlag_StringTriplet       = (1<<11),
 
 // rjf: error info
 MD_TokenFlag_BrokenComment       = (1<<12),
 MD_TokenFlag_BrokenStringLiteral = (1<<13),
 MD_TokenFlag_BadCharacter        = (1<<14),
};

typedef U32 MD_TokenGroups;
enum
{
 MD_TokenGroup_Comment    = MD_TokenFlag_Comment,
 MD_TokenGroup_Whitespace = (MD_TokenFlag_Whitespace|
                             MD_TokenFlag_Newline),
 MD_TokenGroup_Irregular  = (MD_TokenGroup_Comment|
                             MD_TokenGroup_Whitespace),
 MD_TokenGroup_Regular    = ~MD_TokenGroup_Irregular,
 MD_TokenGroup_Label      = (MD_TokenFlag_Identifier|
                             MD_TokenFlag_Numeric|
                             MD_TokenFlag_StringLiteral|
                             MD_TokenFlag_Symbol),
 MD_TokenGroup_Error      = (MD_TokenFlag_BrokenComment|
                             MD_TokenFlag_BrokenStringLiteral|
                             MD_TokenFlag_BadCharacter),
};

typedef struct MD_Token MD_Token;
struct MD_Token
{
 Rng1U64 range;
 MD_TokenFlags flags;
};

typedef struct MD_TokenChunkNode MD_TokenChunkNode;
struct MD_TokenChunkNode
{
 MD_TokenChunkNode *next;
 MD_Token *v;
 U64 count;
 U64 cap;
};

typedef struct MD_TokenChunkList MD_TokenChunkList;
struct MD_TokenChunkList
{
 MD_TokenChunkNode *first;
 MD_TokenChunkNode *last;
 U64 chunk_count;
 U64 total_token_count;
};

typedef struct MD_TokenArray MD_TokenArray;
struct MD_TokenArray
{
 MD_Token *v;
 U64 count;
};

////////////////////////////////
//~ rjf: Node Types

typedef enum MD_NodeKind
{
 MD_NodeKind_Nil,
 MD_NodeKind_File,
 MD_NodeKind_ErrorMarker,
 MD_NodeKind_Main,
 MD_NodeKind_Tag,
 MD_NodeKind_List,
 MD_NodeKind_Reference,
 MD_NodeKind_COUNT
}
MD_NodeKind;

typedef U64 MD_NodeFlags;
enum
{
 MD_NodeFlag_MaskSetDelimiters          = (0x3F<<0),
 MD_NodeFlag_HasParenLeft               = (1<<0),
 MD_NodeFlag_HasParenRight              = (1<<1),
 MD_NodeFlag_HasBracketLeft             = (1<<2),
 MD_NodeFlag_HasBracketRight            = (1<<3),
 MD_NodeFlag_HasBraceLeft               = (1<<4),
 MD_NodeFlag_HasBraceRight              = (1<<5),
 
 MD_NodeFlag_MaskSeparators             = (0xF<<6),
 MD_NodeFlag_IsBeforeSemicolon          = (1<<6),
 MD_NodeFlag_IsAfterSemicolon           = (1<<7),
 MD_NodeFlag_IsBeforeComma              = (1<<8),
 MD_NodeFlag_IsAfterComma               = (1<<9),
 
 MD_NodeFlag_MaskStringDelimiters       = (0xF<<10),
 MD_NodeFlag_StringSingleQuote          = (1<<10),
 MD_NodeFlag_StringDoubleQuote          = (1<<11),
 MD_NodeFlag_StringTick                 = (1<<12),
 MD_NodeFlag_StringTriplet              = (1<<13),
 
 MD_NodeFlag_MaskLabelKind              = (0xF<<14),
 MD_NodeFlag_Numeric                    = (1<<14),
 MD_NodeFlag_Identifier                 = (1<<15),
 MD_NodeFlag_StringLiteral              = (1<<16),
 MD_NodeFlag_Symbol                     = (1<<17),
};
#define MD_NodeFlag_AfterFromBefore(f) ((f) << 1)

typedef struct MD_Node MD_Node;
struct MD_Node
{
 // rjf: tree links
 MD_Node *next;
 MD_Node *prev;
 MD_Node *parent;
 MD_Node *first;
 MD_Node *last;
 
 // rjf: tag links
 MD_Node *first_tag;
 MD_Node *last_tag;
 
 // rjf: node info
 MD_NodeKind kind;
 MD_NodeFlags flags;
 String8 string;
 String8 raw_string;
 
 // rjf: source code info
 U64 src_offset;
};

typedef struct MD_NodeRec MD_NodeRec;
struct MD_NodeRec
{
 MD_Node *next;
 S32 push_count;
 S32 pop_count;
};

////////////////////////////////
//~ rjf: Text -> Tokens Types

typedef struct MD_TokenizeResult MD_TokenizeResult;
struct MD_TokenizeResult
{
 MD_TokenArray tokens;
 MD_MsgList msgs;
};

////////////////////////////////
//~ rjf: Tokens -> Tree Types

typedef struct MD_ParseResult MD_ParseResult;
struct MD_ParseResult
{
 MD_Node *root;
 MD_MsgList msgs;
};

////////////////////////////////
//~ rjf: Globals

root_global MD_Node md_nil_node;

////////////////////////////////
//~ rjf: Message Type Functions

root_function void MD_MsgListPush(Arena *arena, MD_MsgList *msgs, MD_Node *node, MD_MsgKind kind, String8 string);
root_function void MD_MsgListPushF(Arena *arena, MD_MsgList *msgs, MD_Node *node, MD_MsgKind kind, char *fmt, ...);
root_function void MD_MsgListConcatInPlace(MD_MsgList *dst, MD_MsgList *src);

////////////////////////////////
//~ rjf: Token Type Functions

root_function MD_Token MD_TokenMake(Rng1U64 range, MD_TokenFlags flags);
root_function B32 MD_TokenMatch(MD_Token a, MD_Token b);
root_function String8List MD_StringListFromTokenFlags(Arena *arena, MD_TokenFlags flags);
root_function void MD_TokenChunkListPush(Arena *arena, MD_TokenChunkList *list, U64 cap, MD_Token token);
root_function MD_TokenArray MD_TokenArrayFromChunkList(Arena *arena, MD_TokenChunkList *chunks);
root_function String8 MD_ContentStringFromTokenFlagsStr8(MD_TokenFlags flags, String8 string);

////////////////////////////////
//~ rjf: Node Type Functions

//- rjf: flag conversions
root_function MD_NodeFlags MD_NodeFlagsFromTokenFlags(MD_TokenFlags flags);

//- rjf: nil
root_function B32 MD_NodeIsNil(MD_Node *node);
#define MD_NodeSetNil(p) ((p) = &md_nil_node)

//- rjf: iteration
#define MD_EachNode(it, first) MD_Node *it = first; !MD_NodeIsNil(it); it = it->next
root_function MD_NodeRec MD_NodeRecDepthFirst(MD_Node *node, MD_Node *subtree_root, MemberOffset child_off, MemberOffset sib_off);
#define MD_NodeRecDepthFirstPre(node, subtree_root) MD_NodeRecDepthFirst((node), (subtree_root), MemberOff(MD_Node, first), MemberOff(MD_Node, next))
#define MD_NodeRecDepthFirstPost(node, subtree_root) MD_NodeRecDepthFirst((node), (subtree_root), MemberOff(MD_Node, last), MemberOff(MD_Node, prev))

//- rjf: tree building
root_function MD_Node *MD_PushNode(Arena *arena, MD_NodeKind kind, MD_NodeFlags flags, String8 string, String8 raw_string, U64 src_offset);
root_function void MD_NodePushChild(MD_Node *parent, MD_Node *node);
root_function void MD_NodePushTag(MD_Node *parent, MD_Node *node);

//- rjf: tree introspection
root_function MD_Node *  MD_NodeFromChainString(MD_Node *first, MD_Node *opl, String8 string, MatchFlags flags);
root_function MD_Node *  MD_NodeFromChainIndex(MD_Node *first, MD_Node *opl, U64 index);
root_function MD_Node *  MD_NodeFromChainFlags(MD_Node *first, MD_Node *opl, MD_NodeFlags flags);
root_function U64        MD_IndexFromNode(MD_Node *node);
root_function MD_Node *  MD_RootFromNode(MD_Node *node);
root_function MD_Node *  MD_ChildFromString(MD_Node *node, String8 child_string, MatchFlags flags);
root_function MD_Node *  MD_TagFromString(MD_Node *node, String8 tag_string, MatchFlags flags);
root_function MD_Node *  MD_ChildFromIndex(MD_Node *node, U64 index);
root_function MD_Node *  MD_TagFromIndex(MD_Node *node, U64 index);
root_function MD_Node *  MD_TagArgFromIndex(MD_Node *node, String8 tag_string, MatchFlags flags, U64 index);
root_function MD_Node *  MD_TagArgFromString(MD_Node *node, String8 tag_string, MatchFlags tag_str_flags, String8 arg_string, MatchFlags arg_str_flags);
root_function B32        MD_NodeHasChild(MD_Node *node, String8 string, MatchFlags flags);
root_function B32        MD_NodeHasTag(MD_Node *node, String8 string, MatchFlags flags);
root_function U64        MD_ChildCountFromNode(MD_Node *node);
root_function U64        MD_TagCountFromNode(MD_Node *node);

//- rjf: tree comparison
root_function B32 MD_NodeDeepMatch(MD_Node *a, MD_Node *b, MatchFlags flags);
root_function B32 MD_NodeMatch(MD_Node *a, MD_Node *b, MatchFlags flags);

////////////////////////////////
//~ rjf: Text -> Tokens Functions

root_function MD_TokenizeResult MD_TokenizeFromText(Arena *arena, String8 text);

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

root_function MD_ParseResult MD_ParseFromTextTokens(Arena *arena, String8 filename, String8 text, MD_TokenArray tokens);

////////////////////////////////
//~ rjf: Tree -> Text Functions

root_function String8List MD_DebugStringListFromTree(Arena *arena, MD_Node *root);

#endif // MDESK_H
