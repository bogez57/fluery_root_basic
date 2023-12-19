/* date = June 23rd 2022 10:01 pm */

#ifndef FONT_CACHE_H
#define FONT_CACHE_H

////////////////////////////////
//~ rjf: Basic Types

typedef struct F_InitReceipt F_InitReceipt;
struct F_InitReceipt
{
 U64 u64[1];
};

typedef struct F_Tag F_Tag;
struct F_Tag
{
 U64 u64[2];
};

typedef struct F_Hash F_Hash;
struct F_Hash
{
 U64 u64[2];
};

////////////////////////////////
//~ rjf: Font Metadata Types

typedef struct F_Metrics F_Metrics;
struct F_Metrics
{
 F32 line_gap;
 F32 ascent;
 F32 descent;
 F32 capital_height;
};

////////////////////////////////
//~ rjf: Cache Extraction Types

typedef struct F_Piece F_Piece;
struct F_Piece
{
 F_Piece *next;
 R_Handle texture;
 Rng2F32 src_rect;
 Vec2F32 offset;
 F32 advance;
 U32 decode_size;
};

typedef struct F_Run F_Run;
struct F_Run
{
 F_Piece *first_piece;
 F_Piece *last_piece;
 U64 piece_count;
 F32 advance;
};

////////////////////////////////
//~ rjf: Tag -> Handle Types

typedef struct F_TagNode F_TagNode;
struct F_TagNode
{
 F_TagNode *hash_next;
 F_Tag tag;
 FP_Handle handle;
};

typedef struct F_TagSlot F_TagSlot;
struct F_TagSlot
{
 F_TagNode *first;
 F_TagNode *last;
};

////////////////////////////////
//~ rjf: Cache Types

typedef struct F_CacheNode F_CacheNode;
struct F_CacheNode
{
 F_CacheNode *next;
 F_CacheNode *prev;
 F_Hash hash;
 Rng2S64 alloc_region;
 F32 advance;
 F32 height;
};

typedef struct F_CacheSlot F_CacheSlot;
struct F_CacheSlot
{
 F_CacheNode *first;
 F_CacheNode *last;
};

////////////////////////////////
//~ rjf: Atlas State

typedef struct F_Atlas F_Atlas;
struct F_Atlas
{
 Atlas *allocator;
 R_Handle texture;
};

typedef struct F_AtlasNode F_AtlasNode;
struct F_AtlasNode
{
 F_AtlasNode *next;
 F_Atlas atlas;
};

typedef struct F_AtlasList F_AtlasList;
struct F_AtlasList
{
 F_AtlasNode *first;
 F_AtlasNode *last;
 U64 count;
};

////////////////////////////////
//~ rjf: Main State

typedef struct F_State F_State;
struct F_State
{
 Arena *arena;
 F_TagSlot *tag_table;
 U64 tag_table_size;
 F_CacheSlot *cache_table;
 U64 cache_table_size;
 F_Atlas atlas;
};

////////////////////////////////
//~ rjf: Globals

root_global F_State *f_state;

////////////////////////////////
//~ rjf: Tag Functions

root_function B32 F_TagMatch(F_Tag a, F_Tag b);
root_function F_Tag F_TagFromHash(U128 hash);
root_function F_Tag F_TagFromFontPath(String8 string);

root_function B32 F_HashMatch(F_Hash a, F_Hash b);
root_function F_Hash F_HashFromTagSizeString(F_Tag tag, F32 size, String8 string);

////////////////////////////////
//~ rjf: Cache Interaction Functions

root_function FP_Handle F_FontHandleFromTag(F_Tag font);
root_function F_Metrics F_MetricsFromTag(F_Tag font, F32 size);
root_function F_Run F_RunFromFontSizeString(Arena *arena, F_Tag font, F32 size, String8 string);
root_function F32 F_AdvanceFromFontSizeString(F_Tag font, F32 size, String8 string);
root_function U64 F_ByteOffFromFontSizeStringAdvance(F_Tag font, F32 size, String8 string, F32 advance);
root_function String8 F_TruncatedStringFromFontSizeStringMax(F_Tag font, F32 size, String8 string, F32 max, F32 trailer_advance);
root_function String8List F_WrappedStringLinesFromFontSizeStringMax(Arena *arena, F_Tag font, F32 size, String8 string, F32 max);

////////////////////////////////
//~ rjf: Diagnostics

root_function F_AtlasList F_PushAtlasList(Arena *arena);

////////////////////////////////
//~ rjf: Main API

root_function F_InitReceipt F_Init(FP_InitReceipt fp_init_receipt, R_InitReceipt r_init_receipt, Vec2S64 glyph_atlas_size);

#endif // FONT_CACHE_H
