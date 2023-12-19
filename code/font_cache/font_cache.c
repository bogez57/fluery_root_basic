////////////////////////////////
//~ rjf: Globals

#if BUILD_CORE
F_State *f_state = 0;
#endif

////////////////////////////////
//~ rjf: Tag Functions

root_function B32
F_TagMatch(F_Tag a, F_Tag b)
{
 return a.u64[0] == b.u64[0] && a.u64[1] == b.u64[1];
}

root_function F_Tag
F_TagFromHash(U128 hash)
{
 //- rjf: build tag from hash
 F_Tag result = {hash.u64[0], hash.u64[1]};
 
 //- rjf: tag => slot
 U64 slot = result.u64[1] % f_state->tag_table_size;
 
 //- rjf: map to node
 F_TagNode *node = 0;
 {
  for(F_TagNode *n = f_state->tag_table[slot].first; n != 0; n = n->hash_next)
  {
   if(F_TagMatch(n->tag, result))
   {
    node = n;
    break;
   }
  }
 }
 
 //- rjf: push new node if no node is in cache
 if(node == 0)
 {
  node = PushArray(f_state->arena, F_TagNode, 1);
  node->tag = result;
  node->handle = FP_FontOpen(hash);
  QueuePush_NZ(f_state->tag_table[slot].first, f_state->tag_table[slot].last, node, hash_next, CheckNull, SetNull);
 }
 
 return result;
}

root_function F_Tag
F_TagFromFontPath(String8 string)
{
 U128 hash = FS_DataHashFromPath(string, U64Max);
 F_Tag tag = F_TagFromHash(hash);
 return tag;
}

root_function B32
F_HashMatch(F_Hash a, F_Hash b)
{
 return a.u64[0] == b.u64[0] && a.u64[1] == b.u64[1];
}

root_function F_Hash
F_HashFromTagSizeString(F_Tag tag, F32 size, String8 string)
{
 union {F32 f; U64 u;} size_u = {0};
 size_u.f = size;
 U64 buffer[] =
 {
  tag.u64[0],
  tag.u64[1],
  size_u.u,
 };
 F_Hash hash = {0};
 {
  meow_u128 metadata_meow_hash = MeowHash(MeowDefaultSeed, sizeof(buffer), buffer);
  meow_u128 string_meow_hash   = MeowHash(MeowDefaultSeed, string.size, string.str);
  MemoryCopy(&hash, &metadata_meow_hash, sizeof(hash));
  hash.u64[0] = MeowU64From(metadata_meow_hash, 0) ^ MeowU64From(string_meow_hash, 1);
  hash.u64[1] = MeowU64From(metadata_meow_hash, 1) ^ MeowU64From(string_meow_hash, 0);
 }
 return hash;
}

////////////////////////////////
//~ rjf: Cache Interaction Functions

root_function FP_Handle
F_FontHandleFromTag(F_Tag font)
{
 //- rjf: tag => slot
 U64 slot = font.u64[1] % f_state->tag_table_size;
 
 //- rjf: tag*slot => node
 F_TagNode *node = 0;
 {
  for(F_TagNode *n = f_state->tag_table[slot].first; n != 0; n = n->hash_next)
  {
   if(F_TagMatch(n->tag, font))
   {
    node = n;
    break;
   }
  }
 }
 
 //- rjf: node => handle
 FP_Handle handle = {0};
 if(node != 0)
 {
  handle = node->handle;
 }
 return handle;
}

root_function F_Metrics
F_MetricsFromTag(F_Tag font, F32 size)
{
 F_Metrics result = {0};
 FP_Handle handle = F_FontHandleFromTag(font);
 FP_Metrics metrics = FP_MetricsFromFont(handle, size);
 {
  result.line_gap       = metrics.line_gap;
  result.ascent         = metrics.ascent;
  result.descent        = metrics.descent;
  result.capital_height = metrics.capital_height;
 }
 return result;
}

