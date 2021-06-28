import React from 'react';
import { Link, withRouter } from 'react-router-dom';
import "./navbar.css";
import { library } from '@fortawesome/fontawesome-svg-core';
import { fas } from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
library.add(fas);

function Navbar(props){
	return (
		<div>
			<nav className="navbar">
				<ul className="navlist"> 
					<Link to="/devices" className="navitem">
						<li className="navitem">
							<FontAwesomeIcon className="navimg" icon={["fas", "flask"]}/>
							<a className="navlink" href="/"><span>Devices</span></a>
						</li>
					</Link>

					<Link to="/control" className="navitem">
						<li className="navitem">
							<FontAwesomeIcon className="navimg" icon={["fas", "sliders-h"]}/>
							<a className="navlink" href="/"><span>Control</span></a>
						</li>
					</Link>

					<Link to="/experiment" className="navitem">
						<li className="navitem">
							<FontAwesomeIcon className="navimg" icon={["fas", "clone"]}/>
							<a className="navlink" href="/"><span>Experiment</span></a>
						</li>
					</Link>

					<Link to="/settings" className="navitem">
						<li className="navitem">
							<FontAwesomeIcon className="navimg" icon={["fas", "cogs"]}/>
							<a className="navlink" href="/"><span>Settings</span></a>
						</li>
					</Link>
				</ul>
			</nav>
		</div>
	);
}

export default withRouter(Navbar);
