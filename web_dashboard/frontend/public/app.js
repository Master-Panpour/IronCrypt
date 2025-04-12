// Initialize application when DOM is fully loaded
document.addEventListener('DOMContentLoaded', function() {
    // Display current OS information
    const systemInfo = document.getElementById('system-info');
    systemInfo.textContent = `Running on ${getOS()} OS`;

    // Load all data sections
    loadEmployeeData();
    loadThreatData();
    loadBehaviorAnalytics();

    // Set up event listeners
    document.getElementById('refresh').addEventListener('click', function() {
        loadEmployeeData();
        loadThreatData();
        loadBehaviorAnalytics();
    });

    document.getElementById('search').addEventListener('input', function(e) {
        filterEmployees(e.target.value);
    });
});

function getOS() {
    const userAgent = navigator.userAgent;
    if (userAgent.includes('Windows')) return 'Windows';
    if (userAgent.includes('Linux')) return 'Linux';
    if (userAgent.includes('Mac')) return 'macOS';
    return 'Unknown';
}

async function loadEmployeeData() {
    try {
        const response = await fetch('http://localhost:8000/logs/recent?limit=100');
        const data = await response.json();
        displayEmployees(transformEmployeeData(data.data));
    } catch (error) {
        console.error('Error loading employee data:', error);
    }
}

function transformEmployeeData(logs) {
    const employees = {};
    logs.forEach(log => {
        if (!employees[log.user_id]) {
            employees[log.user_id] = {
                name: log.user_id,
                logCount: 0,
                fileAccessCount: 0
            };
        }
        employees[log.user_id].logCount++;
        if (log.event_type === 'file_access') {
            employees[log.user_id].fileAccessCount++;
        }
    });
    return Object.values(employees);
}

function displayEmployees(employees) {
    const container = document.getElementById('employees-container');
    container.innerHTML = '';

    employees.forEach(emp => {
        const empElement = document.createElement('div');
        empElement.className = 'employee-card';
        empElement.innerHTML = `
            <h3>${emp.name}</h3>
            <p>Total logins: ${emp.logCount}</p>
            <p>File accesses: ${emp.fileAccessCount}</p>
        `;
        container.appendChild(empElement);
    });
}

async function loadThreatData() {
    try {
        const response = await fetch('http://localhost:8000/alerts/active');
        const data = await response.json();
        displayThreats(data.data);
    } catch (error) {
        console.error('Error loading threat data:', error);
    }
}

function displayThreats(threats) {
    const container = document.getElementById('threats-container');
    container.innerHTML = '';

    if (threats.length === 0) {
        container.innerHTML = '<p class="no-threats">No threats detected</p>';
        return;
    }

    threats.forEach(threat => {
        const threatElement = document.createElement('div');
        threatElement.className = `threat-alert ${getSeverityClass(threat.severity)}`;
        threatElement.innerHTML = `
            <h3>${threat.description.split(' ')[0]}</h3>
            <p>${threat.description} (${getSeverityText(threat.severity)})</p>
        `;
        container.appendChild(threatElement);
    });
}

function getSeverityClass(severity) {
    if (severity >= 3) return 'high';
    if (severity >= 2) return 'medium';
    return 'low';
}

function getSeverityText(severity) {
    if (severity >= 3) return 'High';
    if (severity >= 2) return 'Medium';
    return 'Low';
}

async function loadBehaviorAnalytics() {
    try {
        const [logsResponse, alertsResponse] = await Promise.all([
            fetch('http://localhost:8000/logs/recent?limit=500'),
            fetch('http://localhost:8000/alerts/active')
        ]);
        
        const [logsData, alertsData] = await Promise.all([
            logsResponse.json(),
            alertsResponse.json()
        ]);

        displayAnalytics({
            metrics: [
                { 
                    name: 'File Access Frequency', 
                    value: logsData.data.filter(l => l.event_type === 'file_access').length,
                    trend: 'up' 
                },
                { 
                    name: 'Unusual Access Times', 
                    value: logsData.data.filter(l => l.risk_score > 50).length,
                    trend: 'stable' 
                },
                { 
                    name: 'Sensitive File Access', 
                    value: alertsData.data.length,
                    trend: 'up' 
                }
            ],
            anomalies: alertsData.data.map(alert => ({
                employee: alert.description.split(' ')[0],
                type: alert.description,
                severity: getSeverityClass(alert.severity)
            }))
        });
    } catch (error) {
        console.error('Error loading behavior analytics:', error);
    }
}

function displayAnalytics(analytics) {
    const container = document.getElementById('analytics-container');
    container.innerHTML = '';

    // Display metrics section
    const metricsSection = document.createElement('div');
    metricsSection.className = 'analytics-section';
    metricsSection.innerHTML = '<h3>Behavior Metrics</h3>';
    
    analytics.metrics.forEach(metric => {
        const metricElement = document.createElement('div');
        metricElement.className = 'metric-card';
        metricElement.innerHTML = `
            <h4>${metric.name}</h4>
            <p>Value: ${metric.value}</p>
            <p>Trend: ${metric.trend}</p>
        `;
        metricsSection.appendChild(metricElement);
    });
    container.appendChild(metricsSection);

    // Display anomalies section
    const anomaliesSection = document.createElement('div');
    anomaliesSection.className = 'analytics-section';
    anomaliesSection.innerHTML = '<h3>Detected Anomalies</h3>';
    
    if (analytics.anomalies.length === 0) {
        anomaliesSection.innerHTML += '<p class="no-anomalies">No anomalies detected</p>';
    } else {
        analytics.anomalies.forEach(anomaly => {
            const anomalyElement = document.createElement('div');
            anomalyElement.className = `anomaly-alert ${anomaly.severity}`;
            anomalyElement.innerHTML = `
                <h4>${anomaly.employee}</h4>
                <p>${anomaly.type}</p>
            `;
            anomaliesSection.appendChild(anomalyElement);
        });
    }
    container.appendChild(anomaliesSection);
}

function filterEmployees(searchTerm) {
    const cards = document.querySelectorAll('.employee-card');
    cards.forEach(card => {
        const name = card.querySelector('h3').textContent.toLowerCase();
        if (name.includes(searchTerm.toLowerCase())) {
            card.style.display = 'block';
        } else {
            card.style.display = 'none';
        }
    });
}

// Home page specific functions
async function loadHomeStats() {
    try {
        const [logsResponse, alertsResponse] = await Promise.all([
            fetch('http://localhost:8000/logs/recent?limit=100'),
            fetch('http://localhost:8000/alerts/active')
        ]);

        const [logsData, alertsData] = await Promise.all([
            logsResponse.json(),
            alertsResponse.json()
        ]);

        // Count unique employees
        const employees = new Set(logsData.data.map(log => log.user_id));
        
        // Update stats
        document.getElementById('total-employees').textContent = employees.size;
        document.getElementById('active-threats').textContent = alertsData.data.length;
        
        // Count high severity anomalies
        const highAnomalies = alertsData.data.filter(alert => alert.severity >= 3).length;
        document.getElementById('recent-anomalies').textContent = highAnomalies;
    } catch (error) {
        console.error('Error loading home stats:', error);
    }
}

// Initialize home page if needed
if (window.location.pathname.includes('home.html')) {
    document.addEventListener('DOMContentLoaded', function() {
        loadHomeStats();
        document.getElementById('system-info').textContent = `Running on ${getOS()} OS`;
    });
}

function navigateTo(page) {
    window.location.href = page;
}
