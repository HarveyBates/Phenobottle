// React imports 
import React from 'react';
import { BrowserRouter as Router, Route, Switch, Link, withRouter } from "react-router-dom";

// User imports 
import { Login } from "../../containers/pages/login.js";
import { UserContext } from "../../containers/usercontext.js";
import "./navbar.css";

// Logo / icon imports
import { library } from '@fortawesome/fontawesome-svg-core';
import { fas } from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
library.add(fas);

export class Navbar extends React.Component {
	render() {
		return (
			<nav className="navbar">
				<ul className="navlist"> 
					<Link to="/device-control" className="navitem">
						<FontAwesomeIcon className="navimg" icon={["fas", "flask"]}/>
						<span>Device Control</span>
					</Link>

					<Link path="/experiment" className="navitem">
						<FontAwesomeIcon className="navimg" icon={["fas", "clone"]}/>
						<span>Experiment</span>
					</Link>

					<Link path="/dashboard" className="navitem">
						<FontAwesomeIcon className="navimg" icon={["fas", "chart-line"]}/>
						<span>Dashboard</span>
					</Link>

					<Link path="/settings" className="navitem">
						<FontAwesomeIcon className="navimg" icon={["fas", "cogs"]}/>
						<span>Settings</span>
					</Link>
				</ul>
			</nav>
		);
	}
}

