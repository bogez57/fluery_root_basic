#ifndef OS_GFX_H
#define OS_GFX_H

////////////////////////////////
//~ rjf: Receipts

typedef struct OS_InitGfxReceipt OS_InitGfxReceipt;
struct OS_InitGfxReceipt
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Generated Code

#include "generated/os_gfx.meta.h"

////////////////////////////////
//~ rjf: Events

typedef enum OS_EventKind
{
 OS_EventKind_Null,
 OS_EventKind_WindowClose,
 OS_EventKind_WindowLoseFocus,
 OS_EventKind_Press,
 OS_EventKind_Release,
 OS_EventKind_Text,
 OS_EventKind_MouseScroll,
 OS_EventKind_DropFile,
 OS_EventKind_COUNT
}
OS_EventKind;

typedef U32 OS_Modifiers;
enum
{
 OS_Modifier_Ctrl  = (1<<0),
 OS_Modifier_Shift = (1<<1),
 OS_Modifier_Alt   = (1<<2),
};

typedef struct OS_Event OS_Event;
struct OS_Event
{
 OS_Event *next;
 OS_Event *prev;
 OS_Handle window;
 OS_EventKind kind;
 OS_Modifiers modifiers;
 OS_Key key;
 U32 character;
 Vec2F32 position;
 Vec2F32 scroll;
 String8 path;
};

typedef struct OS_EventList OS_EventList;
struct OS_EventList
{
 OS_Event *first;
 OS_Event *last;
 U64 count;
};

////////////////////////////////
//~ rjf: Cursors

typedef enum OS_CursorKind
{
 OS_CursorKind_Null,
 OS_CursorKind_Hidden,
 OS_CursorKind_Pointer,
 OS_CursorKind_Hand,
 OS_CursorKind_WestEast,
 OS_CursorKind_NorthSouth,
 OS_CursorKind_NorthEastSouthWest,
 OS_CursorKind_NorthWestSouthEast,
 OS_CursorKind_AllCardinalDirections,
 OS_CursorKind_IBar,
 OS_CursorKind_Blocked,
 OS_CursorKind_Loading,
 OS_CursorKind_Pan,
 OS_CursorKind_COUNT
}
OS_CursorKind;

////////////////////////////////
//~ rjf: Window Flags

typedef U32 OS_WindowFlags;
enum
{
 OS_WindowFlag_CustomBorder = (1<<0),
};

////////////////////////////////
//~ rjf: Hooks

typedef void OS_RepaintFunction(void);

////////////////////////////////
//~ rjf: Basic Helpers

root_function String8 OS_StringFromKey(OS_Key key);
root_function String8 OS_StringFromModifiersKey(Arena *arena, OS_Modifiers modifiers, OS_Key key);

////////////////////////////////
//~ rjf: Event Helpers

root_function U64           OS_CharacterFromModifiersAndKey(OS_Modifiers modifiers, OS_Key key);
root_function OS_CursorKind OS_CursorKindFromResizeSides(Side x, Side y);
root_function String8       OS_StringFromEvent(Arena *arena, OS_Event *event);
root_function B32           OS_KeyPress(OS_EventList *events, OS_Handle window, OS_Key key, OS_Modifiers mods);
root_function B32           OS_KeyRelease(OS_EventList *events, OS_Handle window, OS_Key key, OS_Modifiers mods);
root_function B32           OS_TextCodepoint(OS_EventList *events, OS_Handle window, U32 codepoint);

////////////////////////////////
//~ rjf: @os_per_backend Main API

root_function OS_InitGfxReceipt OS_InitGfx(OS_InitReceipt os_init_receipt);

////////////////////////////////
//~ rjf: @os_per_backend System Info

root_function F32 OS_DefaultRefreshRate(void);

////////////////////////////////
//~ rjf: @os_per_backend Windows

//- rjf: open/closing/metadata
root_function OS_Handle OS_WindowOpen(OS_WindowFlags flags, Vec2S64 size, String8 title);
root_function void      OS_WindowClose(OS_Handle handle);
root_function void      OS_WindowSetTitle(OS_Handle handle, String8 title);
root_function void      OS_WindowSetIcon(OS_Handle handle, Vec2S32 size, String8 rgba_data);
root_function void      OS_WindowSetRepaint(OS_Handle handle, OS_RepaintFunction *repaint);

//- rjf: per-paint custom border hit-testing info
root_function void      OS_WindowClearCustomBorderData(OS_Handle handle);
root_function void      OS_WindowPushCustomTitleBar(OS_Handle handle, F32 thickness);
root_function void      OS_WindowPushCustomEdges(OS_Handle handle, F32 thickness);
root_function void      OS_WindowPushCustomTitleBarClientArea(OS_Handle handle, Rng2F32 rect);

//- rjf: minimizing/maximizing
root_function B32       OS_WindowIsMaximized(OS_Handle handle);
root_function void      OS_WindowMinimize(OS_Handle handle);
root_function void      OS_WindowMaximize(OS_Handle handle);
root_function void      OS_WindowRestore(OS_Handle handle);

//- rjf: focusing
root_function B32       OS_WindowIsFocused(OS_Handle handle);

//- rjf: fullscreen
root_function B32       OS_WindowIsFullscreen(OS_Handle handle);
root_function void      OS_WindowToggleFullscreen(OS_Handle handle);

//- rjf: first paint
root_function void      OS_WindowFirstPaint(OS_Handle handle);

//- rjf: accessors
root_function Rng2F32   OS_RectFromWindow(OS_Handle handle);
root_function Rng2F32   OS_ClientRectFromWindow(OS_Handle handle);
root_function F32       OS_DPIFromWindow(OS_Handle handle);
root_function Vec2F32   OS_MouseFromWindow(OS_Handle handle);

////////////////////////////////
//~ rjf: @os_per_backend Events

root_function OS_Modifiers OS_GetModifiers(void);
root_function OS_EventList OS_GetEvents(Arena *arena);
root_function void OS_EatEvent(OS_EventList *events, OS_Event *event);

////////////////////////////////
//~ rjf: @os_per_backend Cursors

root_function void OS_SetCursor(OS_CursorKind kind);

////////////////////////////////
//~ rjf: @os_per_backend Clipboard

root_function void OS_SetClipboardText(String8 string);
root_function String8 OS_GetClipboardText(Arena *arena);

#endif // OS_GFX_H
