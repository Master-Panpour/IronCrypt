const API_BASE_URL = 'http://localhost:8000';

export const fetchRecentLogs = async (limit = 100) => {
    try {
        const response = await fetch(`${API_BASE_URL}/logs/recent?limit=${limit}`);
        if (!response.ok) {
            throw new Error('Failed to fetch logs');
        }
        return await response.json();
    } catch (error) {
        console.error('Error fetching logs:', error);
        return [];
    }
};

export const fetchActiveAlerts = async () => {
    try {
        const response = await fetch(`${API_BASE_URL}/alerts/active`);
        if (!response.ok) {
            throw new Error('Failed to fetch alerts');
        }
        return await response.json();
    } catch (error) {
        console.error('Error fetching alerts:', error);
        return [];
    }
};

export const resolveAlert = async (alertId) => {
    try {
        const response = await fetch(`${API_BASE_URL}/alerts/${alertId}/resolve`, {
            method: 'POST'
        });
        return response.ok;
    } catch (error) {
        console.error('Error resolving alert:', error);
        return false;
    }
};
