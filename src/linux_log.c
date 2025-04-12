#include "../include/log_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define LINUX_LOG_PATH "/var/log/auth.log"
#define MAX_LINE_LENGTH 1024

LogParseStatus parse_linux_log(LogEntry **entries, size_t *count) {
    FILE *file = fopen(LINUX_LOG_PATH, "r");
    if (!file) {
        return PARSE_ERROR_FILE;
    }

    // Initial allocation
    *entries = malloc(MAX_LOG_ENTRIES * sizeof(LogEntry));
    if (!*entries) {
        fclose(file);
        return PARSE_ERROR_MEMORY;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    while (fgets(line, sizeof(line), file) && *count < MAX_LOG_ENTRIES) {
        if (strstr(line, "session opened") || strstr(line, "session closed")) {
            LogEntry entry = {0};
            char month[4], time_str[9];
            int day, hour, minute;

            // Parse timestamp (format: "MMM DD HH:MM:SS")
            if (sscanf(line, "%3s %d %8s", month, &day, time_str) != 3) {
                continue;
            }

            // Parse time components
            if (sscanf(time_str, "%d:%d", &hour, &minute) != 2) {
                continue;
            }

            // Store parsed data
            entry.login_time.hour = hour;
            entry.login_time.minute = minute;
            strncpy(entry.user_id, strstr(line, "for user") + 9, sizeof(entry.user_id) - 1);
            char *newline = strchr(entry.user_id, '\n');
            if (newline) *newline = '\0';

            (*entries)[(*count)++] = entry;
        }
    }

    fclose(file);
    return PARSE_SUCCESS;
}

