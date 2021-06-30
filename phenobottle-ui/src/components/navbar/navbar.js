// React imports 
import React from 'react';
import { BrowserRouter as Router, Route, Switch, Link, withRouter } from "react-router-dom";

// User imports 
import { Home } from "../../containers/pages/home.js";
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
			<Router>
				<nav className="navbar">
					<ul className="navlist"> 
						<li className="navitem">
							<Link to="/" className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "home"]}/>
								<span>Home</span>
							</Link>
						</li>

						<li className="navitem">
							<Link to="/devices" className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "flask"]}/>
								<span>Devices</span>
							</Link>
						</li>

						<li className="navitem">
							<Link path="/control" className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "sliders-h"]}/>
								<span>Control</span>
							</Link>
						</li>

						<li className="navitem">
							<Link path="/experiment" className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "clone"]}/>
								<span>Experiment</span>
							</Link>
						</li>

						<li className="navitem">
							<Link path="/settings" className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "cogs"]}/>
								<span>Settings</span>
							</Link>
						</li>
					</ul>
				</nav>
				<UserContext.Provider value="Hello">
					<Route path="/" exact component={ Home }/>
				</UserContext.Provider>
			</Router>
		);
	}
}

