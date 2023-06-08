/* date = June 2nd 2023 8:24 am */

#ifndef BASE_COMMAND_LINE_H
#define BASE_COMMAND_LINE_H

typedef struct CmdLineOptNode CmdLineOptNode;
struct CmdLineOptNode
{
 CmdLineOptNode *next;
 String8 name;
 String8List values;
 String8 value;
};

typedef struct CmdLineOptSlot CmdLineOptSlot;
struct CmdLineOptSlot
{
 CmdLineOptNode *first;
 CmdLineOptNode *last;
};

typedef struct CmdLine CmdLine;
struct CmdLine
{
 U64 slots_count;
 CmdLineOptSlot *slots;
 String8List inputs;
};

core_function U64 CmdLineHashFromString(String8 string);
core_function CmdLine CmdLineFromStringList(Arena *arena, String8List strings);
core_function String8List CmdLineOptStrings(CmdLine *cmdln, String8 name);
core_function String8 CmdLineOptString(CmdLine *cmdln, String8 name);
core_function B32 CmdLineOptB32(CmdLine *cmdln, String8 name);
core_function F64 CmdLineOptF64(CmdLine *cmdln, String8 name);
core_function S64 CmdLineOptS64(CmdLine *cmdln, String8 name);

#endif // BASE_COMMAND_LINE_H
