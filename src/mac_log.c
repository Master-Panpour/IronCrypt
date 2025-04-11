#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "activity.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef __APPLE__

#include <TargetConditionals.h>
#if TARGET_OS_MAC
#include <mach-o/dyld.h>
#endif
#endif

int readMacLog(EmployeeActivity employees[]){
    FILE *fp = popen("log show --predicate 'eventMessage contains \"login\"' --info --last 1d", "r");
    if (!fp) {
        fprintf(stderr, "Error: Unable to read macOS log.\n");
        return -1;
    }

    char line[512], username[50];
    int hour, count = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "Account Name:")) {
            sscanf(line, " Account Name: %s", username);
            fgets(line, sizeof(line), fp);
            sscanf(line, " Logon Time: %*s %d:%*d:%*d", &hour);

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

    _pclose(fp);
    return count;
}