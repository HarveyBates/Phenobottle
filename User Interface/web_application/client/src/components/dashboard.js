import React, {Component, Fragment, useState} from 'react';
import Plot from 'react-plotly.js';

class Dashboard extends Component{ 
	render(){
	return (
		<main>
		<h1>DASHBOARD</h1>
		<div className="graph-card">
		<Plot
			useResizeHandler
			style={{width: '100%', height: '100%'}}
			data={[
				{
				x: [1, 2, 3, 4, 5, 6],
				y: [1, 3, 6, 12, 24, 48],
				type: 'scatter', 
				fill: 'tozeroy',
						fillcolor:'rgba(131, 90, 241, 0.25)',
				mode: 'lines+markers',
				marker:{color: 'blue'},
				}
			]}
			layout={{
				autosize: true,
			}}
		/>
		</div>
		</main>
	)
	}
}

export default Dashboard;
