/* date = August 4th 2022 6:57 pm */

#ifndef UI_H
#define UI_H

////////////////////////////////
//~ rjf: Keys

typedef struct UI_Key UI_Key;
struct UI_Key
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Semantic Sizes

typedef enum UI_SizeKind
{
 UI_SizeKind_Pixels,
 UI_SizeKind_TextDim,
 UI_SizeKind_Pct,
 UI_SizeKind_SizeByChildren,
 UI_SizeKind_COUNT
}
UI_SizeKind;

typedef struct UI_Size UI_Size;
struct UI_Size
{
 UI_SizeKind kind;
 F32 value;
 F32 strictness;
};

////////////////////////////////
//~ rjf: Main UI Hierarchy Node Types

typedef enum UI_TextAlignment
{
 UI_TextAlignment_Left,
 UI_TextAlignment_Center,
 UI_TextAlignment_Right,
 UI_TextAlignment_COUNT,
}
UI_TextAlignment;

typedef U32 UI_BoxFlags;
enum
{
 // rjf: interaction
 UI_BoxFlag_Disabled              = (1<<0),
 UI_BoxFlag_MouseClickable        = (1<<1),
 UI_BoxFlag_KeyboardClickable     = (1<<2),
 UI_BoxFlag_FocusHot              = (1<<3),
 UI_BoxFlag_FocusActive           = (1<<4),
 UI_BoxFlag_FocusHotDisabled      = (1<<5),
 UI_BoxFlag_FocusActiveDisabled   = (1<<6),
 UI_BoxFlag_ViewScroll            = (1<<7),
 
 // rjf: layout
 UI_BoxFlag_FloatingX             = (1<<8),
 UI_BoxFlag_FloatingY             = (1<<9),
 UI_BoxFlag_OverflowX             = (1<<10),
 UI_BoxFlag_OverflowY             = (1<<11),
 
 // rjf: appearance
 UI_BoxFlag_Clip                  = (1<<12),
 UI_BoxFlag_DisableTextTruncate   = (1<<13),
 UI_BoxFlag_DisableStringHashPart = (1<<14),
 UI_BoxFlag_DrawDropShadow        = (1<<15),
 UI_BoxFlag_DrawText              = (1<<16),
 UI_BoxFlag_DrawBorder            = (1<<17),
 UI_BoxFlag_DrawOverlay           = (1<<18),
 UI_BoxFlag_DrawBackground        = (1<<19),
 UI_BoxFlag_DrawHotEffects        = (1<<20),
 UI_BoxFlag_DrawActiveEffects     = (1<<21),
 UI_BoxFlag_DrawBucket            = (1<<22),
 UI_BoxFlag_DrawCustomFunction    = (1<<23),
 
 // rjf: helpers
 UI_BoxFlag_Floating              = UI_BoxFlag_FloatingX|UI_BoxFlag_FloatingY,
 UI_BoxFlag_Clickable             = UI_BoxFlag_MouseClickable|UI_BoxFlag_KeyboardClickable,
};

typedef void UI_BoxCustomDrawFunctionType(struct UI_Box *box);
#define UI_CUSTOM_DRAW_FUNCTION(name) void name(struct UI_Box *box)

typedef struct UI_BoxTextExt UI_BoxTextExt;
struct UI_BoxTextExt
{
 UI_TextAlignment string_alignment;
 F32 text_edge_padding;
 F_Tag font_tag;
 F32 font_size;
 Vec4F32 text_color;
};

typedef struct UI_BoxRectStyleExt UI_BoxRectStyleExt;
struct UI_BoxRectStyleExt
{
 F32 corner_radii[Corner_COUNT];
 F32 border_thickness;
 Vec4F32 background_color;
 Vec4F32 border_color;
 Vec4F32 overlay_color;
 R_Slice2F32 slice;
};

typedef struct UI_BoxBucketExt UI_BoxBucketExt;
struct UI_BoxBucketExt
{
 D_Bucket *bucket;
 F32 bucket_anchor_weights[Corner_COUNT];
};

typedef struct UI_BoxCustomDrawExt UI_BoxCustomDrawExt;
struct UI_BoxCustomDrawExt
{
 void *custom_draw_user_data;
 UI_BoxCustomDrawFunctionType *CustomDraw;
};

