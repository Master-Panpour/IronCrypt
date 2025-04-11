#ifndef STORAGE_UPDATED_H
#define STORAGE_UPDATED_H

#include "activity.h"
#include "security_constants.h"

/**
 * Log entry structure
 * Note: user_id, source_ip, and details are encrypted in storage
 */
typedef struct {
    char* user_id;          // Encrypted in storage
    char* source_ip;        // Encrypted in storage
    char* event_type;       // Stored as plaintext
    char* details;          // Encrypted in storage
    int risk_score;         // Stored as plaintext
} LogEntry;

/**
 * Storage interface with encryption support
 */
typedef struct {
    /**
     * Stores a log entry with sensitive fields encrypted
     * @param entry The log entry to store
     * @return 0 on success, -1 on failure
     */
    int (*store_log_entry)(const LogEntry* entry);
    
    /**
     * Retrieves recent log entries with decrypted sensitive fields
     * @param entries Output array for entries (caller must free)
     * @param max_entries Maximum number of entries to retrieve
     * @return Number of entries retrieved, or -1 on error
     */
    int (*get_recent_entries)(LogEntry** entries, int max_entries);
    
    /**
     * Adds a threat alert (stored as plaintext)
     * @param description Alert description
     * @param severity Severity level (1-10)
     * @param log_entry_id Associated log entry ID
     * @return 0 on success, -1 on failure
     */
    int (*add_threat_alert)(const char* description, int severity, int log_entry_id);
} StorageInterface;

/**
 * Initializes the storage system with encryption support
 * @param db_path Path to database file
 * @return Initialized storage interface
 */
StorageInterface* init_storage(const char* db_path);

/**
 * Cleans up storage resources
 */
void cleanup_storage();

#endif // STORAGE_UPDATED_H
