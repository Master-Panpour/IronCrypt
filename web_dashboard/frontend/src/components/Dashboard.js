import React, { useState, useEffect } from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend } from 'recharts';
import { fetchRecentLogs, fetchActiveAlerts } from '../services/api';

const Dashboard = () => {
    const [logs, setLogs] = useState([]);
    const [alerts, setAlerts] = useState([]);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const loadData = async () => {
            try {
                const [logsData, alertsData] = await Promise.all([
                    fetchRecentLogs(),
                    fetchActiveAlerts()
                ]);
                setLogs(logsData);
                setAlerts(alertsData);
            } catch (error) {
                console.error('Error loading data:', error);
            } finally {
                setLoading(false);
            }
        };
        loadData();
    }, []);

    const processChartData = () => {
        // Process logs for chart display
        return logs.map(log => ({
            time: new Date(log.timestamp).toLocaleTimeString(),
            risk: log.risk_score
        }));
    };

    if (loading) return <div>Loading dashboard...</div>;

    return (
        <div className="dashboard">
            <h1>IronCrypt Dashboard</h1>
            
            <div className="chart-container">
                <h2>Risk Score Timeline</h2>
                <LineChart width={800} height={300} data={processChartData()}>
                    <CartesianGrid strokeDasharray="3 3" />
                    <XAxis dataKey="time" />
                    <YAxis />
                    <Tooltip />
                    <Legend />
                    <Line type="monotone" dataKey="risk" stroke="#8884d8" />
                </LineChart>
            </div>

            <div className="alerts-summary">
                <h2>Active Alerts: {alerts.length}</h2>
                <ul>
                    {alerts.slice(0, 3).map(alert => (
                        <li key={alert.id}>
                            {alert.description} (Severity: {alert.severity})
                        </li>
                    ))}
                </ul>
            </div>
        </div>
    );
};

export default Dashboard;
