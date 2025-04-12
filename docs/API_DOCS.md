# IronCrypt API Documentation

## Core Functions

### 1. Log Parsing API
```c
// Header: log_parser.h
LogParseStatus parse_linux_log(LogEntry **entries, size_t *count);
LogParseStatus parse_windows_log(LogEntry **entries, size_t *count); 
LogParseStatus parse_mac_log(LogEntry **entries, size_t *count);
```

### 2. Threat Detection API
```c
// Header: threat_detection.h
void detect_basic_threats(LogEntry *entry);

// Header: ai_bridge.h  
void ai_classify_threat(const char* user_id, int login_count, int file_access_count);
```

## Data Structures

### LogEntry
```c
typedef struct {
    char user_id[32];
    LoginTime login_time;
    int login_count;
    int file_access_count; 
    int failed_attempts;
    int risk_score;
} LogEntry;
```

## Python API (ML Integration)

### classify.py
```python
class AnomalyClassifier:
    def __init__(self, model_path):
        """Load trained model"""
    
    def classify(self, features):
        """Classify single set of features"""
    
    def batch_classify(self, data):
        """Classify pandas DataFrame"""
```

## Example Usage

### C Integration
```c
#include "log_parser.h"
#include "threat_detection.h"

int main() {
    LogEntry *entries;
    size_t count;
    
    parse_linux_log(&entries, &count);
    
    for (int i = 0; i < count; i++) {
        detect_basic_threats(&entries[i]);
    }
}
```

### Python Integration
```python
from ai.classify import AnomalyClassifier

classifier = AnomalyClassifier("threat_model.pkl")
result = classifier.classify([12, 5])  # login_freq=12, file_access=5
