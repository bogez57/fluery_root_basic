/* date = July 19th 2022 9:15 pm */

#ifndef RENDER_INC_H
#define RENDER_INC_H

////////////////////////////////
//~ rjf: Backend Constants

#define R_BACKEND_D3D11   1

////////////////////////////////
//~ rjf: Pick Backend

#if !defined(R_BACKEND)
# if OS_WINDOWS
#  define R_BACKEND R_BACKEND_D3D11
# else
#  error There is no default rendering backend for this operating system.
# endif
#endif

////////////////////////////////
//~ rjf: Main Includes

#include "render_types.h"
#include "render_core.h"

////////////////////////////////
//~ rjf: (D3D11) C++ Build -> Directly Include

#if R_BACKEND == R_BACKEND_D3D11
# if LANG_CPP
#  include "render_d3d11.h"
# endif
#endif

#endif // RENDER_INC_H
