////////////////////////////////
//~ rjf: Main Includes

#include "render_types.c"
#include "render_core.c"

////////////////////////////////
//~ rjf: (D3D11) C++ Build -> Directly Include

#if R_BACKEND == R_BACKEND_D3D11
# if LANG_CPP
#  include "render_d3d11.cpp"
# endif
#endif
