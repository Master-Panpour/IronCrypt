#ifndef SECURITY_CONSTANTS_H
#define SECURITY_CONSTANTS_H

// Threshold values
#define FILE_ACCESS_THRESHOLD 15      // Max normal file accesses per session
#define FAILED_LOGIN_THRESHOLD 3      // Max allowed failed login attempts
#define RISK_THRESHOLD 50             // Minimum score to flag as potential threat

// Risk weights
#define RISK_WEIGHT_LATE_LOGIN 30     // Weight for late-night logins (1AM-4AM)
#define RISK_WEIGHT_FILE_ACCESS 25    // Weight for excessive file access
#define RISK_WEIGHT_FAILED_LOGIN 40   // Weight for failed login attempts

// Time constants (in seconds)
#define SESSION_TIMEOUT 3600          // 1 hour session timeout
#define LOGIN_WINDOW 300              // 5 minute window for failed login tracking

#endif // SECURITY_CONSTANTS_H
