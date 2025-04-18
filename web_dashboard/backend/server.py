from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
import sqlite3
import logging
import logging.config
import os
from pydantic import BaseModel
from typing import List, Optional
import uvicorn

app = FastAPI()

# Configure CORS - more flexible for development
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Database connection with absolute path
def get_db():
    try:
        db_path = os.path.abspath(os.path.join(os.path.dirname(__file__), 'storage/ironcrypt.db'))
        logging.info(f"Attempting to connect to database at: {db_path}")
        conn = sqlite3.connect(db_path)
        conn.row_factory = sqlite3.Row
        return conn
    except sqlite3.Error as e:
        logging.error(f"Database connection error: {e}")
        raise HTTPException(status_code=500, detail="Database connection failed")

# Data models
class LogEntry(BaseModel):
    id: int
    timestamp: str
    user_id: str
    source_ip: str
    event_type: str
    details: Optional[str] = None
    risk_score: Optional[int] = None

class Alert(BaseModel):
    id: int
    log_entry_id: int
    alert_time: str
    severity: int
    description: str
    resolved: bool

# API endpoints
@app.get("/logs/recent")
def get_recent_logs(limit: int = 100):
    try:
        conn = get_db()
        logs = conn.execute('''
            SELECT id, timestamp, user_id, source_ip, event_type, details, risk_score 
            FROM log_entries 
            ORDER BY timestamp DESC 
            LIMIT ?
        ''', (limit,)).fetchall()
        conn.close()
        return {"status": "success", "data": [dict(log) for log in logs]}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/alerts/active")
def get_active_alerts():
    try:
        conn = get_db()
        alerts = conn.execute('''
            SELECT id, log_entry_id, alert_time, severity, description, resolved
            FROM threat_alerts
            WHERE resolved = FALSE
            ORDER BY alert_time DESC
        ''').fetchall()
        conn.close()
        return {"status": "success", "data": [dict(alert) for alert in alerts]}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/alerts/{alert_id}/resolve")
def resolve_alert(alert_id: int):
    try:
        conn = get_db()
        conn.execute('UPDATE threat_alerts SET resolved = TRUE WHERE id = ?', (alert_id,))
        conn.commit()
        conn.close()
        return {"status": "success", "message": f"Alert {alert_id} resolved"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/api/v1/health")
def health_check():
    return {"status": "healthy"}

@app.get("/api/v1/metrics")
def get_metrics():
    # Mock metrics data
    return {
        "threats": 5,
        "highRiskUsers": ["jdoe", "asmith"]
    }

@app.get("/api/v1/classify")
def classify_activities(page: int = 1):
    # Mock activity data
    activities = [
        {"username": "jdoe", "action": "Anomalous Login", "timestamp": "2025-04-12 10:23", "risk": "high"},
        {"username": "asmith", "action": "File Exfiltration", "timestamp": "2025-04-12 09:55", "risk": "medium"}
    ]
    return activities[(page-1)*10:page*10]

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
