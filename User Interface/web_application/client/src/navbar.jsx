import React from 'react';

function Navbar(){
	return(
			<nav className="navbar">
				<ul className="navbar-nav">
					<li className="logo">
						<a href="#" className="nav-link">
							<span className="link-text">Phenobottle</span>
							<i className="fas fa-arrow-alt-circle-left"></i>
						</a> 
					</li>
					<li className="nav-item">
						<a href="#" className="nav-link">
							<i className="fas fa-home"></i>
							<span className="link-text">Dashboard</span>
						</a> 
					</li> 
					<li className="nav-item">
						<a href="#" className="nav-link">
							<i className="fas fa-flask"></i>
							<span className="link-text">Setup</span>
						</a>
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
	);
}

export default Navbar;





