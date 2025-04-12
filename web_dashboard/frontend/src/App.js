import React, { useState, useEffect } from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import Dashboard from './components/Dashboard';
import Alerts from './components/Alerts';
import Navbar from './components/Navbar';
import Sidebar from './components/Sidebar';
import { fetchEmployeeData, fetchThreatData } from './services/api';
import './App.css';

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
    primary: {
      main: '#90caf9',
    },
    secondary: {
      main: '#f48fb1',
    },
  },
});

function App() {
  const [logs, setLogs] = useState([]);
  const [alerts, setAlerts] = useState([]);
  const [loading, setLoading] = useState(true);
  const [sidebarOpen, setSidebarOpen] = useState(true);
  const [darkMode, setDarkMode] = useState(true);

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
        console.error('Failed to load data:', error);
      } finally {
        setLoading(false);
      }
    };
    loadData();
    
    const interval = setInterval(loadData, 10000);
    return () => clearInterval(interval);
  }, []);

  const toggleTheme = () => {
    setDarkMode(!darkMode);
  };

  return (
    <ThemeProvider theme={darkMode ? darkTheme : createTheme({ palette: { mode: 'light' } })}>
      <CssBaseline />
      <Router>
        <div className="app-container">
          <Navbar 
            onMenuClick={() => setSidebarOpen(!sidebarOpen)} 
            onThemeToggle={toggleTheme}
            darkMode={darkMode}
          />
          <div className="main-content">
            <Sidebar open={sidebarOpen} />
            <div className="content-area">
              {loading ? (
                <div className="loading-spinner">Loading...</div>
              ) : (
                <Routes>
                  <Route path="/" element={<Dashboard logs={logs} alerts={alerts} />} />
                  <Route path="/alerts" element={<Alerts alerts={alerts} setAlerts={setAlerts} />} />
                </Routes>
              )}
            </div>
          </div>
        </div>
      </Router>
    </ThemeProvider>
  );
}

export default App;
