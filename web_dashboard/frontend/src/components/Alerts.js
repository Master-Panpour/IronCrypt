import React, { useState, useEffect } from 'react';
import { fetchActiveAlerts, resolveAlert } from '../services/api';

const Alerts = () => {
    const [alerts, setAlerts] = useState([]);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const loadAlerts = async () => {
            try {
                const alertsData = await fetchActiveAlerts();
                setAlerts(alertsData);
            } catch (error) {
                console.error('Error loading alerts:', error);
            } finally {
                setLoading(false);
            }
        };
        loadAlerts();
    }, []);

    const handleResolve = async (alertId) => {
        const success = await resolveAlert(alertId);
        if (success) {
            setAlerts(alerts.filter(alert => alert.id !== alertId));
        }
    };

    if (loading) return <div>Loading alerts...</div>;

    return (
        <div className="alerts">
            <h1>Threat Alerts</h1>
            <table>
                <thead>
                    <tr>
                        <th>Time</th>
                        <th>Description</th>
                        <th>Severity</th>
                        <th>Actions</th>
                    </tr>
                </thead>
                <tbody>
                    {alerts.map(alert => (
                        <tr key={alert.id}>
                            <td>{new Date(alert.alert_time).toLocaleString()}</td>
                            <td>{alert.description}</td>
                            <td>{alert.severity}</td>
                            <td>
                                <button onClick={() => handleResolve(alert.id)}>
                                    Mark as Resolved
                                </button>
                            </td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </div>
    );
};

export default Alerts;
