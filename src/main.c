#include <stdio.h>
#include <stdlib.h>
#include "log_parser.h"
#include "threat_detection.h"
#include "ai_bridge.h"

int main() {
    // Initialize log parsers
    LogEntry *entries = NULL;
    size_t entry_count = 0;
    
    // Parse system logs based on OS
    #ifdef __linux__
        parse_linux_log(&entries, &entry_count);
    #elif _WIN32
        parse_windows_log(&entries, &entry_count);
    #elif __APPLE__
        parse_mac_log(&entries, &entry_count);
    #endif

    // Process each log entry
    for (size_t i = 0; i < entry_count; i++) {
        // Basic rule-based detection
        detect_basic_threats(&entries[i]);
        
        // Advanced ML-based detection
        if (entries[i].risk_score > 0) {
            ai_classify_threat(entries[i].user_id, 
                             entries[i].login_count, 
                             entries[i].file_access_count);
        }
    }

    // Clean up
    free(entries);
    return 0;
}
