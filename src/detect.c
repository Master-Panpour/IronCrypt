#include <stdio.h>
#include <time.h>
#include "activity.h"
#include "threat_detection.h"
#include "security_constants.h"

void detect_basic_threats(LogEntry *entry) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    int current_hour = tm_now->tm_hour;
    
    // Initialize risk score
    entry->risk_score = 0;

    // Check for unusual login times (1AM-4AM)
    if (entry->login_time.hour >= 1 && entry->login_time.hour <= 4) {
        entry->risk_score += RISK_WEIGHT_LATE_LOGIN;
    }

    // Check for high file access rate
    if (entry->file_access_count > FILE_ACCESS_THRESHOLD) {
        entry->risk_score += RISK_WEIGHT_FILE_ACCESS;
    }

    // Check for multiple failed logins
    if (entry->failed_attempts > FAILED_LOGIN_THRESHOLD) {
        entry->risk_score += RISK_WEIGHT_FAILED_LOGIN;
    }

    // Log detection results
    if (entry->risk_score > RISK_THRESHOLD) {
        printf("[THREAT] Potential threat detected for user %s (score: %d)\n",
               entry->user_id, entry->risk_score);
    }
}
