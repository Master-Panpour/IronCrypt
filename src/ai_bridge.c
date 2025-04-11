#include <stdlib.h>
#include <stdio.h>
#include "ai_bridge.h"

void ai_classify_threat(const char* user_id, int login_count, int file_access_count) {
    char command[256];
    snprintf(command, sizeof(command), 
             "python3 ai/classify.py %s %d %d",
             user_id, login_count, file_access_count);
    
    int ret = system(command);
    if (ret != 0) {
        fprintf(stderr, "AI classification failed for user %s\n", user_id);
    }
}
