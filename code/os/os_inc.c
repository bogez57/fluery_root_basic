#include "os_core.c"
#if defined(OS_FEATURE_GFX)
#include "os_gfx.c"
#endif

#if OS_WINDOWS
#include "win32/os_core_win32.c"
#if defined(OS_FEATURE_GFX)
#include "win32/os_gfx_win32.c"
#endif
#else
#error OS layer not implemented.
#endif
