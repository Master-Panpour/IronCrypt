#include <stdio.h>
#include "activity.h"

#ifdef _WIN32
    #define SYSTEM_OS "Windows"
#else
    #define SYSTEM_OS "Linux"
#endif

int main() {
    EmployeeActivity employees[maxemp];
    int totalEmployees = 0;

    printf("Running on %s OS\n", SYSTEM_OS);

#ifdef __linux__
    totalEmployees = readLinuxLog(employees);
#elif _WIN32
    totalEmployees = readWindowsLog(employees);
#endif

    for (int i = 0; i < totalEmployees; i++) {
        detectThreats(&employees[i]);
        printf("\n");
    }

    return 0;
}