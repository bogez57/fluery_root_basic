//- GENERATED CODE

#ifndef UI_META_H
#define UI_META_H

typedef enum UI_IconKind
{
UI_IconKind_Null,
UI_IconKind_Check,
UI_IconKind_FilledCircle,
UI_IconKind_FilledStar,
UI_IconKind_HollowStar,
UI_IconKind_Plus,
UI_IconKind_Pencil,
UI_IconKind_TrashCan,
UI_IconKind_ClosedFolder,
UI_IconKind_OpenFolder,
UI_IconKind_FilePlain,
UI_IconKind_FileCopy,
UI_IconKind_FileTxt,
UI_IconKind_FileImg,
UI_IconKind_FileSound,
UI_IconKind_FileArchive,
UI_IconKind_Gears,
UI_IconKind_Sound0,
UI_IconKind_Sound1,
UI_IconKind_Sound2,
UI_IconKind_DownTri,
UI_IconKind_UpTri,
UI_IconKind_LeftTri,
UI_IconKind_RightTri,
UI_IconKind_LeftCaret,
UI_IconKind_RightCaret,
UI_IconKind_UpCaret,
UI_IconKind_DownCaret,
UI_IconKind_Refresh,
UI_IconKind_Undo,
UI_IconKind_Redo,
UI_IconKind_Machine,
UI_IconKind_X,
UI_IconKind_Maximize,
UI_IconKind_Restore,
UI_IconKind_Minimize,
UI_IconKind_Lock,
UI_IconKind_Unlock,
UI_IconKind_Tag,
UI_IconKind_Save,
UI_IconKind_Gamepad,
UI_IconKind_Keyboard,
UI_IconKind_UnfilledCheck,
UI_IconKind_FilledCheck,
UI_IconKind_Warning,
UI_IconKind_Question,
UI_IconKind_Info,
UI_IconKind_Eye,
UI_IconKind_EyeCovered,
UI_IconKind_COUNT,
}
UI_IconKind;

