#ifndef LOG_PARSER_H
#define LOG_PARSER_H

#include "activity.h"

typedef enum {
    PARSE_SUCCESS,
    PARSE_ERROR_FILE,
    PARSE_ERROR_FORMAT,
    PARSE_ERROR_MEMORY
} LogParseStatus;

/**
 * @brief Parses Linux auth.log file
 * @param entries Pointer to array of log entries
 * @param count Pointer to count of parsed entries
 * @return Parse status code
 */
LogParseStatus parse_linux_log(LogEntry **entries, size_t *count);

/**
 * @brief Parses Windows event logs
 * @param entries Pointer to array of log entries
 * @param count Pointer to count of parsed entries
 * @return Parse status code
 */
LogParseStatus parse_windows_log(LogEntry **entries, size_t *count);

/**
 * @brief Parses macOS unified logs
 * @param entries Pointer to array of log entries
 * @param count Pointer to count of parsed entries
 * @return Parse status code
 */
LogParseStatus parse_mac_log(LogEntry **entries, size_t *count);

#endif // LOG_PARSER_H
