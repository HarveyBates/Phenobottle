import React, { useState, Fragment } from 'react';
import mqtt from 'mqtt';
import './App.css';
import Header from './components/header.js';
require('dotenv').config();

const options = {
	protocol: 'mqtts',
	keepalive: 60, 
	username: process.env.BROKER_UN, 
	password: process.env.BROKER_PSWD
};


function Message() {
	const [mesg, setMesg] = useState(<Fragment><em>nothing heard</em></Fragment>);
	var note;

	const [stat, setStat] = useState(<Fragment><em>Not connected</em></Fragment>);
	const client = mqtt.connect("mqtt://mqtt.phenobottle.xyz:8083", options);

	client.on('connect', function () {
		setStat("Connected");
	});
	
	client.on('subscribe', function () {
		console.log("Subscribed");
	});

	client.subscribe("test");

	client.on('message', function (topic, message) {
		note = message.toString();
		setMesg(note);
		client.end();
	});
	return (
		<header className="App-header">
			<p>You are: {stat}</p>
			<p>The message is: {mesg}</p>
		</header>
	);
}


function App() {
	return (
		<div className="App">
			<Header />
			<Message />
		</div>
	);
}

export default App;
