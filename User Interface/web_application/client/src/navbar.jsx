import React from 'react';
import { BrowserRouter as Router, 
	Switch, 
	Route, 
	NavLink } from 'react-router-dom';

import Setup from './components/setup.js';
import Dashboard from './components/dashboard.js';

function Navbar(){
	return(
		<Router>
			<nav className="navbar">
				<ul className="navbar-nav">
					<li className="logo">
						<a href="#" className="nav-link">
							<span className="link-text">Phenobottle</span>
							<i className="fas fa-arrow-alt-circle-left"></i>
						</a> 
					</li>
					<li className="nav-item">
						<NavLink exact to="/dashboard"
							activeClassName="selected">
						<a href="#" className="nav-link">
							<i className="fas fa-home"></i>
							<span className="link-text">Dashboard</span>
						</a> 
						</NavLink>
					</li> 
					<li className="nav-item">
						<NavLink exact to ="/setup"
							activeClassName="selected">
						<a href="#" className="nav-link">
							<i className="fas fa-flask"></i>
							<span className="link-text">Setup</span>
						</a>
						</NavLink>
					</li>
					<li className="nav-item">
						<a href="#" className="nav-link">
							<i className="fas fa-sliders-h"></i>
							<span className="link-text">Sensors</span>
						</a> 
					</li>
					<li className="nav-item">
						<a href="#" className="nav-link">
							<i className="fas fa-leaf"></i>
							<span className="link-text">Environment</span>
						</a> 
					</li>
					<li className="nav-item">
						<a href="#" className="nav-link">
							<i className="fas fa-cogs"></i>
							<span className="link-text">Settings</span>
						</a>
					</li>
				</ul>
			</nav>
			<Switch>
				<Route path="/setup">
					<Setup/>
				</Route>
				<Route path="/dashboard">
					<Dashboard/>
				</Route>
			</Switch>
		</Router>
	);
}

export default Navbar;





