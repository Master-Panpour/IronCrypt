#ifndef LOG_PARSER_H
#define LOG_PARSER_H

#include <stddef.h>
#include <stdbool.h>
#include "security_constants.h"

// Security buffer sizes from security_constants.h
#define MAX_USER_LEN 32
#define ISO8601_LEN 25
#define IP_LEN 46
#define RES_PATH_LEN 256

typedef enum {
    LOG_PARSE_SUCCESS,
    LOG_PARSE_FAILURE,
    LOG_PARSE_ACCESS_DENIED,
    LOG_PARSE_INVALID_FORMAT
} LogParseStatus;

typedef struct {
    char user[MAX_USER_LEN];
    char timestamp[ISO8601_LEN];
    char source_ip[IP_LEN];
    int activity_type;
    char resource[RES_PATH_LEN];
    bool encrypted;
} LogEntry;

// OS-specific function prototype
typedef LogParseStatus (*LogParser)(LogEntry**, size_t*);

// Common functions
LogParseStatus sanitize_log_entry(LogEntry *entry);
bool validate_log_entry(const LogEntry *entry);
void encrypt_log_data(LogEntry *entry);

// OS detection
#ifdef _WIN32
    #define LOG_PATH "Security"
    LogParseStatus parse_windows_log(LogEntry **entries, size_t *count);
#elif __linux__
    #define LOG_PATH "/var/log/auth.log"
    LogParseStatus parse_linux_log(LogEntry **entries, size_t *count);
#elif __APPLE__
    #define LOG_PATH "/var/log/system.log"
    LogParseStatus parse_macos_log(LogEntry **entries, size_t *count);
#else
    #error "Unsupported operating system"
#endif

#endif