typedef struct UI_ParentNode UI_ParentNode; struct UI_ParentNode{UI_ParentNode *next; UI_Box * v;};
typedef struct UI_FlagsNode UI_FlagsNode; struct UI_FlagsNode{UI_FlagsNode *next; UI_BoxFlags v;};
typedef struct UI_FixedXNode UI_FixedXNode; struct UI_FixedXNode{UI_FixedXNode *next; F32 v;};
typedef struct UI_FixedYNode UI_FixedYNode; struct UI_FixedYNode{UI_FixedYNode *next; F32 v;};
typedef struct UI_PrefWidthNode UI_PrefWidthNode; struct UI_PrefWidthNode{UI_PrefWidthNode *next; UI_Size v;};
typedef struct UI_PrefHeightNode UI_PrefHeightNode; struct UI_PrefHeightNode{UI_PrefHeightNode *next; UI_Size v;};
typedef struct UI_OpacityNode UI_OpacityNode; struct UI_OpacityNode{UI_OpacityNode *next; F32 v;};
typedef struct UI_TextColorNode UI_TextColorNode; struct UI_TextColorNode{UI_TextColorNode *next; Vec4F32 v;};
typedef struct UI_BackgroundColorNode UI_BackgroundColorNode; struct UI_BackgroundColorNode{UI_BackgroundColorNode *next; Vec4F32 v;};
typedef struct UI_BorderColorNode UI_BorderColorNode; struct UI_BorderColorNode{UI_BorderColorNode *next; Vec4F32 v;};
typedef struct UI_OverlayColorNode UI_OverlayColorNode; struct UI_OverlayColorNode{UI_OverlayColorNode *next; Vec4F32 v;};
typedef struct UI_FillColorNode UI_FillColorNode; struct UI_FillColorNode{UI_FillColorNode *next; Vec4F32 v;};
typedef struct UI_CursorColorNode UI_CursorColorNode; struct UI_CursorColorNode{UI_CursorColorNode *next; Vec4F32 v;};
typedef struct UI_CornerRadius00Node UI_CornerRadius00Node; struct UI_CornerRadius00Node{UI_CornerRadius00Node *next; F32 v;};
typedef struct UI_CornerRadius01Node UI_CornerRadius01Node; struct UI_CornerRadius01Node{UI_CornerRadius01Node *next; F32 v;};
typedef struct UI_CornerRadius10Node UI_CornerRadius10Node; struct UI_CornerRadius10Node{UI_CornerRadius10Node *next; F32 v;};
typedef struct UI_CornerRadius11Node UI_CornerRadius11Node; struct UI_CornerRadius11Node{UI_CornerRadius11Node *next; F32 v;};
typedef struct UI_BorderThicknessNode UI_BorderThicknessNode; struct UI_BorderThicknessNode{UI_BorderThicknessNode *next; F32 v;};
typedef struct UI_Slice2F32Node UI_Slice2F32Node; struct UI_Slice2F32Node{UI_Slice2F32Node *next; R_Slice2F32 v;};
typedef struct UI_FontNode UI_FontNode; struct UI_FontNode{UI_FontNode *next; F_Tag v;};
typedef struct UI_FontSizeNode UI_FontSizeNode; struct UI_FontSizeNode{UI_FontSizeNode *next; F32 v;};
typedef struct UI_ChildLayoutAxisNode UI_ChildLayoutAxisNode; struct UI_ChildLayoutAxisNode{UI_ChildLayoutAxisNode *next; Axis2 v;};
typedef struct UI_HoverCursorNode UI_HoverCursorNode; struct UI_HoverCursorNode{UI_HoverCursorNode *next; OS_CursorKind v;};
typedef struct UI_TextAlignNode UI_TextAlignNode; struct UI_TextAlignNode{UI_TextAlignNode *next; UI_TextAlignment v;};
typedef struct UI_TextEdgePaddingNode UI_TextEdgePaddingNode; struct UI_TextEdgePaddingNode{UI_TextEdgePaddingNode *next; F32 v;};
typedef struct UI_SeedKeyNode UI_SeedKeyNode; struct UI_SeedKeyNode{UI_SeedKeyNode *next; UI_Key v;};
typedef struct UI_FocusHotSetNode UI_FocusHotSetNode; struct UI_FocusHotSetNode{UI_FocusHotSetNode *next; B32 v;};
typedef struct UI_FocusHotPossibleNode UI_FocusHotPossibleNode; struct UI_FocusHotPossibleNode{UI_FocusHotPossibleNode *next; B32 v;};
typedef struct UI_FocusActiveSetNode UI_FocusActiveSetNode; struct UI_FocusActiveSetNode{UI_FocusActiveSetNode *next; B32 v;};
typedef struct UI_FocusActivePossibleNode UI_FocusActivePossibleNode; struct UI_FocusActivePossibleNode{UI_FocusActivePossibleNode *next; B32 v;};
#define UI_DeclStackNils \
struct\
{\
UI_ParentNode parent_nil_stack_top;\
UI_FlagsNode flags_nil_stack_top;\
UI_FixedXNode fixed_x_nil_stack_top;\
UI_FixedYNode fixed_y_nil_stack_top;\
UI_PrefWidthNode pref_width_nil_stack_top;\
UI_PrefHeightNode pref_height_nil_stack_top;\
UI_OpacityNode opacity_nil_stack_top;\
UI_TextColorNode text_color_nil_stack_top;\
UI_BackgroundColorNode background_color_nil_stack_top;\
UI_BorderColorNode border_color_nil_stack_top;\
UI_OverlayColorNode overlay_color_nil_stack_top;\
UI_FillColorNode fill_color_nil_stack_top;\
UI_CursorColorNode cursor_color_nil_stack_top;\
UI_CornerRadius00Node corner_radius_00_nil_stack_top;\
UI_CornerRadius01Node corner_radius_01_nil_stack_top;\
UI_CornerRadius10Node corner_radius_10_nil_stack_top;\
UI_CornerRadius11Node corner_radius_11_nil_stack_top;\
UI_BorderThicknessNode border_thickness_nil_stack_top;\
UI_Slice2F32Node slice2f32_nil_stack_top;\
UI_FontNode font_nil_stack_top;\
UI_FontSizeNode font_size_nil_stack_top;\
UI_ChildLayoutAxisNode child_layout_axis_nil_stack_top;\
UI_HoverCursorNode hover_cursor_nil_stack_top;\
UI_TextAlignNode text_align_nil_stack_top;\
UI_TextEdgePaddingNode text_edge_padding_nil_stack_top;\
UI_SeedKeyNode seed_key_nil_stack_top;\
UI_FocusHotSetNode focus_hot_set_nil_stack_top;\
UI_FocusHotPossibleNode focus_hot_possible_nil_stack_top;\
UI_FocusActiveSetNode focus_active_set_nil_stack_top;\
UI_FocusActivePossibleNode focus_active_possible_nil_stack_top;\
}
#define UI_InitStackNils(state) \
state->parent_nil_stack_top.v = &ui_g_nil_box;\
state->flags_nil_stack_top.v = 0;\
state->fixed_x_nil_stack_top.v = 0;\
state->fixed_y_nil_stack_top.v = 0;\
state->pref_width_nil_stack_top.v = UI_Pixels(200.f, 1.f);\
state->pref_height_nil_stack_top.v = UI_Pixels(2.f, 1.f);\
state->opacity_nil_stack_top.v = 1.f;\
state->text_color_nil_stack_top.v = V4F32(1, 1, 1, 1);\
state->background_color_nil_stack_top.v = V4F32(0.3f, 0.3f, 0.3f, 0.8f);\
state->border_color_nil_stack_top.v = V4F32(1, 1, 1, 0.3f);\
state->overlay_color_nil_stack_top.v = V4F32(0, 0, 0, 0);\
state->fill_color_nil_stack_top.v = V4F32(0.3f, 0.7f, 0.9f, 0.4f);\
state->cursor_color_nil_stack_top.v = V4F32(0.4f, 0.9f, 0.3f, 0.8f);\
state->corner_radius_00_nil_stack_top.v = 0;\
state->corner_radius_01_nil_stack_top.v = 0;\
state->corner_radius_10_nil_stack_top.v = 0;\
state->corner_radius_11_nil_stack_top.v = 0;\
state->border_thickness_nil_stack_top.v = 1;\
state->slice2f32_nil_stack_top.v = (R_Slice2F32){0};\
state->font_nil_stack_top.v = F_TagFromHash(ui_g_mono_font_hash);\
state->font_size_nil_stack_top.v = 16.f;\
state->child_layout_axis_nil_stack_top.v = Axis2_X;\
state->hover_cursor_nil_stack_top.v = OS_CursorKind_Null;\
state->text_align_nil_stack_top.v = UI_TextAlignment_Left;\
state->text_edge_padding_nil_stack_top.v = 0.f;\
state->seed_key_nil_stack_top.v = UI_KeyZero();\
state->focus_hot_set_nil_stack_top.v = 0;\
state->focus_hot_possible_nil_stack_top.v = 0;\
state->focus_active_set_nil_stack_top.v = 0;\
state->focus_active_possible_nil_stack_top.v = 0;\

