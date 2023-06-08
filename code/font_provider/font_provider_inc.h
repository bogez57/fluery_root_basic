/* date = November 20th 2022 9:16 pm */

#ifndef FONT_PROVIDER_INC_H
#define FONT_PROVIDER_INC_H

#define FP_BACKEND_DWRITE 1

#if !defined(FP_BACKEND)
# if OS_WINDOWS
#  define FP_BACKEND FP_BACKEND_DWRITE
# else
#  error There is no default font provider backend for this operating system.
# endif
#endif

#include "font_provider_core.h"

#endif // FONT_PROVIDER_INC_H
