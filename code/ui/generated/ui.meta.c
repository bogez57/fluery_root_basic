#if BUILD_CORE
String8 ui_g_icon_kind_string_table[49] =
{
Str8LitComp(" "),
Str8LitComp("X"),
Str8LitComp("."),
Str8LitComp("*"),
Str8LitComp("8"),
Str8LitComp("+"),
Str8LitComp("e"),
Str8LitComp("3"),
Str8LitComp("M"),
Str8LitComp("N"),
Str8LitComp("f"),
Str8LitComp("F"),
Str8LitComp("t"),
Str8LitComp("i"),
Str8LitComp(")"),
Str8LitComp("z"),
Str8LitComp("@"),
Str8LitComp("s"),
Str8LitComp("S"),
Str8LitComp("Z"),
Str8LitComp("d"),
Str8LitComp("u"),
Str8LitComp("l"),
Str8LitComp("r"),
Str8LitComp("<"),
Str8LitComp(">"),
Str8LitComp("^"),
Str8LitComp("v"),
Str8LitComp("R"),
Str8LitComp("{"),
Str8LitComp("}"),
Str8LitComp("m"),
Str8LitComp("x"),
Str8LitComp("w"),
Str8LitComp("b"),
Str8LitComp("g"),
Str8LitComp("p"),
Str8LitComp("q"),
Str8LitComp("T"),
Str8LitComp("D"),
Str8LitComp("G"),
Str8LitComp("k"),
Str8LitComp("!"),
Str8LitComp("1"),
Str8LitComp("W"),
Str8LitComp("?"),
Str8LitComp("I"),
Str8LitComp("V"),
Str8LitComp("H"),
};
#endif // BUILD_CORE

#if 0
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
#endif