#define UI_DeclStacks \
struct\
{\
struct { UI_ParentNode *top; UI_ParentNode *free; B32 auto_pop; } parent_stack;\
struct { UI_FlagsNode *top; UI_FlagsNode *free; B32 auto_pop; } flags_stack;\
struct { UI_FixedXNode *top; UI_FixedXNode *free; B32 auto_pop; } fixed_x_stack;\
struct { UI_FixedYNode *top; UI_FixedYNode *free; B32 auto_pop; } fixed_y_stack;\
struct { UI_PrefWidthNode *top; UI_PrefWidthNode *free; B32 auto_pop; } pref_width_stack;\
struct { UI_PrefHeightNode *top; UI_PrefHeightNode *free; B32 auto_pop; } pref_height_stack;\
struct { UI_OpacityNode *top; UI_OpacityNode *free; B32 auto_pop; } opacity_stack;\
struct { UI_TextColorNode *top; UI_TextColorNode *free; B32 auto_pop; } text_color_stack;\
struct { UI_BackgroundColorNode *top; UI_BackgroundColorNode *free; B32 auto_pop; } background_color_stack;\
struct { UI_BorderColorNode *top; UI_BorderColorNode *free; B32 auto_pop; } border_color_stack;\
struct { UI_OverlayColorNode *top; UI_OverlayColorNode *free; B32 auto_pop; } overlay_color_stack;\
struct { UI_FillColorNode *top; UI_FillColorNode *free; B32 auto_pop; } fill_color_stack;\
struct { UI_CursorColorNode *top; UI_CursorColorNode *free; B32 auto_pop; } cursor_color_stack;\
struct { UI_CornerRadius00Node *top; UI_CornerRadius00Node *free; B32 auto_pop; } corner_radius_00_stack;\
struct { UI_CornerRadius01Node *top; UI_CornerRadius01Node *free; B32 auto_pop; } corner_radius_01_stack;\
struct { UI_CornerRadius10Node *top; UI_CornerRadius10Node *free; B32 auto_pop; } corner_radius_10_stack;\
struct { UI_CornerRadius11Node *top; UI_CornerRadius11Node *free; B32 auto_pop; } corner_radius_11_stack;\
struct { UI_BorderThicknessNode *top; UI_BorderThicknessNode *free; B32 auto_pop; } border_thickness_stack;\
struct { UI_Slice2F32Node *top; UI_Slice2F32Node *free; B32 auto_pop; } slice2f32_stack;\
struct { UI_FontNode *top; UI_FontNode *free; B32 auto_pop; } font_stack;\
struct { UI_FontSizeNode *top; UI_FontSizeNode *free; B32 auto_pop; } font_size_stack;\
struct { UI_ChildLayoutAxisNode *top; UI_ChildLayoutAxisNode *free; B32 auto_pop; } child_layout_axis_stack;\
struct { UI_HoverCursorNode *top; UI_HoverCursorNode *free; B32 auto_pop; } hover_cursor_stack;\
struct { UI_TextAlignNode *top; UI_TextAlignNode *free; B32 auto_pop; } text_align_stack;\
struct { UI_TextEdgePaddingNode *top; UI_TextEdgePaddingNode *free; B32 auto_pop; } text_edge_padding_stack;\
struct { UI_SeedKeyNode *top; UI_SeedKeyNode *free; B32 auto_pop; } seed_key_stack;\
struct { UI_FocusHotSetNode *top; UI_FocusHotSetNode *free; B32 auto_pop; } focus_hot_set_stack;\
struct { UI_FocusHotPossibleNode *top; UI_FocusHotPossibleNode *free; B32 auto_pop; } focus_hot_possible_stack;\
struct { UI_FocusActiveSetNode *top; UI_FocusActiveSetNode *free; B32 auto_pop; } focus_active_set_stack;\
struct { UI_FocusActivePossibleNode *top; UI_FocusActivePossibleNode *free; B32 auto_pop; } focus_active_possible_stack;\
}
#define UI_InitStacks(state) \
state->parent_stack.top = &state->parent_nil_stack_top; state->parent_stack.free = 0; state->parent_stack.auto_pop = 0;\
state->flags_stack.top = &state->flags_nil_stack_top; state->flags_stack.free = 0; state->flags_stack.auto_pop = 0;\
state->fixed_x_stack.top = &state->fixed_x_nil_stack_top; state->fixed_x_stack.free = 0; state->fixed_x_stack.auto_pop = 0;\
state->fixed_y_stack.top = &state->fixed_y_nil_stack_top; state->fixed_y_stack.free = 0; state->fixed_y_stack.auto_pop = 0;\
state->pref_width_stack.top = &state->pref_width_nil_stack_top; state->pref_width_stack.free = 0; state->pref_width_stack.auto_pop = 0;\
state->pref_height_stack.top = &state->pref_height_nil_stack_top; state->pref_height_stack.free = 0; state->pref_height_stack.auto_pop = 0;\
state->opacity_stack.top = &state->opacity_nil_stack_top; state->opacity_stack.free = 0; state->opacity_stack.auto_pop = 0;\
state->text_color_stack.top = &state->text_color_nil_stack_top; state->text_color_stack.free = 0; state->text_color_stack.auto_pop = 0;\
state->background_color_stack.top = &state->background_color_nil_stack_top; state->background_color_stack.free = 0; state->background_color_stack.auto_pop = 0;\
state->border_color_stack.top = &state->border_color_nil_stack_top; state->border_color_stack.free = 0; state->border_color_stack.auto_pop = 0;\
state->overlay_color_stack.top = &state->overlay_color_nil_stack_top; state->overlay_color_stack.free = 0; state->overlay_color_stack.auto_pop = 0;\
state->fill_color_stack.top = &state->fill_color_nil_stack_top; state->fill_color_stack.free = 0; state->fill_color_stack.auto_pop = 0;\
state->cursor_color_stack.top = &state->cursor_color_nil_stack_top; state->cursor_color_stack.free = 0; state->cursor_color_stack.auto_pop = 0;\
state->corner_radius_00_stack.top = &state->corner_radius_00_nil_stack_top; state->corner_radius_00_stack.free = 0; state->corner_radius_00_stack.auto_pop = 0;\
state->corner_radius_01_stack.top = &state->corner_radius_01_nil_stack_top; state->corner_radius_01_stack.free = 0; state->corner_radius_01_stack.auto_pop = 0;\
state->corner_radius_10_stack.top = &state->corner_radius_10_nil_stack_top; state->corner_radius_10_stack.free = 0; state->corner_radius_10_stack.auto_pop = 0;\
state->corner_radius_11_stack.top = &state->corner_radius_11_nil_stack_top; state->corner_radius_11_stack.free = 0; state->corner_radius_11_stack.auto_pop = 0;\
state->border_thickness_stack.top = &state->border_thickness_nil_stack_top; state->border_thickness_stack.free = 0; state->border_thickness_stack.auto_pop = 0;\
state->slice2f32_stack.top = &state->slice2f32_nil_stack_top; state->slice2f32_stack.free = 0; state->slice2f32_stack.auto_pop = 0;\
state->font_stack.top = &state->font_nil_stack_top; state->font_stack.free = 0; state->font_stack.auto_pop = 0;\
state->font_size_stack.top = &state->font_size_nil_stack_top; state->font_size_stack.free = 0; state->font_size_stack.auto_pop = 0;\
state->child_layout_axis_stack.top = &state->child_layout_axis_nil_stack_top; state->child_layout_axis_stack.free = 0; state->child_layout_axis_stack.auto_pop = 0;\
state->hover_cursor_stack.top = &state->hover_cursor_nil_stack_top; state->hover_cursor_stack.free = 0; state->hover_cursor_stack.auto_pop = 0;\
state->text_align_stack.top = &state->text_align_nil_stack_top; state->text_align_stack.free = 0; state->text_align_stack.auto_pop = 0;\
state->text_edge_padding_stack.top = &state->text_edge_padding_nil_stack_top; state->text_edge_padding_stack.free = 0; state->text_edge_padding_stack.auto_pop = 0;\
state->seed_key_stack.top = &state->seed_key_nil_stack_top; state->seed_key_stack.free = 0; state->seed_key_stack.auto_pop = 0;\
state->focus_hot_set_stack.top = &state->focus_hot_set_nil_stack_top; state->focus_hot_set_stack.free = 0; state->focus_hot_set_stack.auto_pop = 0;\
state->focus_hot_possible_stack.top = &state->focus_hot_possible_nil_stack_top; state->focus_hot_possible_stack.free = 0; state->focus_hot_possible_stack.auto_pop = 0;\
state->focus_active_set_stack.top = &state->focus_active_set_nil_stack_top; state->focus_active_set_stack.free = 0; state->focus_active_set_stack.auto_pop = 0;\
state->focus_active_possible_stack.top = &state->focus_active_possible_nil_stack_top; state->focus_active_possible_stack.free = 0; state->focus_active_possible_stack.auto_pop = 0;\

