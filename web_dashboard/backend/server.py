from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
import sqlite3
from pydantic import BaseModel
from typing import List
import uvicorn

app = FastAPI()

# Configure CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Database connection
def get_db():
    try:
        conn = sqlite3.connect('storage/ironcrypt.db')
        conn.row_factory = sqlite3.Row
        return conn
    except sqlite3.Error as e:
        print(f"Database connection error: {e}")
        raise

# Data models
class LogEntry(BaseModel):
    id: int
    timestamp: str
    message: str
    severity: str

class Alert(BaseModel):
    id: int
    timestamp: str
    message: str
    status: str

# API endpoints
@app.get("/logs/recent")
def get_recent_logs(limit: int = 100):
    try:
        conn = get_db()
        logs = conn.execute('SELECT * FROM logs ORDER BY timestamp DESC LIMIT ?', (limit,)).fetchall()
        conn.close()
        return {"status": "success", "logs": logs}
    except Exception as e:
        return {"status": "error", "message": str(e)}

@app.get("/alerts/active")
def get_active_alerts():
    try:
        conn = get_db()
        alerts = conn.execute('SELECT * FROM alerts WHERE status = "active"').fetchall()
        conn.close()
        return {"status": "success", "alerts": alerts}
    except Exception as e:
        return {"status": "error", "message": str(e)}

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
