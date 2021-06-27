import "./header.css";
import { library } from '@fortawesome/fontawesome-svg-core';
import { fas } from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
library.add(fas);

function Header() {
	return (
		<header>
			<nav className="navbar">
				<ul className="navlist"> 
					<li className="navitem">
						<FontAwesomeIcon class="navimg" icon={["fas", "flask"]}/>
						<a className="navlink" href="/"><span>Devices</span></a>
					</li>

					<li className="navitem">
						<FontAwesomeIcon class="navimg" icon={["fas", "sliders-h"]}/>
						<a className="navlink" href="/"><span>Control</span></a>
					</li>

					<li className="navitem">
						<FontAwesomeIcon class="navimg" icon={["fas", "clone"]}/>
						<a className="navlink" href="/"><span>Experiment</span></a>
					</li>

					<li className="navitem">
						<FontAwesomeIcon class="navimg" icon={["fas", "cogs"]}/>
						<a className="navlink" href="/"><span>Settings</span></a>
					</li>
				</ul>
			</nav>
		</header>
	);
}

export default Header;
