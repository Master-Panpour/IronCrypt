#include "../include/log_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MACOS_LOG_CMD "log show --predicate 'eventMessage contains \"session\"' --last 24h"
#define MAX_LINE_LENGTH 1024

LogParseStatus parse_mac_log(LogEntry **entries, size_t *count) {
    FILE *log_stream = popen(MACOS_LOG_CMD, "r");
    if (!log_stream) {
        return PARSE_ERROR_FILE;
    }

    // Initial allocation
    *entries = malloc(MAX_LOG_ENTRIES * sizeof(LogEntry));
    if (!*entries) {
        pclose(log_stream);
        return PARSE_ERROR_MEMORY;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    while (fgets(line, sizeof(line), log_stream) && *count < MAX_LOG_ENTRIES) {
        if (strstr(line, "session opened") || strstr(line, "session closed")) {
            LogEntry entry = {0};
            char timestamp[32], month[4], time_str[9];
            int day, hour, minute;

            // Parse timestamp (format: "YYYY-MM-DD HH:MM:SS")
            if (sscanf(line, "%19s", timestamp) != 1) {
                continue;
            }

            // Parse time components
            if (sscanf(timestamp + 11, "%d:%d", &hour, &minute) != 2) {
                continue;
            }

            // Store parsed data
            entry.login_time.hour = hour;
            entry.login_time.minute = minute;
            
            // Extract username
            char *user_start = strstr(line, "for user '");
            if (user_start) {
                user_start += 10; // Skip "for user '"
                char *user_end = strchr(user_start, '\'');
                if (user_end) {
                    size_t user_len = user_end - user_start;
                    strncpy(entry.user_id, user_start, user_len);
                    entry.user_id[user_len] = '\0';
                }
            }

            (*entries)[(*count)++] = entry;
        }
    }

    pclose(log_stream);
    return PARSE_SUCCESS;
}
