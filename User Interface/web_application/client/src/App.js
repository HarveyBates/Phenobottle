import './App.css';
import Navbar from './navbar.jsx';
import React, {useState} from 'react';
import Plot from 'react-plotly.js';

function App() {

	const [count, setCount] = useState(0);

	const increment = () => {
		setCount(count + 1);
	};
	
	return (
		<div>
			<Navbar/>
			<main>
				<div className="graph-card">
					<Plot
						useResizeHandler
						style={{width: '100%', height: '100%'}}
						data={[
							{
							x: [1, 2, 3, 4, 5, 6],
							y: [1, 3, 6, 12, 24, 48],
							type: 'scatter', 
							mode: 'lines+markers',
							marker:{color: 'red'},
							}
						]}
						layout={{
							autosize: true,
						}}
					/>
					<h1>{count}</h1>
					<button onClick={increment}>Add One</button>
				</div>	
			</main>
		</div>
	);
}

export default App;
