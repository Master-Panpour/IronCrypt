#ifndef LOG_PARSER_H
#define LOG_PARSER_H

#include "threat_detection.h" 

#define maxemp 100
#define maxlog 10

#ifdef _WIN32
    #define SYSTEM_OS "Windows"
    int readLogs(EmployeeActivity employees[]);

#elif __APPLE__
    #define SYSTEM_OS "macOS"
    int readLogs(EmployeeActivity employees[]); 

#elif __linux__
    #define SYSTEM_OS "Linux"
    int readLogs(EmployeeActivity employees[]);  

#else
    #define SYSTEM_OS "Unknown OS"
    #error "Unsupported operating system"
#endif

#endif
