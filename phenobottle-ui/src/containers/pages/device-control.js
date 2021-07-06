import React, { useState, Fragment } from 'react';
import '../styles/device-control.css';
import { Navbar } from '../../components/navbar/navbar.js';
import mqtt from 'mqtt';
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


export function DeviceControl() {
	return (
		<>
			<Navbar/>
			<div class="device-card">
				<div class="device-heading">
					<h3>Connected Devices</h3>
					<button href="#">SEARCH</button>
				</div>
				<table class="device-table">
					<tr>
						<th>Phenobottle Name</th>
						<th>Unique ID</th>
						<th>Version</th>
						<th>Treatment</th>
						<th>Replicate</th>
						<th>Status</th>
						<th>Selected</th>
					</tr>
					<tr>
						<td>Home #1</td>
						<td>182bds&191@</td>
						<td>0.2</td>
						<td>Control</td>
						<td>One</td>
						<td>Online</td>
						<td>Yes</td>
					</tr>
				</table>
		</div>
		</>
	);
}