typedef struct UI_Box UI_Box;
struct UI_Box
{
 // rjf: hash links (persists across frames)
 UI_Box *hash_next;
 UI_Box *hash_prev;
 
 // rjf: tree link data (updated every frame)
 UI_Box *first;
 UI_Box *last;
 UI_Box *next;
 UI_Box *prev;
 UI_Box *parent;
 U64 child_count;
 
 // rjf: key info
 UI_Key key;
 
 // rjf: per-frame parameters
 UI_BoxFlags flags;
 String8 string;
 UI_Size pref_size[Axis2_COUNT];
 Axis2 child_layout_axis;
 OS_CursorKind hover_cursor;
 F32 opacity;
 UI_BoxTextExt             *ext_text;
 UI_BoxRectStyleExt        *ext_rect_style;
 UI_BoxBucketExt           *ext_bucket;
 UI_BoxCustomDrawExt       *ext_custom_draw;
 
 // rjf: post-size-determination data
 Vec2F32 calc_size;
 Vec2F32 calc_rel_pos;
 
 // rjf: post-layout data
 Rng2F32 rel_rect;
 Vec2F32 rel_corner_delta[Corner_COUNT];
 Rng2F32 rect;
 
 // rjf: persistent cross-frame state
 F32 hot_t;
 F32 active_t;
 F32 disabled_t;
 F32 focus_hot_t;
 F32 focus_active_t;
 U64 first_gen_touched;
 U64 last_gen_touched;
 Vec2F32 view_off;
 Vec2F32 target_view_off;
};

typedef struct UI_BoxRec UI_BoxRec;
struct UI_BoxRec
{
 UI_Box *next;
 S32 push_count;
 S32 pop_count;
};

typedef struct UI_Signal UI_Signal;
struct UI_Signal
{
 UI_Box *box;
 B8 clicked          : 1;
 B8 pressed          : 1;
 B8 released         : 1;
 B8 dragging         : 1;
 B8 double_clicked   : 1;
 B8 right_clicked    : 1;
 B8 right_pressed    : 1;
 B8 right_released   : 1;
 B8 right_dragging   : 1;
 B8 hovering         : 1;
 B8 mouse_is_over    : 1;
 B8 keyboard_pressed : 1;
 OS_Modifiers modifiers;
};

////////////////////////////////
//~ rjf: Generated Code

#include "generated/ui.meta.h"

////////////////////////////////
//~ rjf: State

typedef struct UI_BoxSlot UI_BoxSlot;
struct UI_BoxSlot
{
 UI_Box *first;
 UI_Box *last;
};

typedef struct UI_CursorVizData UI_CursorVizData;
struct UI_CursorVizData
{
 UI_Key key;
 B32 is_active;
 Vec2F32 p;
 F32 line_height;
 F32 advance;
 F32 velocity;
};

typedef struct UI_FocusVizData UI_FocusVizData;
struct UI_FocusVizData
{
 Rng2F32 rect;
 F32 alpha;
};

typedef struct UI_State UI_State;
struct UI_State
{
 // rjf: permanent state
 Arena *arena;
 U64 build_gen;
 
 // rjf: frame arenas
 Arena *frame_arenas[2];
 
 // rjf: mouse interaction state
 Vec2F32 mouse;
 B32 hot_key_found_this_frame;
 B32 action_killed_this_frame;
 UI_Key hot_key;
 UI_Key active_key[Side_COUNT];
 Vec2F32 drag_start_mouse;
 Arena *drag_data_arena;
 String8 drag_data;
 
 // rjf: context menu state
 UI_Key ctx_menu_key;
 UI_Key ctx_menu_anchor_key;
 Vec2F32 ctx_menu_anchor_offset;
 F32 ctx_menu_t;
 
 // rjf: cursor visualization/animation state
 UI_CursorVizData cursor_viz_data;
 Vec2F32 target_cursor_p;
 F32 target_cursor_line_height;
 F32 target_cursor_advance;
 U64 cursor_viz_data_build_gen;
 
 // rjf: persistent box state
 UI_Box *first_free_box;
 U64 free_box_list_count;
 UI_BoxSlot *box_table;
 U64 box_table_size;
 
 // rjf: per-build parameters
 OS_Handle window;
 OS_EventList *events;
 UI_Box *root;
 UI_Box *tooltip_root;
 UI_Box *ctx_menu_root;
 
