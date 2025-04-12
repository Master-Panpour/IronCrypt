import React, { useEffect, useState } from 'react';
import { Card, CardContent } from './ui/Card';
import { Button } from './ui/Button';
import { Sun, Moon } from 'lucide-react';

export default function InsiderThreatDashboard() {
  const [darkMode, setDarkMode] = useState(false);
  const [systemHealth, setSystemHealth] = useState('loading');
  const [metrics, setMetrics] = useState({ threats: 0, highRiskUsers: [] });
  const [activities, setActivities] = useState([]);
  const [alerts, setAlerts] = useState([]);
  const [page, setPage] = useState(1);

  useEffect(() => {
    // Mock API calls
    const fetchData = async () => {
      try {
        const healthRes = await fetch('/api/v1/health');
        const healthData = await healthRes.json();
        setSystemHealth(healthData.status || 'healthy');
        
        const metricsRes = await fetch('/api/v1/metrics');
        const metricsData = await metricsRes.json();
        setMetrics(metricsData || { threats: 0, highRiskUsers: [] });
        
        const activitiesRes = await fetch(`/api/v1/classify?page=${page}`);
        const activitiesData = await activitiesRes.json();
        setActivities(activitiesData || []);
        
        // Simulated alerts
        setAlerts([
          { username: 'jdoe', type: 'Anomalous Login', timestamp: new Date().toISOString() },
          { username: 'asmith', type: 'File Exfiltration', timestamp: new Date(Date.now() - 3600000).toISOString() }
        ]);
      } catch (error) {
        console.error('Error fetching data:', error);
        setSystemHealth('error');
      }
    };

    fetchData();
  }, [page]);

  return (
    <div className={darkMode ? 'dark' : ''}>
      <div className="min-h-screen bg-gray-100 dark:bg-gray-900 text-gray-900 dark:text-white flex flex-col">
        {/* Header */}
        <header className="bg-gradient-to-r from-blue-600 to-indigo-600 p-4 flex justify-between items-center text-white">
          <h1 className="text-xl font-bold">IronCrypt Insider Threat Dashboard</h1>
          <div className="flex items-center space-x-4">
            <input 
              type="search" 
              placeholder="Search..." 
              className="px-2 py-1 rounded text-black" 
            />
            <Button 
              onClick={() => setDarkMode(!darkMode)} 
              className="flex items-center"
            >
              {darkMode ? <Sun className="mr-2" /> : <Moon className="mr-2" />}
              Toggle Theme
            </Button>
          </div>
        </header>

        <div className="flex flex-1">
          {/* Sidebar */}
          <aside className="w-64 bg-white dark:bg-gray-800 p-4 space-y-4 hidden md:block">
            <nav className="space-y-2">
              <a href="#" className="block hover:bg-gray-200 dark:hover:bg-gray-700 p-2 rounded">Dashboard</a>
              <a href="#" className="block hover:bg-gray-200 dark:hover:bg-gray-700 p-2 rounded">User Activity</a>
              <a href="#" className="block hover:bg-gray-200 dark:hover:bg-gray-700 p-2 rounded">Threat Reports</a>
              <a href="#" className="block hover:bg-gray-200 dark:hover:bg-gray-700 p-2 rounded">Settings</a>
            </nav>
          </aside>

          {/* Main Content */}
          <main className="flex-1 p-6 space-y-6">
            {/* Stats Cards */}
            <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
              <Card className="bg-gradient-to-br from-red-500 to-pink-500 text-white">
                <CardContent>
                  <h2 className="text-lg font-semibold">Total Threats</h2>
                  <p className="text-3xl mt-2">{metrics.threats}</p>
                </CardContent>
              </Card>
              
              <Card className="bg-gradient-to-br from-yellow-500 to-orange-500 text-white">
                <CardContent>
                  <h2 className="text-lg font-semibold">High-Risk Users</h2>
                  <div className="mt-2">
                    {metrics.highRiskUsers.length > 0 ? (
                      <ul className="list-disc pl-4">
                        {metrics.highRiskUsers.map((user, i) => (
                          <li key={i}>{user}</li>
                        ))}
                      </ul>
                    ) : (
                      <p>No high-risk users</p>
                    )}
                  </div>
                </CardContent>
              </Card>
              
              <Card className="bg-gradient-to-br from-green-500 to-emerald-500 text-white">
                <CardContent>
                  <h2 className="text-lg font-semibold">System Health</h2>
                  <p className="mt-2">
                    Status: <span className={`font-bold ${
                      systemHealth === 'healthy' ? 'text-green-200' : 
                      systemHealth === 'error' ? 'text-red-200' : 'text-yellow-200'
                    }`}>
                      {systemHealth}
                    </span>
                  </p>
                </CardContent>
              </Card>
            </div>

            {/* Activity Table */}
            <section className="bg-white dark:bg-gray-800 rounded-lg shadow p-4">
              <h2 className="text-xl font-semibold mb-4">Recent Activities</h2>
              <div className="overflow-x-auto">
                <table className="w-full">
                  <thead className="bg-gray-200 dark:bg-gray-700">
                    <tr>
                      <th className="p-2 text-left">User</th>
                      <th className="p-2 text-left">Action</th>
                      <th className="p-2 text-left">Time</th>
                      <th className="p-2 text-left">Risk</th>
                    </tr>
                  </thead>
                  <tbody>
                    {activities.map((activity, i) => (
                      <tr key={i} className="border-b border-gray-200 dark:border-gray-700">
                        <td className="p-2">{activity.username || 'Unknown'}</td>
                        <td className="p-2">{activity.action || 'Unknown action'}</td>
                        <td className="p-2">{new Date(activity.timestamp).toLocaleString()}</td>
                        <td className={`p-2 font-bold ${
                          activity.risk === 'high' ? 'text-red-500' :
                          activity.risk === 'medium' ? 'text-yellow-500' : 'text-green-500'
                        }`}>
                          {activity.risk || 'low'}
                        </td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>
              <div className="flex justify-end mt-4 space-x-2">
                <Button 
                  onClick={() => setPage(p => Math.max(1, p - 1))}
                  disabled={page === 1}
                >
                  Previous
                </Button>
                <Button onClick={() => setPage(p => p + 1)}>
                  Next
                </Button>
              </div>
            </section>

            {/* Alerts Section */}
            <section className="bg-white dark:bg-gray-800 rounded-lg shadow">
              <details className="p-4">
                <summary className="font-semibold cursor-pointer">Active Alerts ({alerts.length})</summary>
                <div className="mt-2 space-y-2">
                  {alerts.map((alert, i) => (
                    <div key={i} className="flex justify-between items-center p-2 border-b border-gray-200 dark:border-gray-700">
                      <div>
                        <p className="font-medium">{alert.username} - {alert.type}</p>
                        <p className="text-sm text-gray-600 dark:text-gray-400">
                          {new Date(alert.timestamp).toLocaleString()}
                        </p>
                      </div>
                      <div className="flex space-x-2">
                        <Button size="sm">Investigate</Button>
                        <Button variant="outline" size="sm">Dismiss</Button>
                      </div>
                    </div>
                  ))}
                </div>
              </details>
            </section>
          </main>
        </div>

        {/* Footer */}
        <footer className="bg-gray-200 dark:bg-gray-800 p-4 text-center text-sm">
          <p>© {new Date().getFullYear()} IronCrypt Security</p>
        </footer>
      </div>
    </div>
  );
}
