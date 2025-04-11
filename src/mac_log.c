#include "../include/log_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PARSER_BINARY "/usr/local/bin/unifiedlog_parser"
#define OUTPUT_FILE "/tmp/macos_logs.csv"

LogParseStatus parse_mac_unified_log(LogEntry **entries, size_t *count) {
    // Step 1: Run unifiedlog_parser to extract logs into CSV format
    char command[512];
    snprintf(command, sizeof(command), "%s --output %s", PARSER_BINARY, OUTPUT_FILE);
    
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to execute unifiedlog_parser\n");
        return LOG_PARSE_FAILURE;
    }

    // Step 2: Open the output CSV file for reading
    FILE *file = fopen(OUTPUT_FILE, "r");
    if (!file) {
        fprintf(stderr, "Error: Failed to open parsed log file\n");
        return LOG_PARSE_ACCESS_DENIED;
    }

    // Step 3: Parse CSV file line by line
    char line[1024];
    *entries = NULL;
    *count = 0;

    while (fgets(line, sizeof(line), file)) {
        LogEntry entry = {0};

        // Example CSV format: timestamp,user,event_type,message
        sscanf(line, "%24[^,],%31[^,],%d,%255[^\n]",
               entry.timestamp,
               entry.user,
               &entry.activity_type,
               entry.resource);

        sanitize_log_entry(&entry);

        // Add to entries array
        *entries = realloc(*entries, (*count + 1) * sizeof(LogEntry));
        memcpy(&(*entries)[*count], &entry, sizeof(LogEntry));
        (*count)++;
    }

    fclose(file);
    return LOG_PARSE_SUCCESS;
}