#define UI_AutoPopStacks(state) \
if(state->parent_stack.auto_pop) { UI_PopParent(); state->parent_stack.auto_pop = 0; }\
if(state->flags_stack.auto_pop) { UI_PopFlags(); state->flags_stack.auto_pop = 0; }\
if(state->fixed_x_stack.auto_pop) { UI_PopFixedX(); state->fixed_x_stack.auto_pop = 0; }\
if(state->fixed_y_stack.auto_pop) { UI_PopFixedY(); state->fixed_y_stack.auto_pop = 0; }\
if(state->pref_width_stack.auto_pop) { UI_PopPrefWidth(); state->pref_width_stack.auto_pop = 0; }\
if(state->pref_height_stack.auto_pop) { UI_PopPrefHeight(); state->pref_height_stack.auto_pop = 0; }\
if(state->opacity_stack.auto_pop) { UI_PopOpacity(); state->opacity_stack.auto_pop = 0; }\
if(state->text_color_stack.auto_pop) { UI_PopTextColor(); state->text_color_stack.auto_pop = 0; }\
if(state->background_color_stack.auto_pop) { UI_PopBackgroundColor(); state->background_color_stack.auto_pop = 0; }\
if(state->border_color_stack.auto_pop) { UI_PopBorderColor(); state->border_color_stack.auto_pop = 0; }\
if(state->overlay_color_stack.auto_pop) { UI_PopOverlayColor(); state->overlay_color_stack.auto_pop = 0; }\
if(state->fill_color_stack.auto_pop) { UI_PopFillColor(); state->fill_color_stack.auto_pop = 0; }\
if(state->cursor_color_stack.auto_pop) { UI_PopCursorColor(); state->cursor_color_stack.auto_pop = 0; }\
if(state->corner_radius_00_stack.auto_pop) { UI_PopCornerRadius00(); state->corner_radius_00_stack.auto_pop = 0; }\
if(state->corner_radius_01_stack.auto_pop) { UI_PopCornerRadius01(); state->corner_radius_01_stack.auto_pop = 0; }\
if(state->corner_radius_10_stack.auto_pop) { UI_PopCornerRadius10(); state->corner_radius_10_stack.auto_pop = 0; }\
if(state->corner_radius_11_stack.auto_pop) { UI_PopCornerRadius11(); state->corner_radius_11_stack.auto_pop = 0; }\
if(state->border_thickness_stack.auto_pop) { UI_PopBorderThickness(); state->border_thickness_stack.auto_pop = 0; }\
if(state->slice2f32_stack.auto_pop) { UI_PopSlice2F32(); state->slice2f32_stack.auto_pop = 0; }\
if(state->font_stack.auto_pop) { UI_PopFont(); state->font_stack.auto_pop = 0; }\
if(state->font_size_stack.auto_pop) { UI_PopFontSize(); state->font_size_stack.auto_pop = 0; }\
if(state->child_layout_axis_stack.auto_pop) { UI_PopChildLayoutAxis(); state->child_layout_axis_stack.auto_pop = 0; }\
if(state->hover_cursor_stack.auto_pop) { UI_PopHoverCursor(); state->hover_cursor_stack.auto_pop = 0; }\
if(state->text_align_stack.auto_pop) { UI_PopTextAlign(); state->text_align_stack.auto_pop = 0; }\
if(state->text_edge_padding_stack.auto_pop) { UI_PopTextEdgePadding(); state->text_edge_padding_stack.auto_pop = 0; }\
if(state->seed_key_stack.auto_pop) { UI_PopSeedKey(); state->seed_key_stack.auto_pop = 0; }\
if(state->focus_hot_set_stack.auto_pop) { UI_PopFocusHotSet(); state->focus_hot_set_stack.auto_pop = 0; }\
if(state->focus_hot_possible_stack.auto_pop) { UI_PopFocusHotPossible(); state->focus_hot_possible_stack.auto_pop = 0; }\
if(state->focus_active_set_stack.auto_pop) { UI_PopFocusActiveSet(); state->focus_active_set_stack.auto_pop = 0; }\
if(state->focus_active_possible_stack.auto_pop) { UI_PopFocusActivePossible(); state->focus_active_possible_stack.auto_pop = 0; }\

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

extern String8 ui_g_icon_font;
extern U8 ui_g_icon_font_data[14416];
extern U128 ui_g_icon_font_hash;
read_only String8 ui_g_icon_font = {ui_g_icon_font_data, sizeof(ui_g_icon_font_data)};

extern String8 ui_g_mono_font;
extern U8 ui_g_mono_font_data[108168];
extern U128 ui_g_mono_font_hash;
read_only String8 ui_g_mono_font = {ui_g_mono_font_data, sizeof(ui_g_mono_font_data)};


#endif // UI_META_H
