# IronCrypt System Architecture

## Core Components

### 1. Log Parsing Layer
- **Platform-specific Parsers**:
  - `linux_log.c`: Parses Linux auth.log for session events
  - `windows_log.c`: Reads Windows Security event logs
  - `mac_log.c`: Processes macOS unified logs
- **Common Interface**: Defined in `log_parser.h`

### 2. Threat Detection Engine
- **Rule-based Detection** (`detect.c`):
  - Late-night login detection (1AM-4AM)
  - Excessive file access monitoring
  - Failed login attempt tracking
- **Machine Learning Integration**:
  - Python ML model (Isolation Forest)
  - C-Python bridge (`ai_bridge.c`)

### 3. Data Structures
- `activity.h`: Defines core data structures:
  - `LogEntry`: Tracks user sessions
  - `LoginTime`: Timestamp handling
- `security_constants.h`: Configuration thresholds

## Implementation Details

### Cross-platform Support
```c
#ifdef __linux__
    parse_linux_log();
#elif _WIN32
    parse_windows_log(); 
#elif __APPLE__
    parse_mac_log();
#endif
```

### Hybrid Detection Approach
1. Basic rule-based checks in C
2. Suspicious events forwarded to Python ML model
3. Combined risk scoring

## Build System
- Requires:
  - Python 3.8+ for ML components
  - C11 compatible compiler
  - Platform-specific libraries:
    - Windows: Advapi32 for event logs
    - macOS: Unified logging system

## Deployment
```bash
# Build C components
make all

# Install Python dependencies
pip install -r requirements.txt
