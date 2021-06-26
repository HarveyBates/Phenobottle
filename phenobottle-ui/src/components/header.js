import "./header.css";

function Header() {
	return (
		<header>
			<nav className="navbar">
				<ul className="navlist"> 
					<li className="navitem">
						<a className="navlink" href="/"><span>Devices</span></a>
					</li>
			
					<li className="navitem">
						<a className="navlink" href="/"><span>Control</span></a>
					</li>

					<li className="navitem">
						<a className="navlink" href="/"><span>Experiment</span></a>
					</li>
					
					<li className="navitem">
						<a className="navlink" href="/"><span>Settings</span></a>
					</li>
				</ul>
			</nav>
		</header>
	);
}

export default Header;
