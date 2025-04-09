#ifdef __linux__

#include <stdio.h>
#include <string.h>
#include "activity.h"

#define AUTH_LOG "/var/log/auth.log"

int readLinuxLog(EmployeeActivity employees[]) {
    FILE *file = fopen(AUTH_LOG, "r");
    if (!file) {
        printf("Error: Unable to read %s. Run with sudo.\n", AUTH_LOG);
        return 0;
    }

    char line[512], username[50];
    int hour, count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "session opened for user")) {
            sscanf(line, "%*s %*s %*s %d:%*d:%*d %*s %*s %s", &hour, username);

            int i, found = 0;
            for (i = 0; i < count; i++) {
                if (strcmp(employees[i].name, username) == 0) {
                    found = 1;
                    break;
                }
            }

            if (!found && count < maxemp) {
                strcpy(employees[count].name, username);
                employees[count].logCount = 0;
                employees[count].fileAccessCount = 0;
                i = count++;
            }

            if (employees[i].logCount < maxlog) {
                employees[i].logHrs[employees[i].logCount++] = hour;
            }
        }
    }

    fclose(file);
    return count;
}

#endif