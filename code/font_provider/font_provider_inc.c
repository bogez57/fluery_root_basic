////////////////////////////////
//~ rjf: Main Includes

#include "font_provider_core.c"

////////////////////////////////
//~ rjf: (DWrite) C++ Build -> Directly Include

#if FP_BACKEND == FP_BACKEND_DWRITE
# if LANG_CPP
#  include "dwrite/font_provider_dwrite.cpp"
# endif
#endif
