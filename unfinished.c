// #pragma warning(disable:4996) //only for vs code error handling

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>

// #ifdef _WIN32  // Windows-specific
//     #include <windows.h>
//     #define SYSTEM_OS "Windows"
// #else  // Linux-specific
//     #define AUTH_LOG "/var/log/auth.log"
//     #define SYSTEM_OS "Linux"
// #endif

// #define maxemp 100 //defined user strength
// #define maxlog 10 //max log per user

// // Employee activity structure
// typedef struct {
//     char name[50];
//     int logHrs[maxlog];
//     int logCount;
//     int fileAccessCount;
// } EmployeeActivity;

// // Function to detect suspicious activity
// void detectThreats(EmployeeActivity *emp) {
//     int abnormalLoginCount = 0;

//     // Check for unusual login times (1 AM - 4 AM)
//     for (int i = 0; i < emp->logCount; i++) {
//         if (emp->logHrs[i] >= 1 && emp->logHrs[i] <= 4) {
//             abnormalLoginCount++;
//         }
//     }

//     // Flag as suspicious if abnormal logins > 2 or file access > 7
//     if (abnormalLoginCount > 2 || emp->fileAccessCount > 7) {
//         printf("ALERT: Suspicious activity detected for %s!\n", emp->name);
//         printf("  - Unusual logins: %d times (between 1 AM - 4 AM detected)\n", abnormalLoginCount);
//         printf("  - Files accessed: %d\n", emp->fileAccessCount);
//     } else {
//         printf("No suspicious activity for %s.\n", emp->name);
//     }
// }

// // Function to read login activity from Linux logs
// #ifdef __linux__
// int readLinuxLog(EmployeeActivity employees[]) {
//     FILE *file = fopen(AUTH_LOG, "r");
//     if (!file) {
//         printf("Error: Unable to read %s. Run with sudo.\n", AUTH_LOG);
//         return 0;
//     }

//     char line[512], username[50];
//     int hour, count = 0;
    
//     while (fgets(line, sizeof(line), file)) {
//         if (strstr(line, "session opened for user")) {
//             sscanf(line, "%*s %*s %*s %d:%*d:%*d %*s %*s %s", &hour, username);

//             // Check if user exists
//             int i, found = 0;
//             for (i = 0; i < count; i++) {
//                 if (strcmp(employees[i].name, username) == 0) {
//                     found = 1;
//                     break;
//                 }
//             }

//             // Add new user if not found
//             if (!found && count < maxemp) {
//                 strcpy(employees[count].name, username);
//                 employees[count].logCount = 0;
//                 employees[count].fileAccessCount = 0;
//                 i = count++;
//             }

//             // Store login time
//             if (employees[i].logCount < maxlog) {
//                 employees[i].logHrs[employees[i].logCount++] = hour;
//             }
//         }
//     }
    
//     fclose(file);
//     return count;
// }
// #endif

// // Function to read Windows Event Log for login activity
// #ifdef _WIN32
// int readWindowsLog(EmployeeActivity employees[]) {
//     FILE *fp = _popen("wevtutil qe Security /c:20 /rd:true /f:text /q:\"*[System[Provider[@Name='Microsoft-Windows-Security-Auditing'] and (EventID=4624)]]\"", "r");
//     if (!fp) {
//         printf("Error: Unable to read Windows Event Log.\n");
//         return 0;
//     }

//     char line[512], username[50];
//     int hour, count = 0;
    
//     while (fgets(line, sizeof(line), fp)) {
//         if (strstr(line, "Account Name:")) {
//             sscanf(line, " Account Name: %s", username);
//             fgets(line, sizeof(line), fp); // Read next line with time
//             sscanf(line, " Logon Time: %*s %d:%*d:%*d", &hour);

//             // Check if user exists
//             int i, found = 0;
//             for (i = 0; i < count; i++) {
//                 if (strcmp(employees[i].name, username) == 0) {
//                     found = 1;
//                     break;
//                 }
//             }

//             // Add new user if not found
//             if (!found && count < maxemp) {
//                 strcpy(employees[count].name, username);
//                 employees[count].logCount = 0;
//                 employees[count].fileAccessCount = 0;
//                 i = count++;
//             }

//             // Store login time
//             if (employees[i].logCount < maxlog) {
//                 employees[i].logHrs[employees[i].logCount++] = hour;
//             }
//         }
//     }
    
//     _pclose(fp);
//     return count;
// }
// #endif

// // Main function
// int main() {
//     EmployeeActivity employees[maxemp];

//     printf("Running on %s OS\n", SYSTEM_OS);

//     int totalEmployees = 0;

// #ifdef __linux__
//     totalEmployees = readLinuxLog(employees);
// #elif _WIN32
//     totalEmployees = readWindowsLog(employees);
// #endif

//     // Analyze each employee
//     for (int i = 0; i < totalEmployees; i++) {
//         detectThreats(&employees[i]);
//         printf("\n");
//     }

//     return 0;
// }