 // rjf: stack state
 UI_DeclStackNils;
 UI_DeclStacks;
};

////////////////////////////////
//~ rjf: Globals

root_global UI_BoxTextExt ui_g_nil_box_text_ext;
root_global UI_BoxRectStyleExt ui_g_nil_box_rect_style_ext;
root_global UI_BoxBucketExt ui_g_nil_box_bucket_ext;
root_global UI_BoxCustomDrawExt ui_g_nil_box_custom_draw_ext;
root_global UI_Box ui_g_nil_box;

////////////////////////////////
//~ rjf: Basic Type Functions

//- rjf: id strings
root_function String8 UI_DisplayPartFromBoxString(String8 string);
root_function String8 UI_HashPartFromBoxString(String8 string);

//- rjf: keys
root_function UI_Key UI_KeyZero(void);
root_function UI_Key UI_KeyFromString(UI_Key seed, String8 string);
root_function UI_Key UI_KeyFromStringF(UI_Key seed, char *fmt, ...);
root_function B32 UI_KeyMatch(UI_Key a, UI_Key b);

//- rjf: boxes
root_function B32 UI_BoxIsNil(UI_Box *box);
#define UI_BoxSetNil(b) ((b) = &ui_g_nil_box)
root_function UI_BoxRec UI_BoxRecurseDepthFirst(UI_Box *box, UI_Box *stopper, MemberOffset sib, MemberOffset child);
#define UI_BoxRecurseDepthFirstPre(box, stopper) UI_BoxRecurseDepthFirst((box), (stopper), MemberOff(UI_Box, next), MemberOff(UI_Box, first))
#define UI_BoxRecurseDepthFirstPost(box, stopper) UI_BoxRecurseDepthFirst((box), (stopper), MemberOff(UI_Box, prev), MemberOff(UI_Box, last))

//- rjf: sizes
root_function UI_Size UI_SizeMake(UI_SizeKind kind, F32 value, F32 strictness);
#define UI_Pixels(v, strictness)      UI_SizeMake(UI_SizeKind_Pixels, (v), (strictness))
#define UI_TextDim(strictness)        UI_SizeMake(UI_SizeKind_TextDim, (0), (strictness))
#define UI_SizeByChildren(strictness) UI_SizeMake(UI_SizeKind_SizeByChildren, (0), (strictness))
#define UI_Pct(v, strictness)         UI_SizeMake(UI_SizeKind_Pct, (v), (strictness))
#define UI_Em(v, strictness)          UI_SizeMake(UI_SizeKind_Pixels, (v) * UI_TopFontSize(), (strictness))

////////////////////////////////
//~ rjf: Global Concepts

root_function F_Tag UI_IconFont(void);
root_function F32 UI_CursorBlinkT(void);
root_function void UI_ResetCursorBlinkT(void);

////////////////////////////////
//~ rjf: Frame Boundaries

root_function void UI_BeginFrame(F32 delta_time);
root_function void UI_EndFrame(void);

////////////////////////////////
//~ rjf: State Allocation & Selection

root_function UI_State *UI_StateAlloc(void);
root_function void UI_StateRelease(UI_State *state);
root_function void UI_SelectState(UI_State *state);
root_function UI_State *UI_SelectedState(void);

////////////////////////////////
//~ rjf: Selected State Accessors

root_function Arena *UI_FrameArena(void);
root_function UI_Box *UI_Root(void);
root_function OS_Handle UI_Window(void);
root_function OS_EventList *UI_Events(void);
root_function Vec2F32 UI_Mouse(void);
root_function Vec2F32 UI_DragDelta(void);
root_function UI_CursorVizData UI_GetCursorVizData(void);
root_function UI_Box *UI_BoxFromKey(UI_Key key);
root_function UI_Key UI_HotKey(void);
root_function UI_Key UI_ActiveKey(Side side);

////////////////////////////////
//~ rjf: Selected State Mutators

root_function void UI_SetCursorViz(UI_Key key, B32 is_active, Vec2F32 p, F32 line_height, F32 advance);

////////////////////////////////
//~ rjf: Drag Data

root_function void UI_StoreDragData(String8 data);
root_function String8 UI_LoadDragData(U64 needed_size);