root_function F_Run
F_RunFromFontSizeString(Arena *arena, F_Tag font, F32 size, String8 string)
{
 Temp scratch = ScratchBegin(&arena, 1);
 F_Run run = {0};
 F_Metrics metrics = F_MetricsFromTag(font, size);
 size = (F32)(S64)size;
 {
  U64 cache_start_off = 0;
  for(U64 off = 0; off < string.size;)
  {
   // rjf: decode
   DecodedCodepoint decode = DecodeCodepointFromUtf8(string.str+off, string.size-off);
   off += decode.advance;
   if(decode.advance == 0) { break; }
   U32 codepoint = decode.codepoint;
   
   // rjf: hit cache portion boundary
   // NOTE(rjf): Currently just assuming a 1:1 mapping from codepoints to glyphs.
   // Not looking to get into shaping stuff here right now.
   if(off > cache_start_off)
   {
    String8 hash_portion = Str8(string.str+cache_start_off, off-cache_start_off);
    cache_start_off = off;
    
    // rjf: font/size/hash_portion => hash
    F_Hash hash = F_HashFromTagSizeString(font, size, hash_portion);
    
    // rjf: hash => slot
    U64 slot = hash.u64[1] % f_state->cache_table_size;
    
    // rjf: map hash => cache node
    F_CacheNode *node = 0;
    {
     for(F_CacheNode *n = f_state->cache_table[slot].first; n != 0; n = n->next)
     {
      if(F_HashMatch(n->hash, hash))
      {
       node = n;
       break;
      }
     }
    }
    
    // rjf: cache miss. 
    if(node == 0)
    {
     FP_Handle font_handle = F_FontHandleFromTag(font);
     FP_RasterResult raster = FP_Raster(scratch.arena, font_handle, size, hash_portion);
     
     // rjf: choose atlas
     F_Atlas *atlas = &f_state->atlas;
     
     // rjf: determine space we need to cache this glyph
     Vec2S64 glyph_needed_size = raster.atlas_dim;
     
     // rjf: allocate region from texture cache
     Rng2S64 region = AtlasRegionAlloc(f_state->arena, f_state->atlas.allocator, glyph_needed_size);
     
     // rjf: fill texture memory for this glyph in the cache
     Rng2S64 fill_region = R2S64(region.p0, Add2S64(region.p0, raster.atlas_dim));
     R_Tex2DFillRegion(atlas->texture, fill_region, raster.atlas);
     
     // rjf: push node
     {
      node = PushArray(f_state->arena, F_CacheNode, 1);
      node->hash = hash;
      node->alloc_region = region;
      node->advance = raster.advance;
      node->height = raster.height;
      DLLPushBack_NPZ(f_state->cache_table[slot].first,
                      f_state->cache_table[slot].last,
                      node, next, prev, CheckNull, SetNull);
     }
    }
    
    // rjf: push piece for this node
    if(node != 0)
    {
     Vec2F32 p0 = Vec2FromVec(node->alloc_region.p0);
     Vec2F32 p1 = {(F32)node->alloc_region.p1.x, p0.y + node->height};
     F_Piece *piece = PushArray(arena, F_Piece, 1);
     QueuePush(run.first_piece, run.last_piece, piece);
     piece->texture = f_state->atlas.texture;
     piece->src_rect = R2F32(p0, p1);
     piece->advance = node->advance;
     piece->decode_size = hash_portion.size;
     piece->offset.y = -metrics.ascent;
     run.piece_count += 1;
     run.advance += node->advance;
    }
   }
  }
 }
 ScratchEnd(scratch);
 return run;
}

root_function F32
F_AdvanceFromFontSizeString(F_Tag font, F32 size, String8 string)
{
 Temp scratch = ScratchBegin(0, 0);
 F_Run run = F_RunFromFontSizeString(scratch.arena, font, size, string);
 ScratchEnd(scratch);
 return run.advance;
}

root_function U64
F_ByteOffFromFontSizeStringAdvance(F_Tag font, F32 size, String8 string, F32 advance)
{
 U64 result = 0;
 Temp scratch = ScratchBegin(0, 0);
 F_Run run = F_RunFromFontSizeString(scratch.arena, font, size, string);
 U64 off_bytes = 0;
 F32 off_px = 0;
 for(F_Piece *piece = run.first_piece;
     piece != 0;
     piece = piece->next)
 {
  if(advance < off_px + piece->advance/2)
  {
   result = off_bytes;
   break;
  }
  if(advance >= off_px + piece->advance/2)
  {
   result = off_bytes + piece->decode_size;
  }
  off_px += piece->advance;
  off_bytes += piece->decode_size;
 }
 ScratchEnd(scratch);
 return result;
}

root_function String8
F_TruncatedStringFromFontSizeStringMax(F_Tag font, F32 size, String8 string, F32 max, F32 trailer_advance)
{
 // rjf: calculate truncated size
 U64 truncated_size = string.size;
 {
  Temp scratch  = ScratchBegin(0, 0);
  F_Run run = F_RunFromFontSizeString(scratch.arena, font, size, string);
  U64 off_bytes = 0;
  F32 off_px = 0;
  U64 violation_size = 0;
  for(F_Piece *piece = run.first_piece; piece != 0; piece = piece->next)
  {
   F32 advance_px    = piece->advance;
   U32 advance_bytes = piece->decode_size;
   if(off_px+trailer_advance <= max)
   {
    violation_size = off_bytes;
   }
   if(off_px+advance_px > max)
   {
    truncated_size = violation_size;
    break;
   }
   off_px    += advance_px;
   off_bytes += advance_bytes;
  }
  ScratchEnd(scratch);
 }
 
 // rjf: calculate result
 String8 result = Prefix8(string, truncated_size);
 return result;
}

