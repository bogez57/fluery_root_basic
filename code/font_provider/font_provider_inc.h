/* date = November 20th 2022 9:16 pm */

#ifndef FONT_PROVIDER_INC_H
#define FONT_PROVIDER_INC_H

////////////////////////////////
//~ rjf: Backend Constants

#define FP_BACKEND_DWRITE 1

////////////////////////////////
//~ rjf: Pick Backend

#if !defined(FP_BACKEND)
# if OS_WINDOWS
#  define FP_BACKEND FP_BACKEND_DWRITE
# else
#  error There is no default font provider backend for this operating system.
# endif
#endif

////////////////////////////////
//~ rjf: Main Includes

#include "font_provider_core.h"

////////////////////////////////
//~ rjf: (DWrite) C++ Build -> Directly Include

#if FP_BACKEND == FP_BACKEND_DWRITE
# if LANG_CPP
#  include "dwrite/font_provider_dwrite.h"
# endif
#endif

#endif // FONT_PROVIDER_INC_H
