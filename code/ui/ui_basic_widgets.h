/* date = August 4th 2022 9:43 pm */

#ifndef UI_BASIC_WIDGETS_H
#define UI_BASIC_WIDGETS_H

////////////////////////////////
//~ rjf: Basic Types

typedef struct UI_SliderF32DrawData UI_SliderF32DrawData;
struct UI_SliderF32DrawData
{
 F32 pct_filled;
 Vec4F32 fill_color;
};

////////////////////////////////
//~ rjf: Basics

root_function UI_Signal UI_Label(String8 string);
root_function UI_Signal UI_LabelF(char *fmt, ...);

root_function UI_Signal UI_Image(R_Slice2F32 slice, String8 string);
root_function UI_Signal UI_ImageF(char *fmt, ...);

root_function void UI_Spacer(UI_Size size);
#define UI_Padding(size) DeferLoop(UI_Spacer(size), UI_Spacer(size))

root_function UI_Signal UI_Button(String8 string);
root_function UI_Signal UI_ButtonF(char *fmt, ...);

root_function UI_Signal UI_Check(B32 checked, String8 string);
root_function UI_Signal UI_CheckF(B32 checked, char *fmt, ...);

root_function UI_Signal UI_Radio(B32 selected, String8 string);
root_function UI_Signal UI_RadioF(B32 selected, char *fmt, ...);

root_function UI_Signal UI_Expander(B32 expanded, String8 string);
root_function UI_Signal UI_ExpanderF(B32 expanded, char *fmt, ...);

function UI_CUSTOM_DRAW_FUNCTION(UI_SliderF32Draw);
root_function UI_Signal UI_SliderF32(F32 *value, Rng1F32 range, String8 string);
root_function UI_Signal UI_SliderF32F(F32 *value, Rng1F32 range, char *fmt, ...);

root_function UI_Signal UI_LineEdit(TxtPt *cursor, TxtPt *mark, U64 buffer_size, U8 *buffer, U64 *string_size, String8 string);
root_function UI_Signal UI_LineEditF(TxtPt *cursor, TxtPt *mark, U64 buffer_size, U8 *buffer, U64 *string_size, char *fmt, ...);

root_function void UI_ColorPickerTooltipInfo(Vec3F32 hsv);
root_function UI_Signal UI_SatValPicker(F32 hue, F32 *out_sat, F32 *out_val, String8 string);
root_function UI_Signal UI_SatValPickerF(F32 hue, F32 *out_sat, F32 *out_val, char *fmt, ...);
root_function UI_Signal UI_HuePicker(F32 *out_hue, F32 sat, F32 val, String8 string);
root_function UI_Signal UI_HuePickerF(F32 *out_hue, F32 sat, F32 val, char *fmt, ...);

////////////////////////////////
//~ rjf: Layout Parents

root_function void UI_NamedColumnBegin(String8 string);
root_function void UI_NamedColumnBeginF(char *fmt, ...);
root_function void UI_ColumnBegin(void);
root_function void UI_ColumnEnd(void);
#define UI_NamedColumn(s) DeferLoop(UI_NamedColumnBegin(s), UI_ColumnEnd())
#define UI_NamedColumnF(...) DeferLoop(UI_NamedColumnBeginF(__VA_ARGS__), UI_ColumnEnd())
#define UI_Column DeferLoop(UI_ColumnBegin(), UI_ColumnEnd())

root_function void UI_NamedRowBegin(String8 string);
root_function void UI_NamedRowBeginF(char *fmt, ...);
root_function void UI_RowBegin(void);
root_function void UI_RowEnd(void);
#define UI_NamedRow(s) DeferLoop(UI_NamedRowBegin(s), UI_RowEnd())
#define UI_NamedRowF(...) DeferLoop(UI_NamedRowBeginF(__VA_ARGS__), UI_RowEnd())
#define UI_Row DeferLoop(UI_RowBegin(), UI_RowEnd())

#endif // UI_BASIC_WIDGETS_H
