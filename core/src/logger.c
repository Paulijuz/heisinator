#include "logger.h"

#define DEBUG   0
#define INFO    1
#define WARNING 2
#define ERROR   3
#define FATAL   4
#define NO_LOG  5

#define LOG_LEVEL INFO

void log_debug(const char *fmt, ...) {
    #if LOG_LEVEL <= DEBUG
        va_list args;
        va_start(args, fmt);

        time_t time_raw;
        time(&time_raw);

        struct tm *time_info = localtime(&time_raw);

        char time_string[10];
        strftime(time_string, 10, "%H:%M:%S", time_info);

        printf("\033[94m○\033[39m Debug   [%s]: ", time_string);
        vprintf(fmt, args);
        printf("\n");
        
        va_end(args);
    #endif
}

void log_info(const char *fmt, ...) {
    #if LOG_LEVEL <= INFO
        va_list args;
        va_start(args, fmt);

        time_t time_raw;
        time(&time_raw);

        struct tm *time_info = localtime(&time_raw);

        char time_string[10];
        strftime(time_string, 10, "%H:%M:%S", time_info);

        printf("\033[94m●\033[39m Info    [%s]: ", time_string);
        vprintf(fmt, args);
        printf("\n");
        
        va_end(args);
    #endif
}

void log_warning(const char *fmt, ...) {
    #if LOG_LEVEL <= WARNING
        va_list args;
        va_start(args, fmt);

        time_t time_raw;
        time(&time_raw);

        struct tm *time_info = localtime(&time_raw);

        char time_string[10];
        strftime(time_string, 10, "%H:%M:%S", time_info);

        printf("\033[33m▲\033[39m Warning [%s]: ", time_string);
        vprintf(fmt, args);
        printf("\n");

        va_end(args);
    #endif
}

void log_error(const char *fmt, ...) {
    #if LOG_LEVEL <= ERROR
        va_list args;
        va_start(args, fmt);
        
        time_t time_raw;
        time(&time_raw);

        struct tm *time_info = localtime(&time_raw);

        char time_string[10];
        strftime(time_string, 10, "%H:%M:%S", time_info);

        printf("\033[91mx\033[39m Error   [%s]: ", time_string);
        vprintf(fmt, args);
        printf("\n");

        va_end(args);
    #endif
}

void log_fatal(const char *fmt, ...) {
    #if LOG_LEVEL <= FATAL
        va_list args;
        va_start(args, fmt);
        
        time_t time_raw;
        time(&time_raw);

        struct tm *time_info = localtime(&time_raw);

        char time_string[10];
        strftime(time_string, 10, "%H:%M:%S", time_info);

        printf("☠️ Fatal  [%s]: ", time_string);
        vprintf(fmt, args);
        printf("\n");

        va_end(args);
    #endif
}