root_function String8List
F_WrappedStringLinesFromFontSizeStringMax(Arena *arena, F_Tag font, F32 size, String8 string, F32 max)
{
 String8List list = {0};
 {
  Temp scratch = ScratchBegin(&arena, 1);
  F_Run run = F_RunFromFontSizeString(scratch.arena, font, size, string);
  F32 off_px = 0;
  U64 off_bytes = 0;
  U64 line_start_off_bytes = 0;
  U64 line_end_off_bytes = 0;
  B32 seeking_word_end = 0;
  F32 word_start_off_px = 0;
  F_Piece *last_word_start_piece = 0;
  U64 last_word_start_off_bytes = 0;
  for(F_Piece *piece = run.first_piece, *next = 0;; piece = next)
  {
   if(piece != 0) {next = piece->next;}
   
   // rjf: gather info
   U8 byte         = off_bytes < string.size ? string.str[off_bytes] : 0;
   F32 advance     = (piece != 0) ? piece->advance : 0;
   U64 decode_size = (piece != 0) ? piece->decode_size : 0;
   
   // rjf: find start/end of words
   B32 is_first_byte_of_word = 0;
   B32 is_first_space_after_word = 0;
   if(!seeking_word_end && !CharIsSpace(byte))
   {
    seeking_word_end = 1;
    is_first_byte_of_word = 1;
    last_word_start_off_bytes = off_bytes;
    last_word_start_piece = piece;
    word_start_off_px = off_px;
   }
   else if(seeking_word_end && CharIsSpace(byte))
   {
    seeking_word_end = 0;
    is_first_space_after_word = 1;
   }
   else if(seeking_word_end && byte == 0)
   {
    is_first_space_after_word = 1;
   }
   
   // rjf: determine properties of this advance
   B32 is_illegal = (off_px >= max);
   B32 is_next_illegal = (off_px + advance >= max);
   B32 is_end = (byte == 0);
   
   // rjf: legal word end -> extend line
   if(is_first_space_after_word && !is_illegal)
   {
    line_end_off_bytes = off_bytes;
   }
   
   // rjf: illegal mid-word split -> wrap mid-word
   if(is_next_illegal && word_start_off_px == 0)
   {
    String8 line = Str8(string.str + line_start_off_bytes, off_bytes - line_start_off_bytes);
    line = Str8SkipChopWhitespace(line);
    if(line.size != 0)
    {
     Str8ListPush(arena, &list, line);
    }
    off_px = advance;
    line_start_off_bytes = off_bytes;
    line_end_off_bytes = off_bytes;
    word_start_off_px = 0;
    last_word_start_piece = piece;
    last_word_start_off_bytes = off_bytes;
    off_bytes += decode_size;
   }
   
   // rjf: illegal word end -> wrap line
   else if(is_first_space_after_word && (is_illegal || is_end))
   {
    String8 line = Str8(string.str + line_start_off_bytes, line_end_off_bytes - line_start_off_bytes);
    line = Str8SkipChopWhitespace(line);
    if(line.size != 0)
    {
     Str8ListPush(arena, &list, line);
    }
    line_start_off_bytes = line_end_off_bytes;
    if(is_illegal)
    {
     off_px = 0;
     word_start_off_px = 0;
     off_bytes = last_word_start_off_bytes;
     next = last_word_start_piece;
    }
   }
   
   // rjf: advance offsets otherwise
   else
   {
    off_px += advance;
    off_bytes += decode_size;
   }
   
   // rjf: 0 piece and 0 next -> done
   if(piece == 0 && next == 0)
   {
    break;
   }
  }
  ScratchEnd(scratch);
 }
 return list;
}

////////////////////////////////
//~ rjf: Diagnostics

root_function F_AtlasList
F_PushAtlasList(Arena *arena)
{
 F_AtlasList list = {0};
 F_AtlasNode *n = PushArray(arena, F_AtlasNode, 1);
 n->atlas = f_state->atlas;
 QueuePush(list.first, list.last, n);
 list.count += 1;
 return list;
}

////////////////////////////////
//~ rjf: Main API

root_function F_InitReceipt
F_Init(FP_InitReceipt fp_init_receipt, R_InitReceipt r_init_receipt, Vec2S64 glyph_atlas_size)
{
 if(IsMainThread() && f_state == 0)
 {
  Arena *arena = ArenaAlloc(Gigabytes(64));
  f_state = PushArray(arena, F_State, 1);
  f_state->arena = arena;
  f_state->tag_table_size = 1024;
  f_state->tag_table = PushArray(arena, F_TagSlot, f_state->tag_table_size);
  f_state->cache_table_size = 16384;
  f_state->cache_table = PushArray(arena, F_CacheSlot, f_state->cache_table_size);
  f_state->atlas.allocator = AtlasMake(f_state->arena, glyph_atlas_size);
  f_state->atlas.texture = R_Tex2DAlloc(glyph_atlas_size, R_Tex2DFormat_RGBA8, R_Tex2DKind_Dynamic, 0);
 }
 F_InitReceipt init_receipt = {0};
 return init_receipt;
}
