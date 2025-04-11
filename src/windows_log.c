#include "../include/log_parser.h"
#include <windows.h>
#include <stdio.h>

LogParseStatus parse_windows_log(LogEntry **entries, size_t *count) {
    HANDLE hEventLog = OpenEventLog(NULL, LOG_PATH);
    if (!hEventLog) return LOG_PARSE_ACCESS_DENIED;
    
    EVENTLOGRECORD *pevlr;
    DWORD dwRead, dwNeeded;
    
    while(ReadEventLog(hEventLog, EVENTLOG_SEQUENTIAL_READ, 
        0, pevlr, 0, &dwRead, &dwNeeded)) {
        // Parse Windows event log structure
        LogEntry entry = {0};
        
        // Extract user from event data
        char *user = (char*)pevlr + sizeof(EVENTLOGRECORD);
        strncpy(entry.user, user, MAX_USER_LEN-1);
        
        encrypt_log_data(&entry);
        
        // Add to entries array
        *entries = realloc(*entries, (*count + 1) * sizeof(LogEntry));
        memcpy(&(*entries)[*count], &entry, sizeof(LogEntry));
        (*count)++;
    }
    
    CloseEventLog(hEventLog);
    return LOG_PARSE_SUCCESS;
}
