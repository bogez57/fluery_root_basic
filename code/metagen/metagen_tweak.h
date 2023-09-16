/* date = September 13th 2023 9:17 pm */

#ifndef METAGEN_TWEAK_H
#define METAGEN_TWEAK_H

typedef struct MG_TWK_Arg MG_TWK_Arg;
struct MG_TWK_Arg
{
 MG_TWK_Arg *next;
 CL_TokenPtrList tokens;
};

typedef struct MG_TWK_Tweak MG_TWK_Tweak;
struct MG_TWK_Tweak
{
 MG_TWK_Tweak *next;
 String8 name;
 String8 default_val;
};

typedef struct MG_TWK_Layer MG_TWK_Layer;
struct MG_TWK_Layer
{
 MG_TWK_Layer *next;
 String8 name;
 String8 path;
 MG_TWK_Tweak *first_tweak;
 MG_TWK_Tweak *last_tweak;
};

function U64 MG_TWK_HashFromString(String8 string);
function void MG_TWK_Generate(MG_CFileList *c_files);

#endif //METAGEN_TWEAK_H