#define UI_StoreDragDataStruct(ptr) UI_StoreDragData(Str8Struct(ptr))
#define UI_LoadDragDataStruct(type) ((type *)UI_LoadDragData(sizeof(type)).str)

////////////////////////////////
//~ rjf: Layout Passes

root_function void UI_SolveIndependentSizes(UI_Box *root, Axis2 axis);
root_function void UI_SolveUpwardDependentSizes(UI_Box *root, Axis2 axis);
root_function void UI_SolveDownwardDependentSizes(UI_Box *root, Axis2 axis);
root_function void UI_SolveSizeViolations(UI_Box *root, Axis2 axis);
root_function void UI_LayoutRoot(UI_Box *root, Axis2 axis);
root_function void UI_Layout(void);

////////////////////////////////
//~ rjf: Animation Passes

root_function void UI_AnimateRoot(UI_Box *root, F32 delta_time);
root_function void UI_Animate(F32 delta_time);

////////////////////////////////
//~ rjf: Build Phase Markers

root_function void UI_BeginBuild(OS_Handle window, OS_EventList *events);
root_function void UI_EndBuild(void);

////////////////////////////////
//~ rjf: Box Hierarchy Construction API

//- rjf: lowest level box construction
root_function UI_Box *UI_BoxMakeFromKey(UI_BoxFlags flags, UI_Key key);
root_function UI_Box *UI_BoxMake(UI_BoxFlags flags, String8 string);
root_function UI_Box *UI_BoxMakeF(UI_BoxFlags flags, char *fmt, ...);

//- rjf: extra box equipment
root_function void UI_BoxEquipDisplayString(UI_Box *box, String8 string);
root_function void UI_BoxEquipDrawBucket(UI_Box *box, D_Bucket *bucket, F32 anchor_weight_00, F32 anchor_weight_01, F32 anchor_weight_10, F32 anchor_weight_11);
root_function void UI_BoxEquipCustomDrawFunction(UI_Box *box, UI_BoxCustomDrawFunctionType *CustomDraw, void *user_data);

//- rjf: tooltip
root_function void UI_TooltipBaseBegin(void);
root_function void UI_TooltipBaseEnd(void);
root_function void UI_TooltipBegin(void);
root_function void UI_TooltipEnd(void);

//- rjf: box interaction
root_function UI_Signal UI_SignalFromBox(UI_Box *box);
root_function void UI_KillAction(void);

////////////////////////////////
//~ rjf: Box Accessors

root_function String8 UI_DisplayStringFromBox(UI_Box *box);
root_function Vec2F32 UI_TextPosFromBox(UI_Box *box);
root_function Rng1F32 UI_ScrollBoundsFromBox(UI_Box *box, Axis2 axis);

////////////////////////////////
//~ rjf: Focus

root_function B32 UI_IsFocusHot(void);
root_function B32 UI_IsFocusActive(void);

////////////////////////////////
//~ rjf: Context Menus

root_function void UI_OpenCtxMenu(UI_Key anchor_box_key, Vec2F32 anchor_offset, UI_Key ctx_menu_key);
root_function void UI_CloseCtxMenu(void);
root_function B32 UI_CtxMenuIsOpen(UI_Key key);
root_function B32 UI_CtxMenuBegin(UI_Key key);
root_function void UI_CtxMenuEnd(void);

////////////////////////////////
//~ rjf: Stacks

