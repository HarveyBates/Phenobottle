import { UserContext } from "./containers/usercontext.js";
import React, { useState, Fragment } from 'react';
import { BrowserRouter as Router, Route, Switch, Link } from "react-router-dom";
import './App.css';
import { Navbar } from './components/navbar/navbar.js';
import { Login } from "./containers/pages/login.js";
import { DeviceControl } from "./containers/pages/device-control.js";

function App() {
	return (
		<Router>
		<div className="App">
			<UserContext.Provider value="Hello">
				<Route path="/" exact component={ Login }/>
				<Route path="/device-control" exact component={ DeviceControl }/>
			</UserContext.Provider>
		</div>
		</Router>
	);
}

export default App;
