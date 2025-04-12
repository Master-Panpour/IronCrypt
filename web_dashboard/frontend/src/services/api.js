const API_BASE_URL = 'http://localhost:8000';

const handleResponse = async (response) => {
    if (!response.ok) {
        const errorData = await response.json().catch(() => ({}));
        throw new Error(errorData.detail || 'Request failed');
    }
    return response.json();
};

// Employee Data Endpoints
export const fetchEmployeeData = async () => {
    try {
        const response = await fetch(`${API_BASE_URL}/logs/recent?limit=100`, {
            headers: {
                'Content-Type': 'application/json',
            },
        });
        const data = await handleResponse(response);
        return data.data.map(log => ({
            name: log.user_id,
            logHrs: [8,7,8,6,9], // Mock data - replace with actual from logs
            logCount: data.data.filter(l => l.user_id === log.user_id).length,
            fileAccessCount: data.data.filter(l => l.user_id === log.user_id && l.event_type === 'file_access').length
        }));
    } catch (error) {
        console.error('Error fetching employee data:', error);
        throw error;
    }
};

// Threat Detection Endpoints
export const fetchThreatData = async () => {
    try {
        const response = await fetch(`${API_BASE_URL}/alerts/active`, {
            headers: {
                'Content-Type': 'application/json',
            },
        });
        const data = await handleResponse(response);
        return data.data.map(alert => ({
            employee: alert.description.split(' ')[0], // Extract name from alert
            type: alert.description,
            severity: ['Low', 'Medium', 'High'][alert.severity - 1] || 'Medium'
        }));
    } catch (error) {
        console.error('Error fetching threat data:', error);
        throw error;
    }
};

// Behavior Analytics Endpoints
export const fetchBehaviorAnalytics = async (timePeriod = 'daily') => {
    try {
        const [logsResponse, alertsResponse] = await Promise.all([
            fetch(`${API_BASE_URL}/logs/recent?limit=500`),
            fetch(`${API_BASE_URL}/alerts/active`)
        ]);
        
        const [logsData, alertsData] = await Promise.all([
            handleResponse(logsResponse),
            handleResponse(alertsResponse)
        ]);

        return {
            metrics: [
                { name: 'File Access Frequency', value: logsData.data.filter(l => l.event_type === 'file_access').length, trend: 'up' },
                { name: 'Unusual Access Times', value: logsData.data.filter(l => l.risk_score > 50).length, trend: 'stable' },
                { name: 'Sensitive File Access', value: alertsData.data.length, trend: 'up' }
            ],
            anomalies: alertsData.data.map(alert => ({
                employee: alert.description.split(' ')[0],
                type: alert.description,
                severity: ['low', 'medium', 'high'][alert.severity - 1] || 'medium'
            }))
        };
    } catch (error) {
        console.error('Error fetching behavior analytics:', error);
        throw error;
    }
};

// Existing alert resolution endpoint
export const resolveAlert = async (alertId) => {
    try {
        const response = await fetch(`${API_BASE_URL}/alerts/${alertId}/resolve`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
        });
        const data = await handleResponse(response);
        return data.status === 'success';
    } catch (error) {
        console.error('Error resolving alert:', error);
        throw error;
    }
};
