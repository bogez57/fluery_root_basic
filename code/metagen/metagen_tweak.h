/* date = September 13th 2023 9:17 pm */

#ifndef METAGEN_TWEAK_H
#define METAGEN_TWEAK_H

typedef struct MG_TWK_Tweak MG_TWK_Tweak;
struct MG_TWK_Tweak
{
 MG_TWK_Tweak *next;
 String8 kind;
 String8 name;
 String8 default_val;
 String8 min;
 String8 max;
};

typedef struct MG_TWK_TweakList MG_TWK_TweakList;
struct MG_TWK_TweakList
{
 MG_TWK_Tweak *first;
 MG_TWK_Tweak *last;
 U64 count;
};

typedef struct MG_TWK_Layer MG_TWK_Layer;
struct MG_TWK_Layer
{
 MG_TWK_Layer *next;
 String8 name;
 String8 path;
 MG_TWK_TweakList tweak_b32s;
 MG_TWK_TweakList tweak_f32s;
};

function void MG_TWK_Generate(MG_CFileList *c_files);

#endif //METAGEN_TWEAK_H
