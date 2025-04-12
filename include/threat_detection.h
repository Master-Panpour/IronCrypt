#ifndef THREAT_DETECTION_H
#define THREAT_DETECTION_H

#include "activity.h"

/**
 * @brief Detects potential threats based on user activity logs.
 * @param entry Pointer to the log entry to analyze.
 */
void detect_basic_threats(LogEntry *entry);

#endif // THREAT_DETECTION_H
