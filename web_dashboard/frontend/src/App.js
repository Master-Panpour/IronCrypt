import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Dashboard from './components/Dashboard';
import Alerts from './components/Alerts';

function App() {
    return (
        <Router>
            <Routes>
                <Route path="/" element={<Dashboard />} />
                <Route path="/alerts" element={<Alerts />} />
            </Routes>
        </Router>
    );
}

export default App;
