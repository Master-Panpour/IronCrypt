-- IronCrypt Database Schema
CREATE TABLE IF NOT EXISTS log_entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME NOT NULL,
    user_id TEXT NOT NULL,
    source_ip TEXT NOT NULL,
    event_type TEXT NOT NULL,
    details TEXT,
    risk_score INTEGER DEFAULT 0,
    processed BOOLEAN DEFAULT FALSE
);

CREATE TABLE IF NOT EXISTS threat_alerts (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    log_entry_id INTEGER REFERENCES log_entries(id),
    alert_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    severity INTEGER NOT NULL,
    description TEXT NOT NULL,
    resolved BOOLEAN DEFAULT FALSE
);

CREATE INDEX IF NOT EXISTS idx_log_entries_timestamp ON log_entries(timestamp);
CREATE INDEX IF NOT EXISTS idx_log_entries_user_id ON log_entries(user_id);
CREATE INDEX IF NOT EXISTS idx_threat_alerts_severity ON threat_alerts(severity);
