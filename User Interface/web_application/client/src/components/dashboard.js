import Plot from 'react-plotly.js';

var xVals = [1, 2, 3, 4, 5, 6, 7, 8];
var yVals = [2, 4, 6, 8, 10, 12, 14, 16];

const Dashboard = () => {
	return (
		<>
		<div className="dashboard">
		<h1>DASHBOARD</h1>
		<div className="graph-card">
		<Plot
			useResizeHandler
			style={{width: '100%', height: '100%'}}
			data={[
				{
				x: xVals,
				y: yVals,
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
			<button>Update</button>
		</div>
		</div>
		</>
	)
}

export default Dashboard;
