import React from 'react';
import { BrowserRouter as Router, Route, Switch } from 'react-router-dom';
import Dashboard from './components/Dashboard';
import Alerts from './components/Alerts';

function App() {
    return (
        <Router>
            <Switch>
                <Route path="/" exact component={Dashboard} />
                <Route path="/alerts" component={Alerts} />
            </Switch>
        </Router>
    );
}

export default App;
