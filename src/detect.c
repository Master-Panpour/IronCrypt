#include <stdio.h>
#include "activity.h"

void detectThreats(EmployeeActivity *emp) {
    int abnormalLoginCount = 0;

    for (int i = 0; i < emp->logCount; i++) {
        if (emp->logHrs[i] >= 1 && emp->logHrs[i] <= 4) {
            abnormalLoginCount++;
        }
    }

    if (abnormalLoginCount > 2 || emp->fileAccessCount > 7) {
        printf("ALERT: Suspicious activity detected for %s!\n", emp->name);
        printf("  - Unusual logins: %d times (between 1 AM - 4 AM detected)\n", abnormalLoginCount);
        printf("  - Files accessed: %d\n", emp->fileAccessCount);
    } else {
        printf("No suspicious activity for %s.\n", emp->name);
    }
}