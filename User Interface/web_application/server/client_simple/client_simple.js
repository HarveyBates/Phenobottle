const net = require('net');


const client = net.createConnection({port: 15001}, () => {
	console.log('CLIENT: Connected to server');
	client.write('This is the client');
});

client.on('data', (data) => {
	console.log(data.toString());
	//client.end();
});

client.on('end', () => {
	console.log('Client: Disconnected');
});
