#ifndef ACTIVITY_H
#define ACTIVITY_H

#define maxemp 100
#define maxlog 10

typedef struct {
    char name[50];
    int logHrs[maxlog];
    int logCount;
    int fileAccessCount;
} EmployeeActivity;

void detectThreats(EmployeeActivity *emp);
int readLinuxLog(EmployeeActivity employees[]);
int readWindowsLog(EmployeeActivity employees[]);
int readMacLog(EmployeeActivity employees[]);

#endif