//- rjf: base
root_function UI_Box *              UI_TopParent(void);
root_function UI_BoxFlags           UI_TopFlags(void);
root_function F32                   UI_TopFixedX(void);
root_function F32                   UI_TopFixedY(void);
root_function UI_Size               UI_TopPrefWidth(void);
root_function UI_Size               UI_TopPrefHeight(void);
root_function F32                   UI_TopOpacity(void);
root_function Vec4F32               UI_TopTextColor(void);
root_function Vec4F32               UI_TopBackgroundColor(void);
root_function Vec4F32               UI_TopBorderColor(void);
root_function Vec4F32               UI_TopOverlayColor(void);
root_function Vec4F32               UI_TopFillColor(void);
root_function Vec4F32               UI_TopCursorColor(void);
root_function F32                   UI_TopCornerRadius00(void);
root_function F32                   UI_TopCornerRadius01(void);
root_function F32                   UI_TopCornerRadius10(void);
root_function F32                   UI_TopCornerRadius11(void);
root_function F32                   UI_TopBorderThickness(void);
root_function R_Slice2F32           UI_TopSlice2F32(void);
root_function F_Tag                 UI_TopFont(void);
root_function F32                   UI_TopFontSize(void);
root_function Axis2                 UI_TopChildLayoutAxis(void);
root_function OS_CursorKind         UI_TopHoverCursor(void);
root_function UI_TextAlignment      UI_TopTextAlign(void);
root_function F32                   UI_TopTextEdgePadding(void);
root_function UI_Key                UI_TopSeedKey(void);
root_function B32                   UI_TopFocusHotSet(void);
root_function B32                   UI_TopFocusHotPossible(void);
root_function B32                   UI_TopFocusActiveSet(void);
root_function B32                   UI_TopFocusActivePossible(void);
root_function UI_Box *              UI_PushParent(UI_Box * v);
root_function UI_BoxFlags           UI_PushFlags(UI_BoxFlags v);
root_function F32                   UI_PushFixedX(F32 v);
root_function F32                   UI_PushFixedY(F32 v);
root_function UI_Size               UI_PushPrefWidth(UI_Size v);
root_function UI_Size               UI_PushPrefHeight(UI_Size v);
root_function F32                   UI_PushOpacity(F32 v);
root_function Vec4F32               UI_PushTextColor(Vec4F32 v);
root_function Vec4F32               UI_PushBackgroundColor(Vec4F32 v);
root_function Vec4F32               UI_PushBorderColor(Vec4F32 v);
root_function Vec4F32               UI_PushOverlayColor(Vec4F32 v);
root_function Vec4F32               UI_PushFillColor(Vec4F32 v);
root_function Vec4F32               UI_PushCursorColor(Vec4F32 v);
root_function F32                   UI_PushCornerRadius00(F32 v);
root_function F32                   UI_PushCornerRadius01(F32 v);
root_function F32                   UI_PushCornerRadius10(F32 v);
root_function F32                   UI_PushCornerRadius11(F32 v);
root_function F32                   UI_PushBorderThickness(F32 v);
root_function R_Slice2F32           UI_PushSlice2F32(R_Slice2F32 v);
root_function F_Tag                 UI_PushFont(F_Tag v);
root_function F32                   UI_PushFontSize(F32 v);
root_function Axis2                 UI_PushChildLayoutAxis(Axis2 v);
root_function OS_CursorKind         UI_PushHoverCursor(OS_CursorKind v);
root_function UI_TextAlignment      UI_PushTextAlign(UI_TextAlignment v);
root_function F32                   UI_PushTextEdgePadding(F32 v);
root_function UI_Key                UI_PushSeedKey(UI_Key v);
root_function B32                   UI_PushFocusHotSet(B32 v);
root_function B32                   UI_PushFocusHotPossible(B32 v);
root_function B32                   UI_PushFocusActiveSet(B32 v);
root_function B32                   UI_PushFocusActivePossible(B32 v);
root_function UI_Box *              UI_PopParent(void);
root_function UI_BoxFlags           UI_PopFlags(void);
root_function F32                   UI_PopFixedX(void);
root_function F32                   UI_PopFixedY(void);
root_function UI_Size               UI_PopPrefWidth(void);
root_function UI_Size               UI_PopPrefHeight(void);
root_function F32                   UI_PopOpacity(void);
root_function Vec4F32               UI_PopTextColor(void);
root_function Vec4F32               UI_PopBackgroundColor(void);
root_function Vec4F32               UI_PopBorderColor(void);
root_function Vec4F32               UI_PopOverlayColor(void);
root_function Vec4F32               UI_PopFillColor(void);
root_function Vec4F32               UI_PopCursorColor(void);
root_function F32                   UI_PopCornerRadius00(void);
root_function F32                   UI_PopCornerRadius01(void);
root_function F32                   UI_PopCornerRadius10(void);
root_function F32                   UI_PopCornerRadius11(void);
root_function F32                   UI_PopBorderThickness(void);
root_function R_Slice2F32           UI_PopSlice2F32(void);
root_function F_Tag                 UI_PopFont(void);
root_function F32                   UI_PopFontSize(void);
root_function Axis2                 UI_PopChildLayoutAxis(void);
root_function OS_CursorKind         UI_PopHoverCursor(void);
root_function UI_TextAlignment      UI_PopTextAlign(void);
root_function F32                   UI_PopTextEdgePadding(void);
root_function UI_Key                UI_PopSeedKey(void);
root_function B32                   UI_PopFocusHotSet(void);
root_function B32                   UI_PopFocusHotPossible(void);
root_function B32                   UI_PopFocusActiveSet(void);
root_function B32                   UI_PopFocusActivePossible(void);
root_function UI_Box *              UI_SetNextParent(UI_Box * v);
root_function UI_BoxFlags           UI_SetNextFlags(UI_BoxFlags v);
root_function F32                   UI_SetNextFixedX(F32 v);
root_function F32                   UI_SetNextFixedY(F32 v);
root_function UI_Size               UI_SetNextPrefWidth(UI_Size v);
root_function UI_Size               UI_SetNextPrefHeight(UI_Size v);
root_function F32                   UI_SetNextOpacity(F32 v);
root_function Vec4F32               UI_SetNextTextColor(Vec4F32 v);
root_function Vec4F32               UI_SetNextBackgroundColor(Vec4F32 v);
root_function Vec4F32               UI_SetNextBorderColor(Vec4F32 v);
root_function Vec4F32               UI_SetNextOverlayColor(Vec4F32 v);
root_function Vec4F32               UI_SetNextFillColor(Vec4F32 v);
root_function Vec4F32               UI_SetNextCursorColor(Vec4F32 v);
root_function F32                   UI_SetNextCornerRadius00(F32 v);
root_function F32                   UI_SetNextCornerRadius01(F32 v);
root_function F32                   UI_SetNextCornerRadius10(F32 v);
root_function F32                   UI_SetNextCornerRadius11(F32 v);
root_function F32                   UI_SetNextBorderThickness(F32 v);
root_function R_Slice2F32           UI_SetNextSlice2F32(R_Slice2F32 v);
root_function F_Tag                 UI_SetNextFont(F_Tag v);
root_function F32                   UI_SetNextFontSize(F32 v);
root_function Axis2                 UI_SetNextChildLayoutAxis(Axis2 v);
root_function OS_CursorKind         UI_SetNextHoverCursor(OS_CursorKind v);
root_function UI_TextAlignment      UI_SetNextTextAlign(UI_TextAlignment v);
root_function F32                   UI_SetNextTextEdgePadding(F32 v);
root_function UI_Key                UI_SetNextSeedKey(UI_Key v);
root_function B32                   UI_SetNextFocusHotSet(B32 v);
root_function B32                   UI_SetNextFocusHotPossible(B32 v);
root_function B32                   UI_SetNextFocusActiveSet(B32 v);
root_function B32                   UI_SetNextFocusActivePossible(B32 v);
root_global String8 ui_g_icon_kind_string_table[49];