root_function UI_Box * UI_TopParent(void) { UI_StackTopImpl(ui_state, Parent, parent) }
root_function UI_BoxFlags UI_TopFlags(void) { UI_StackTopImpl(ui_state, Flags, flags) }
root_function F32 UI_TopFixedX(void) { UI_StackTopImpl(ui_state, FixedX, fixed_x) }
root_function F32 UI_TopFixedY(void) { UI_StackTopImpl(ui_state, FixedY, fixed_y) }
root_function UI_Size UI_TopPrefWidth(void) { UI_StackTopImpl(ui_state, PrefWidth, pref_width) }
root_function UI_Size UI_TopPrefHeight(void) { UI_StackTopImpl(ui_state, PrefHeight, pref_height) }
root_function F32 UI_TopOpacity(void) { UI_StackTopImpl(ui_state, Opacity, opacity) }
root_function Vec4F32 UI_TopTextColor(void) { UI_StackTopImpl(ui_state, TextColor, text_color) }
root_function Vec4F32 UI_TopBackgroundColor(void) { UI_StackTopImpl(ui_state, BackgroundColor, background_color) }
root_function Vec4F32 UI_TopBorderColor(void) { UI_StackTopImpl(ui_state, BorderColor, border_color) }
root_function Vec4F32 UI_TopOverlayColor(void) { UI_StackTopImpl(ui_state, OverlayColor, overlay_color) }
root_function Vec4F32 UI_TopFillColor(void) { UI_StackTopImpl(ui_state, FillColor, fill_color) }
root_function Vec4F32 UI_TopCursorColor(void) { UI_StackTopImpl(ui_state, CursorColor, cursor_color) }
root_function F32 UI_TopCornerRadius00(void) { UI_StackTopImpl(ui_state, CornerRadius00, corner_radius_00) }
root_function F32 UI_TopCornerRadius01(void) { UI_StackTopImpl(ui_state, CornerRadius01, corner_radius_01) }
root_function F32 UI_TopCornerRadius10(void) { UI_StackTopImpl(ui_state, CornerRadius10, corner_radius_10) }
root_function F32 UI_TopCornerRadius11(void) { UI_StackTopImpl(ui_state, CornerRadius11, corner_radius_11) }
root_function F32 UI_TopBorderThickness(void) { UI_StackTopImpl(ui_state, BorderThickness, border_thickness) }
root_function R_Slice2F32 UI_TopSlice2F32(void) { UI_StackTopImpl(ui_state, Slice2F32, slice2f32) }
root_function F_Tag UI_TopFont(void) { UI_StackTopImpl(ui_state, Font, font) }
root_function F32 UI_TopFontSize(void) { UI_StackTopImpl(ui_state, FontSize, font_size) }
root_function Axis2 UI_TopChildLayoutAxis(void) { UI_StackTopImpl(ui_state, ChildLayoutAxis, child_layout_axis) }
root_function OS_CursorKind UI_TopHoverCursor(void) { UI_StackTopImpl(ui_state, HoverCursor, hover_cursor) }
root_function UI_TextAlignment UI_TopTextAlign(void) { UI_StackTopImpl(ui_state, TextAlign, text_align) }
root_function F32 UI_TopTextEdgePadding(void) { UI_StackTopImpl(ui_state, TextEdgePadding, text_edge_padding) }
root_function UI_Key UI_TopSeedKey(void) { UI_StackTopImpl(ui_state, SeedKey, seed_key) }
root_function UI_Box * UI_PushParent(UI_Box * v) { UI_StackPushImpl(ui_state, Parent, parent, UI_Box *, v) }
root_function UI_BoxFlags UI_PushFlags(UI_BoxFlags v) { UI_StackPushImpl(ui_state, Flags, flags, UI_BoxFlags, v) }
root_function F32 UI_PushFixedX(F32 v) { UI_StackPushImpl(ui_state, FixedX, fixed_x, F32, v) }
root_function F32 UI_PushFixedY(F32 v) { UI_StackPushImpl(ui_state, FixedY, fixed_y, F32, v) }
root_function UI_Size UI_PushPrefWidth(UI_Size v) { UI_StackPushImpl(ui_state, PrefWidth, pref_width, UI_Size, v) }
root_function UI_Size UI_PushPrefHeight(UI_Size v) { UI_StackPushImpl(ui_state, PrefHeight, pref_height, UI_Size, v) }
root_function F32 UI_PushOpacity(F32 v) { UI_StackPushImpl(ui_state, Opacity, opacity, F32, v) }
root_function Vec4F32 UI_PushTextColor(Vec4F32 v) { UI_StackPushImpl(ui_state, TextColor, text_color, Vec4F32, v) }
root_function Vec4F32 UI_PushBackgroundColor(Vec4F32 v) { UI_StackPushImpl(ui_state, BackgroundColor, background_color, Vec4F32, v) }
root_function Vec4F32 UI_PushBorderColor(Vec4F32 v) { UI_StackPushImpl(ui_state, BorderColor, border_color, Vec4F32, v) }
root_function Vec4F32 UI_PushOverlayColor(Vec4F32 v) { UI_StackPushImpl(ui_state, OverlayColor, overlay_color, Vec4F32, v) }
root_function Vec4F32 UI_PushFillColor(Vec4F32 v) { UI_StackPushImpl(ui_state, FillColor, fill_color, Vec4F32, v) }
root_function Vec4F32 UI_PushCursorColor(Vec4F32 v) { UI_StackPushImpl(ui_state, CursorColor, cursor_color, Vec4F32, v) }
root_function F32 UI_PushCornerRadius00(F32 v) { UI_StackPushImpl(ui_state, CornerRadius00, corner_radius_00, F32, v) }
root_function F32 UI_PushCornerRadius01(F32 v) { UI_StackPushImpl(ui_state, CornerRadius01, corner_radius_01, F32, v) }
root_function F32 UI_PushCornerRadius10(F32 v) { UI_StackPushImpl(ui_state, CornerRadius10, corner_radius_10, F32, v) }
root_function F32 UI_PushCornerRadius11(F32 v) { UI_StackPushImpl(ui_state, CornerRadius11, corner_radius_11, F32, v) }
root_function F32 UI_PushBorderThickness(F32 v) { UI_StackPushImpl(ui_state, BorderThickness, border_thickness, F32, v) }
root_function R_Slice2F32 UI_PushSlice2F32(R_Slice2F32 v) { UI_StackPushImpl(ui_state, Slice2F32, slice2f32, R_Slice2F32, v) }
root_function F_Tag UI_PushFont(F_Tag v) { UI_StackPushImpl(ui_state, Font, font, F_Tag, v) }
root_function F32 UI_PushFontSize(F32 v) { UI_StackPushImpl(ui_state, FontSize, font_size, F32, v) }
root_function Axis2 UI_PushChildLayoutAxis(Axis2 v) { UI_StackPushImpl(ui_state, ChildLayoutAxis, child_layout_axis, Axis2, v) }
root_function OS_CursorKind UI_PushHoverCursor(OS_CursorKind v) { UI_StackPushImpl(ui_state, HoverCursor, hover_cursor, OS_CursorKind, v) }
root_function UI_TextAlignment UI_PushTextAlign(UI_TextAlignment v) { UI_StackPushImpl(ui_state, TextAlign, text_align, UI_TextAlignment, v) }
root_function F32 UI_PushTextEdgePadding(F32 v) { UI_StackPushImpl(ui_state, TextEdgePadding, text_edge_padding, F32, v) }
root_function UI_Key UI_PushSeedKey(UI_Key v) { UI_StackPushImpl(ui_state, SeedKey, seed_key, UI_Key, v) }
root_function UI_Box * UI_PopParent(void) { UI_StackPopImpl(ui_state, Parent, parent) }
root_function UI_BoxFlags UI_PopFlags(void) { UI_StackPopImpl(ui_state, Flags, flags) }
root_function F32 UI_PopFixedX(void) { UI_StackPopImpl(ui_state, FixedX, fixed_x) }
root_function F32 UI_PopFixedY(void) { UI_StackPopImpl(ui_state, FixedY, fixed_y) }
root_function UI_Size UI_PopPrefWidth(void) { UI_StackPopImpl(ui_state, PrefWidth, pref_width) }
root_function UI_Size UI_PopPrefHeight(void) { UI_StackPopImpl(ui_state, PrefHeight, pref_height) }
root_function F32 UI_PopOpacity(void) { UI_StackPopImpl(ui_state, Opacity, opacity) }
root_function Vec4F32 UI_PopTextColor(void) { UI_StackPopImpl(ui_state, TextColor, text_color) }
root_function Vec4F32 UI_PopBackgroundColor(void) { UI_StackPopImpl(ui_state, BackgroundColor, background_color) }
root_function Vec4F32 UI_PopBorderColor(void) { UI_StackPopImpl(ui_state, BorderColor, border_color) }
root_function Vec4F32 UI_PopOverlayColor(void) { UI_StackPopImpl(ui_state, OverlayColor, overlay_color) }
root_function Vec4F32 UI_PopFillColor(void) { UI_StackPopImpl(ui_state, FillColor, fill_color) }
root_function Vec4F32 UI_PopCursorColor(void) { UI_StackPopImpl(ui_state, CursorColor, cursor_color) }
root_function F32 UI_PopCornerRadius00(void) { UI_StackPopImpl(ui_state, CornerRadius00, corner_radius_00) }
root_function F32 UI_PopCornerRadius01(void) { UI_StackPopImpl(ui_state, CornerRadius01, corner_radius_01) }
root_function F32 UI_PopCornerRadius10(void) { UI_StackPopImpl(ui_state, CornerRadius10, corner_radius_10) }
root_function F32 UI_PopCornerRadius11(void) { UI_StackPopImpl(ui_state, CornerRadius11, corner_radius_11) }
root_function F32 UI_PopBorderThickness(void) { UI_StackPopImpl(ui_state, BorderThickness, border_thickness) }
root_function R_Slice2F32 UI_PopSlice2F32(void) { UI_StackPopImpl(ui_state, Slice2F32, slice2f32) }
root_function F_Tag UI_PopFont(void) { UI_StackPopImpl(ui_state, Font, font) }
root_function F32 UI_PopFontSize(void) { UI_StackPopImpl(ui_state, FontSize, font_size) }
root_function Axis2 UI_PopChildLayoutAxis(void) { UI_StackPopImpl(ui_state, ChildLayoutAxis, child_layout_axis) }
root_function OS_CursorKind UI_PopHoverCursor(void) { UI_StackPopImpl(ui_state, HoverCursor, hover_cursor) }
root_function UI_TextAlignment UI_PopTextAlign(void) { UI_StackPopImpl(ui_state, TextAlign, text_align) }
root_function F32 UI_PopTextEdgePadding(void) { UI_StackPopImpl(ui_state, TextEdgePadding, text_edge_padding) }
root_function UI_Key UI_PopSeedKey(void) { UI_StackPopImpl(ui_state, SeedKey, seed_key) }
root_function UI_Box * UI_SetNextParent(UI_Box * v) { UI_StackSetNextImpl(ui_state, Parent, parent, UI_Box *, v) }
root_function UI_BoxFlags UI_SetNextFlags(UI_BoxFlags v) { UI_StackSetNextImpl(ui_state, Flags, flags, UI_BoxFlags, v) }
root_function F32 UI_SetNextFixedX(F32 v) { UI_StackSetNextImpl(ui_state, FixedX, fixed_x, F32, v) }
root_function F32 UI_SetNextFixedY(F32 v) { UI_StackSetNextImpl(ui_state, FixedY, fixed_y, F32, v) }
root_function UI_Size UI_SetNextPrefWidth(UI_Size v) { UI_StackSetNextImpl(ui_state, PrefWidth, pref_width, UI_Size, v) }
root_function UI_Size UI_SetNextPrefHeight(UI_Size v) { UI_StackSetNextImpl(ui_state, PrefHeight, pref_height, UI_Size, v) }
root_function F32 UI_SetNextOpacity(F32 v) { UI_StackSetNextImpl(ui_state, Opacity, opacity, F32, v) }
root_function Vec4F32 UI_SetNextTextColor(Vec4F32 v) { UI_StackSetNextImpl(ui_state, TextColor, text_color, Vec4F32, v) }
root_function Vec4F32 UI_SetNextBackgroundColor(Vec4F32 v) { UI_StackSetNextImpl(ui_state, BackgroundColor, background_color, Vec4F32, v) }
root_function Vec4F32 UI_SetNextBorderColor(Vec4F32 v) { UI_StackSetNextImpl(ui_state, BorderColor, border_color, Vec4F32, v) }
root_function Vec4F32 UI_SetNextOverlayColor(Vec4F32 v) { UI_StackSetNextImpl(ui_state, OverlayColor, overlay_color, Vec4F32, v) }
root_function Vec4F32 UI_SetNextFillColor(Vec4F32 v) { UI_StackSetNextImpl(ui_state, FillColor, fill_color, Vec4F32, v) }
root_function Vec4F32 UI_SetNextCursorColor(Vec4F32 v) { UI_StackSetNextImpl(ui_state, CursorColor, cursor_color, Vec4F32, v) }
root_function F32 UI_SetNextCornerRadius00(F32 v) { UI_StackSetNextImpl(ui_state, CornerRadius00, corner_radius_00, F32, v) }
root_function F32 UI_SetNextCornerRadius01(F32 v) { UI_StackSetNextImpl(ui_state, CornerRadius01, corner_radius_01, F32, v) }
root_function F32 UI_SetNextCornerRadius10(F32 v) { UI_StackSetNextImpl(ui_state, CornerRadius10, corner_radius_10, F32, v) }
root_function F32 UI_SetNextCornerRadius11(F32 v) { UI_StackSetNextImpl(ui_state, CornerRadius11, corner_radius_11, F32, v) }
root_function F32 UI_SetNextBorderThickness(F32 v) { UI_StackSetNextImpl(ui_state, BorderThickness, border_thickness, F32, v) }
root_function R_Slice2F32 UI_SetNextSlice2F32(R_Slice2F32 v) { UI_StackSetNextImpl(ui_state, Slice2F32, slice2f32, R_Slice2F32, v) }
root_function F_Tag UI_SetNextFont(F_Tag v) { UI_StackSetNextImpl(ui_state, Font, font, F_Tag, v) }
root_function F32 UI_SetNextFontSize(F32 v) { UI_StackSetNextImpl(ui_state, FontSize, font_size, F32, v) }
root_function Axis2 UI_SetNextChildLayoutAxis(Axis2 v) { UI_StackSetNextImpl(ui_state, ChildLayoutAxis, child_layout_axis, Axis2, v) }
root_function OS_CursorKind UI_SetNextHoverCursor(OS_CursorKind v) { UI_StackSetNextImpl(ui_state, HoverCursor, hover_cursor, OS_CursorKind, v) }
root_function UI_TextAlignment UI_SetNextTextAlign(UI_TextAlignment v) { UI_StackSetNextImpl(ui_state, TextAlign, text_align, UI_TextAlignment, v) }
root_function F32 UI_SetNextTextEdgePadding(F32 v) { UI_StackSetNextImpl(ui_state, TextEdgePadding, text_edge_padding, F32, v) }
root_function UI_Key UI_SetNextSeedKey(UI_Key v) { UI_StackSetNextImpl(ui_state, SeedKey, seed_key, UI_Key, v) }

