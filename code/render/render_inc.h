/* date = July 19th 2022 9:15 pm */

#ifndef RENDER_INC_H
#define RENDER_INC_H

#define R_BACKEND_D3D11   1

#if !defined(R_BACKEND)
# if OS_WINDOWS
#  define R_BACKEND R_BACKEND_D3D11
# else
#  error There is no default rendering backend for this operating system.
# endif
#endif

#include "render_types.h"
#include "render_core.h"

#endif // RENDER_INC_H
