const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const SerialPort = require('serialport');

// serve files directly from /public 
const app = express();
app.use(express.static('public'));

const server = http.createServer(app);
const wss = new WebSocket.Server({server});

// listen on 8080
server.listen(8080, function listening() {
	console.log('listening on', server.address().port);
});

// loop every 500ms sending new attitude data to connected clients with silly values
const attitude = {
	roll: 0,
	pitch: 0,
	yaw: 0
};
setInterval(() => {
	attitude.pitch += 0.05;
	attitude.roll += 0.05;
	broadcast(attitude);
}, 500);

function broadcast(argument) {
	wss.clients.forEach(client => {
		if (client.readyState === WebSocket.OPEN)
			client.send(JSON.stringify(argument));
	});
}

// Open serial port at /dev/serial0
const port = new SerialPort('/dev/serial0', {
	baudRate: 115200,
	parser: SerialPort.parsers.readline('\n')
});

// broadcast all serial data
port.on('data', data => {
	broadcast(data);
});
