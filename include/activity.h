#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <time.h>

typedef struct {
    int hour;
    int minute;
} LoginTime;

typedef struct {
    char user_id[32];
    LoginTime login_time;
    int login_count;
    int file_access_count;
    int failed_attempts;
    int risk_score;
} LogEntry;

// Maximum values for arrays
#define MAX_USERS 1000
#define MAX_LOG_ENTRIES 10000

#endif // ACTIVITY_H
