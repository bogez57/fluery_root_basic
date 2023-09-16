#include "metagen_os_core.c"

#if OS_WINDOWS
# include "metagen/metagen_os/win32/metagen_os_core_win32.c"
#else
# error OS layer not implemented.
#endif
