import React, { useState, Fragment } from 'react';
import { BrowserRouter as Router, Route, Switch, Link } from "react-router-dom";
import mqtt from 'mqtt';
import './App.css';
import { Navbar } from './components/navbar/navbar.js';
import { Login } from "./containers/pages/home.js";
require('dotenv').config();

const options = {
	protocol: 'mqtts',
	keepalive: 60, 
	username: "",
	password: ""
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
		<header className="mainpage">
			<p>You are: {stat}</p>
			<p>The message is: {mesg}</p>
		</header>
	);
}

function App() {
	return (
		<Router>
			<div className="App">
				<Route path="/Login" exact component={Login} />
			</div>
		</Router>
	);
}

export default App;
