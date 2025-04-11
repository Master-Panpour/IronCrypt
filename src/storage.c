#include "storage_updated.h"
#include "encryption_utils.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Encryption key (should be securely stored in production)
static const unsigned char encryption_key[32] = "thisisaverysecureencryptionkey123";

static sqlite3* db = NULL;

static int store_log_entry(const LogEntry* entry) {
    char* sql = "INSERT INTO log_entries (timestamp, user_id, source_ip, event_type, details, risk_score) "
                "VALUES (datetime(?, 'unixepoch'), ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    // Encrypt sensitive fields
    unsigned char encrypted_user_id[256];
    unsigned char encrypted_source_ip[256];
    unsigned char encrypted_details[1024];
    
    encrypt_data((unsigned char*)entry->user_id, encrypted_user_id, encryption_key);
    encrypt_data((unsigned char*)entry->source_ip, encrypted_source_ip, encryption_key);
    encrypt_data((unsigned char*)entry->details, encrypted_details, encryption_key);

    sqlite3_bind_text(stmt, 2, (char*)encrypted_user_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, (char*)encrypted_source_ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, entry->event_type, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, (char*)encrypted_details, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, entry->risk_score);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (result == SQLITE_DONE) ? 0 : -1;
}

static int get_recent_entries(LogEntry** entries, int max_entries) {
    char* sql = "SELECT * FROM log_entries ORDER BY timestamp DESC LIMIT ?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    sqlite3_bind_int(stmt, 1, max_entries);
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_entries) {
        LogEntry* entry = malloc(sizeof(LogEntry));
        
        // Decrypt sensitive fields
        unsigned char decrypted_user_id[256];
        unsigned char decrypted_source_ip[256];
        unsigned char decrypted_details[1024];
        
        decrypt_data((unsigned char*)sqlite3_column_text(stmt, 2), decrypted_user_id, encryption_key);
        decrypt_data((unsigned char*)sqlite3_column_text(stmt, 3), decrypted_source_ip, encryption_key);
        decrypt_data((unsigned char*)sqlite3_column_text(stmt, 5), decrypted_details, encryption_key);
        
        entry->user_id = strdup((char*)decrypted_user_id);
        entry->source_ip = strdup((char*)decrypted_source_ip);
        entry->event_type = strdup((char*)sqlite3_column_text(stmt, 4));
        entry->details = strdup((char*)decrypted_details);
        entry->risk_score = sqlite3_column_int(stmt, 6);
        
        entries[count++] = entry;
    }
    
    sqlite3_finalize(stmt);
    return count;
}

static int add_threat_alert(const char* description, int severity, int log_entry_id) {
    char* sql = "INSERT INTO threat_alerts (log_entry_id, severity, description) "
                "VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    sqlite3_bind_int(stmt, 1, log_entry_id);
    sqlite3_bind_int(stmt, 2, severity);
    sqlite3_bind_text(stmt, 3, description, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (result == SQLITE_DONE) ? 0 : -1;
}

StorageInterface* init_storage(const char* db_path) {
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        return NULL;
    }

    // Create tables if they don't exist
    FILE* schema_file = fopen("storage/db_schema.sql", "r");
    if (schema_file) {
        fseek(schema_file, 0, SEEK_END);
        long size = ftell(schema_file);
        rewind(schema_file);
        
        char* sql = malloc(size + 1);
        fread(sql, 1, size, schema_file);
        sql[size] = '\0';
        
        sqlite3_exec(db, sql, NULL, NULL, NULL);
        free(sql);
        fclose(schema_file);
    }

    StorageInterface* interface = malloc(sizeof(StorageInterface));
    interface->store_log_entry = store_log_entry;
    interface->get_recent_entries = get_recent_entries;
    interface->add_threat_alert = add_threat_alert;

    return interface;
}

void cleanup_storage() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}
