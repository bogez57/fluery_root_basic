/* date = August 16th 2022 8:53 pm */

#ifndef UI_TEXT_INPUT_H
#define UI_TEXT_INPUT_H

typedef U32 UI_TxtActionFlags;
enum
{
 UI_TxtActionFlag_Good              = (1<<0),
 UI_TxtActionFlag_KeepMark          = (1<<1),
 UI_TxtActionFlag_WordScan          = (1<<2),
 UI_TxtActionFlag_ZeroDeltaOnSelect = (1<<3),
 UI_TxtActionFlag_PickSideOnSelect  = (1<<4),
 UI_TxtActionFlag_Delete            = (1<<5),
 UI_TxtActionFlag_Copy              = (1<<6),
 UI_TxtActionFlag_Paste             = (1<<7),
};

typedef struct UI_TxtAction UI_TxtAction;
struct UI_TxtAction
{
 UI_TxtActionFlags flags;
 Vec2S64 delta;
 U32 codepoint;
};

typedef struct UI_TxtActionNode UI_TxtActionNode;
struct UI_TxtActionNode
{
 UI_TxtActionNode *next;
 UI_TxtAction action;
};

typedef struct UI_TxtActionList UI_TxtActionList;
struct UI_TxtActionList
{
 UI_TxtActionNode *first;
 UI_TxtActionNode *last;
 U64 count;
};

typedef struct UI_TxtOp UI_TxtOp;
struct UI_TxtOp
{
 String8 string;
 TxtRng replaced_range;
 TxtRng copy_range;
 TxtPt cursor;
 TxtPt mark;
};

root_function B32 UI_ASCIICharIsBoundary(U8 c);
root_function void UI_TxtActionListPush(Arena *arena, UI_TxtActionList *list, UI_TxtAction action);
root_function UI_TxtAction UI_TxtActionFromEvent(OS_Event *event);
root_function UI_TxtActionList UI_TxtActionListFromEatenEvents(Arena *arena, OS_Handle window, OS_EventList *events, B32 multiline);
root_function UI_TxtOp UI_SingleLineTxtOpFromTxtAction(Arena *arena, TxtPt cursor, TxtPt mark, String8 string, UI_TxtAction *action);
root_function String8 UI_PushStringReplaceRange(Arena *arena, String8 string, Rng1U64 range, String8 replace);

#endif // UI_TEXT_INPUT_H
