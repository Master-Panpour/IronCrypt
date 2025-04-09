#ifdef _WIN32

#include <stdio.h>
#include <string.h>
#include "activity.h"

int readWindowsLog(EmployeeActivity employees[]) {
    FILE *fp = _popen("wevtutil qe Security /c:20 /rd:true /f:text /q:\"*[System[Provider[@Name='Microsoft-Windows-Security-Auditing'] and (EventID=4624)]]\"", "r");
    if (!fp) {
        printf("Error: Unable to read Windows Event Log.\n");
        return 0;
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

#endif