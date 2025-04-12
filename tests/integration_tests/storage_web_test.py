import unittest
import sqlite3
import os
from fastapi.testclient import TestClient
from web_dashboard.backend.server import app
from include.storage_updated import LogEntry

class TestStorageAndWebIntegration(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # Setup test database
        cls.test_db = "test_ironcrypt.db"
        if os.path.exists(cls.test_db):
            os.remove(cls.test_db)
        
        conn = sqlite3.connect(cls.test_db)
        with open('storage/db_schema.sql') as f:
            conn.executescript(f.read())
        conn.close()

        # Setup test client
        os.environ['TESTING'] = 'True'
        cls.client = TestClient(app)

    def test_log_storage_and_retrieval(self):
        # Test database operations
        test_log = {
            "timestamp": "2023-01-01 12:00:00",
            "user_id": "test_user",
            "source_ip": "192.168.1.1",
            "event_type": "login",
            "details": "Successful login",
            "risk_score": 10
        }

        # Insert test data
        conn = sqlite3.connect(self.test_db)
        cursor = conn.cursor()
        cursor.execute(
            "INSERT INTO log_entries (timestamp, user_id, source_ip, event_type, details, risk_score) "
            "VALUES (?, ?, ?, ?, ?, ?)",
            (test_log['timestamp'], test_log['user_id'], test_log['source_ip'], 
             test_log['event_type'], test_log['details'], test_log['risk_score'])
        )
        conn.commit()
        conn.close()

        # Verify API endpoint
        response = self.client.get("/logs/recent?limit=1")
        self.assertEqual(response.status_code, 200)
        logs = response.json()
        self.assertEqual(len(logs), 1)
        self.assertEqual(logs[0]['user_id'], test_log['user_id'])

    def test_alert_workflow(self):
        # First create a log entry
        conn = sqlite3.connect(self.test_db)
        cursor = conn.cursor()
        cursor.execute(
            "INSERT INTO log_entries (timestamp, user_id, source_ip, event_type, details, risk_score) "
            "VALUES (datetime('now'), 'alert_user', '10.0.0.1', 'failed_login', '3 failed attempts', 45)"
        )
        log_id = cursor.lastrowid

        # Create an alert
        cursor.execute(
            "INSERT INTO threat_alerts (log_entry_id, severity, description) "
            "VALUES (?, ?, ?)",
            (log_id, 3, "Multiple failed login attempts")
        )
        conn.commit()
        conn.close()

        # Verify API endpoint
        response = self.client.get("/alerts/active")
        self.assertEqual(response.status_code, 200)
        alerts = response.json()
        self.assertGreater(len(alerts), 0)
        self.assertEqual(alerts[0]['description'], "Multiple failed login attempts")

if __name__ == '__main__':
    unittest.main()
