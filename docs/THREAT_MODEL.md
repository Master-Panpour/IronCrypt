# IronCrypt Threat Model

## Detection Capabilities

### 1. Behavioral Anomalies
- **Unusual Login Times** (1AM-4AM)
  - Implemented in: `detect.c`
  - Weight: 30 points (configurable in `security_constants.h`)

- **Excessive File Access**
  - Threshold: 15 files/session (configurable)
  - Weight: 25 points

- **Failed Login Attempts**
  - Threshold: 3 attempts (configurable)
  - Weight: 40 points

### 2. Machine Learning Detection
- **Algorithm**: Isolation Forest
- **Features**:
  - Login frequency
  - File access rate
- **Integration**: Via `ai_bridge.c`

## Risk Scoring
```c
// Basic detection scoring (detect.c)
if (entry->login_time.hour >= 1 && entry->login_time.hour <= 4) {
    entry->risk_score += RISK_WEIGHT_LATE_LOGIN; // 30 points
}

// ML integration (ai_bridge.c)
void ai_classify_threat(const char* user_id, int login_count, int file_access_count) {
    // Calls Python ML model
}
```

## Threat Scenarios Covered

| Scenario | Detection Method | Implementation File |
|----------|------------------|---------------------|
| Insider data exfiltration | File access monitoring | `detect.c` |
| Brute force attacks | Failed login tracking | `detect.c` |
| Unauthorized after-hours access | Time-based rules | `detect.c` |
| Behavioral anomalies | ML classification | `ai_bridge.c` + Python |

## Security Constants
Defined in `security_constants.h`:
```c
#define FILE_ACCESS_THRESHOLD 15
#define FAILED_LOGIN_THRESHOLD 3  
#define RISK_THRESHOLD 50
