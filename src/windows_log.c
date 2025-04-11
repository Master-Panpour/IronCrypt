#include "../include/log_parser.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOWS_LOG_SOURCE "Security"
#define EVENT_ID_LOGON 4624
#define EVENT_ID_LOGOFF 4634

LogParseStatus parse_windows_log(LogEntry **entries, size_t *count) {
    HANDLE hEventLog = OpenEventLog(NULL, WINDOWS_LOG_SOURCE);
    if (!hEventLog) {
        return PARSE_ERROR_FILE;
    }

    // Initial allocation
    *entries = malloc(MAX_LOG_ENTRIES * sizeof(LogEntry));
    if (!*entries) {
        CloseEventLog(hEventLog);
        return PARSE_ERROR_MEMORY;
    }

    EVENTLOGRECORD *pevlr;
    BYTE buffer[4096];
    DWORD dwRead, dwNeeded;
    *count = 0;

    while (ReadEventLog(hEventLog, 
                       EVENTLOG_FORWARDS_READ|EVENTLOG_SEQUENTIAL_READ,
                       0, buffer, sizeof(buffer), &dwRead, &dwNeeded) &&
           *count < MAX_LOG_ENTRIES) {
        
        pevlr = (EVENTLOGRECORD *)buffer;
        while (dwRead > 0 && *count < MAX_LOG_ENTRIES) {
            if (pevlr->EventID == EVENT_ID_LOGON || pevlr->EventID == EVENT_ID_LOGOFF) {
                LogEntry entry = {0};
                SYSTEMTIME st;
                FileTimeToSystemTime((FILETIME*)&pevlr->TimeGenerated, &st);

                // Store parsed data
                entry.login_time.hour = st.wHour;
                entry.login_time.minute = st.wMinute;
                
                // Extract username from event data
                char *pUser = (char *)pevlr + pevlr->StringOffset;
                strncpy(entry.user_id, pUser, sizeof(entry.user_id) - 1);

                (*entries)[(*count)++] = entry;
            }

            dwRead -= pevlr->Length;
            pevlr = (EVENTLOGRECORD *)((LPBYTE)pevlr + pevlr->Length);
        }
    }

    CloseEventLog(hEventLog);
    return PARSE_SUCCESS;
}
