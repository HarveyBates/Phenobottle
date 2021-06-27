import React from 'react';
import "./navbar.css";
import { library } from '@fortawesome/fontawesome-svg-core';
import { fas } from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
library.add(fas);

export class Navbar extends React.Component {
	render() {
		return (
			<div>
				<nav className="navbar">
					<ul className="navlist"> 
						<a href="/" className="navitem">
							<li className="navitem">
							<FontAwesomeIcon className="navimg" icon={["fas", "flask"]}/>
								<a className="navlink" href="/"><span>Devices</span></a>
							</li>
						</a>

						<a href="/" className="navitem">
							<li className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "sliders-h"]}/>
								<a className="navlink" href="/"><span>Control</span></a>
							</li>
						</a>

						<a href="/" className="navitem">
							<li className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "clone"]}/>
								<a className="navlink" href="/"><span>Experiment</span></a>
							</li>
						</a>

						<a href="/" className="navitem">
							<li className="navitem">
								<FontAwesomeIcon className="navimg" icon={["fas", "cogs"]}/>
								<a className="navlink" href="/"><span>Settings</span></a>
							</li>
						</a>
					</ul>
				</nav>
			</div>
		);
	}
}