//- rjf: compositions
root_function void UI_PushCornerRadius(F32 v);
root_function void UI_PopCornerRadius(void);
root_function void UI_SetNextCornerRadius(F32 v);
root_function void UI_PushPrefSize(Axis2 axis, UI_Size v);
root_function void UI_PopPrefSize(Axis2 axis);
root_function void UI_SetNextPrefSize(Axis2 axis, UI_Size v);
root_function void UI_PushFixedPos(Vec2F32 v);
root_function void UI_PopFixedPos(void);
root_function void UI_SetNextFixedPos(Vec2F32 v);
root_function void UI_PushFixedRect(Rng2F32 rect);
root_function void UI_PopFixedRect(void);
root_function void UI_SetNextFixedRect(Rng2F32 rect);

////////////////////////////////
//~ rjf: Defer Loop Helpers

//- rjf: base
#define UI_Parent(v) DeferLoop(UI_PushParent(v), UI_PopParent())
#define UI_Flags(v) DeferLoop(UI_PushFlags(v), UI_PopFlags())
#define UI_FixedX(v) DeferLoop(UI_PushFixedX(v), UI_PopFixedX())
#define UI_FixedY(v) DeferLoop(UI_PushFixedY(v), UI_PopFixedY())
#define UI_PrefWidth(v) DeferLoop(UI_PushPrefWidth(v), UI_PopPrefWidth())
#define UI_PrefHeight(v) DeferLoop(UI_PushPrefHeight(v), UI_PopPrefHeight())
#define UI_Opacity(v) DeferLoop(UI_PushOpacity(v), UI_PopOpacity())
#define UI_TextColor(v) DeferLoop(UI_PushTextColor(v), UI_PopTextColor())
#define UI_BackgroundColor(v) DeferLoop(UI_PushBackgroundColor(v), UI_PopBackgroundColor())
#define UI_BorderColor(v) DeferLoop(UI_PushBorderColor(v), UI_PopBorderColor())
#define UI_OverlayColor(v) DeferLoop(UI_PushOverlayColor(v), UI_PopOverlayColor())
#define UI_FillColor(v) DeferLoop(UI_PushFillColor(v), UI_PopFillColor())
#define UI_CursorColor(v) DeferLoop(UI_PushCursorColor(v), UI_PopCursorColor())
#define UI_CornerRadius00(v) DeferLoop(UI_PushCornerRadius00(v), UI_PopCornerRadius00())
#define UI_CornerRadius01(v) DeferLoop(UI_PushCornerRadius01(v), UI_PopCornerRadius01())
#define UI_CornerRadius10(v) DeferLoop(UI_PushCornerRadius10(v), UI_PopCornerRadius10())
#define UI_CornerRadius11(v) DeferLoop(UI_PushCornerRadius11(v), UI_PopCornerRadius11())
#define UI_BorderThickness(v) DeferLoop(UI_PushBorderThickness(v), UI_PopBorderThickness())
#define UI_Slice2F32(v) DeferLoop(UI_PushSlice2F32(v), UI_PopSlice2F32())
#define UI_Font(v) DeferLoop(UI_PushFont(v), UI_PopFont())
#define UI_FontSize(v) DeferLoop(UI_PushFontSize(v), UI_PopFontSize())
#define UI_ChildLayoutAxis(v) DeferLoop(UI_PushChildLayoutAxis(v), UI_PopChildLayoutAxis())
#define UI_HoverCursor(v) DeferLoop(UI_PushHoverCursor(v), UI_PopHoverCursor())
#define UI_TextAlign(v) DeferLoop(UI_PushTextAlign(v), UI_PopTextAlign())
#define UI_TextEdgePadding(v) DeferLoop(UI_PushTextEdgePadding(v), UI_PopTextEdgePadding())
#define UI_SeedKey(v) DeferLoop(UI_PushSeedKey(v), UI_PopSeedKey())
#define UI_FocusHotSet(v) DeferLoop(UI_PushFocusHotSet(v), UI_PopFocusHotSet())
#define UI_FocusHotPossible(v) DeferLoop(UI_PushFocusHotPossible(v), UI_PopFocusHotPossible())
#define UI_FocusActiveSet(v) DeferLoop(UI_PushFocusActiveSet(v), UI_PopFocusActiveSet())
#define UI_FocusActivePossible(v) DeferLoop(UI_PushFocusActivePossible(v), UI_PopFocusActivePossible())

