import React, { useState, Fragment } from 'react';
import logo from './logo.svg';
import './App.css';
import mqtt from 'mqtt';

const options = {
	protocol: 'mqtts',
	keepalive: 60, 
	username: '*****',
	password: '*****'

};

const client = mqtt.connect("mqtt://mqtt.phenobottle.xyz:8083", options);

client.subscribe("test");

function App() {
	const [mesg, setMesg] = useState(<Fragment><em>nothing heard</em></Fragment>);
	var note;
	client.on('message', function (topic, message) {
		note = message.toString();
		setMesg(note);
		console.log(note);
		client.end();
	});
	return (
		<div className="App">
			<header className="App-header">
				<img src={logo} className="App-logo" alt="logo" />
				<p>
					Edit <code>src/App.js</code> and save to reload.
				</p>
				<p>The message is: {mesg}</p>
				<a
					className="App-link"
					href="https://reactjs.org"
					target="_blank"
					rel="noopener noreferrer"
				>
					Learn React
				</a>
			</header>
		</div>
	);
}

export default App;
