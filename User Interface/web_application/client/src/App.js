import './App.css';
import Navbar from './navbar.jsx';
import React, {Fragment, useState} from 'react';
import Plot from 'react-plotly.js';
import Dashboard from './components/dashboard.js';
import Setup from './components/setup.js';

function App() {

	const [count, setCount] = useState(0);

	const increment = () => {
		setCount(count + 1);
	};
	
	return (
		<div>
			<Navbar/>
			<main>
				
			</main>
		</div>
	);
}

export default App;
