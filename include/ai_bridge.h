#ifndef AI_BRIDGE_H
#define AI_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calls Python ML model to classify potential threat
 * @param user_id Unique user identifier
 * @param login_count Number of logins in time period
 * @param file_access_count Number of files accessed
 */
void ai_classify_threat(const char* user_id, int login_count, int file_access_count);

#ifdef __cplusplus
}
#endif

#endif // AI_BRIDGE_H
