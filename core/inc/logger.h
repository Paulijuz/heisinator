#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void log_debug(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_error(const char *fmt, ...);
void log_fatal(const char *fmt, ...);

#define LOG_INT(x) static int x##_debug_state_storage; if(x##_debug_state_storage != x) { x##_debug_state_storage = x; log_debug("'%s': %d", #x, x); }