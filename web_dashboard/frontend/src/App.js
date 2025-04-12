import React, { useState, useEffect } from 'react';
import './App.css';

function App() {
  const [employees, setEmployees] = useState([]);
  const [threats, setThreats] = useState([]);
  const [analytics, setAnalytics] = useState({
    metrics: [],
    anomalies: []
  });
  const [searchTerm, setSearchTerm] = useState('');

  useEffect(() => {
    // Load initial data
    loadEmployeeData();
    loadThreatData();
    loadBehaviorAnalytics();
  }, []);

  const getOS = () => {
    const userAgent = navigator.userAgent;
    if (userAgent.includes('Windows')) return 'Windows';
    if (userAgent.includes('Linux')) return 'Linux';
    if (userAgent.includes('Mac')) return 'macOS';
    return 'Unknown';
  };

  const loadEmployeeData = () => {
    const mockData = [
      { name: 'John Doe', logHrs: [8,7,8,6,9], logCount: 5, fileAccessCount: 42 },
      { name: 'Jane Smith', logHrs: [7,8,7,8,7], logCount: 5, fileAccessCount: 35 }
    ];
    setEmployees(mockData);
  };

  const loadThreatData = () => {
    const mockThreats = [
      { employee: 'John Doe', type: 'Excessive file access', severity: 'High' }
    ];
    setThreats(mockThreats);
  };

  const loadBehaviorAnalytics = () => {
    const mockAnalytics = {
      metrics: [
        { name: 'File Access Frequency', value: 42, trend: 'up' },
        { name: 'Unusual Access Times', value: 3, trend: 'stable' },
        { name: 'Sensitive File Access', value: 5, trend: 'up' }
      ],
      anomalies: [
        { employee: 'John Doe', type: 'After-hours access', severity: 'medium' },
        { employee: 'Jane Smith', type: 'High frequency access', severity: 'low' }
      ]
    };
    setAnalytics(mockAnalytics);
  };

  const handleRefresh = () => {
    loadEmployeeData();
    loadThreatData();
    loadBehaviorAnalytics();
  };

  const handleSearch = (e) => {
    setSearchTerm(e.target.value);
    console.log(`Filtering employees by: ${e.target.value}`);
  };

  return (
    <div className="App">
      <header>
        <div className="logo">IronCrypt</div>
        <div className="system-info">Running on {getOS()} OS</div>
      </header>

      <main>
        <section className="dashboard-section">
          <h1 className="section-title">Insider Threat Dashboard</h1>
          <div className="flex">
            <input 
              type="text" 
              className="search" 
              placeholder="Search employees..." 
              value={searchTerm}
              onChange={handleSearch}
            />
            <button className="refresh" onClick={handleRefresh}>Refresh Data</button>
          </div>
        </section>

        <section className="dashboard-section">
          <h2 className="section-title">Employee Activity</h2>
          <div className="employees-container grid">
            {employees.map(emp => (
              <div key={emp.name} className="employee-card">
                <h3>{emp.name}</h3>
                <p>Logged hours: {emp.logHrs.join(', ')}</p>
                <p>Total file accesses: {emp.fileAccessCount}</p>
              </div>
            ))}
          </div>
        </section>

        <section className="dashboard-section">
          <h2 className="section-title">Recent Threats</h2>
          <div className="threats-container">
            {threats.length === 0 ? (
              <p className="no-threats">No threats detected</p>
            ) : (
              threats.map(threat => (
                <div key={`${threat.employee}-${threat.type}`} className={`threat-alert ${threat.severity.toLowerCase()}`}>
                  <h3>{threat.employee}</h3>
                  <p>{threat.type} ({threat.severity})</p>
                </div>
              ))
            )}
          </div>
        </section>

        <section className="dashboard-section">
          <h2 className="section-title">Behavior Analytics</h2>
          <div className="analytics-container">
            <div className="analytics-section">
              <h3>Behavior Metrics</h3>
              {analytics.metrics.map(metric => (
                <div key={metric.name} className="metric-card">
                  <h4>{metric.name}</h4>
                  <p>Value: {metric.value}</p>
                  <p>Trend: {metric.trend}</p>
                </div>
              ))}
            </div>
            <div className="analytics-section">
              <h3>Detected Anomalies</h3>
              {analytics.anomalies.length === 0 ? (
                <p className="no-anomalies">No anomalies detected</p>
              ) : (
                analytics.anomalies.map(anomaly => (
                  <div key={`${anomaly.employee}-${anomaly.type}`} className={`anomaly-alert ${anomaly.severity}`}>
                    <h4>{anomaly.employee}</h4>
                    <p>{anomaly.type}</p>
                  </div>
                ))
              )}
            </div>
          </div>
        </section>
      </main>
    </div>
  );
}

export default App;
