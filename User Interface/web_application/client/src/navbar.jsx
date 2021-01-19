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
						<NavLink exact to="/" activeClassName="selected" className="nav-link">
							<span className="link-text">Phenobottle</span>
							<i className="fas fa-arrow-alt-circle-left"></i>
						</NavLink> 
					</li>
					<li className="nav-item">
						<NavLink exact to="/dashboard"
							activeClassName="selected" className="nav-link">
							<i className="fas fa-home"></i>
							<span className="link-text">Dashboard</span>
						</NavLink>
					</li> 
					<li className="nav-item">
						<NavLink exact to ="/setup"
							activeClassName="selected" className="nav-link">
							<i className="fas fa-flask"></i>
							<span className="link-text">Setup</span>
						</NavLink>
					</li>
					<li className="nav-item">
						<NavLink exact to="/sensors" 
							activeClassName="selected" className="nav-link">
							<i className="fas fa-sliders-h"></i>
							<span className="link-text">Sensors</span>
						</NavLink>
					</li>
					<li className="nav-item">
						<NavLink exact to= "/environment" activeClassName="selected" className="nav-link">
							<i className="fas fa-leaf"></i>
							<span className="link-text">Environment</span>
						</NavLink>
					</li>
					<li className="nav-item">
						<NavLink exact to="/settings" activeClassName="selected" className="nav-link">
							<i className="fas fa-cogs"></i>
							<span className="link-text">Settings</span>
						</NavLink>
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