//- rjf: compositions
#define UI_FocusHot(v) DeferLoop((UI_PushFocusHotSet(v), UI_PushFocusHotPossible(1)), (UI_PopFocusHotSet(), UI_PopFocusHotPossible()))
#define UI_FocusActive(v) DeferLoop((UI_PushFocusActiveSet(v), UI_PushFocusActivePossible(1)), (UI_PopFocusActiveSet(), UI_PopFocusActivePossible()))
#define UI_Focus(v) UI_FocusHot(v) UI_FocusActive(v)
#define UI_WidthFill  UI_PrefWidth(UI_Pct(1, 0))
#define UI_HeightFill UI_PrefHeight(UI_Pct(1, 0))
#define UI_CornerRadius(v) DeferLoop(UI_PushCornerRadius(v), UI_PopCornerRadius())
#define UI_FixedPos(v) DeferLoop(UI_PushFixedPos(v), UI_PopFixedPos())
#define UI_Tooltip DeferLoop(UI_TooltipBegin(), UI_TooltipEnd())
#define UI_TooltipBase DeferLoop(UI_TooltipBaseBegin(), UI_TooltipBaseEnd())

//- rjf: other
#define UI_CtxMenu(key) DeferLoopChecked(UI_CtxMenuBegin(key), UI_CtxMenuEnd())

#endif // UI_H
