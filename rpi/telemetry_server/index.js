const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const SerialPort = require('serialport');
const UART = require('./uart_protocol.js');
const joystick = require('joystick');

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

function broadcast(argument) {
	wss.clients.forEach(client => {
		if (client.readyState === WebSocket.OPEN)
			client.send(JSON.stringify(argument));
	});
}

// Open serial port at /dev/serial0
const port = new SerialPort('/dev/serial0', {
	baudRate: 115200,
	parser: SerialPort.parsers.byteDelimiter([0x0A])
});

// broadcast all serial data
port.on('data', data => {
	console.log(data.map((datum, i) => String.fromCharCode(data[i])));
	const decoded = UART.code_to_command(Buffer.from(data));
	attitude[decoded.command.toLowerCase()] = decoded.data;
	broadcast(JSON.stringify(attitude));
});

const controller = new joystick(0,3500,350);
controller.on('axis', data => {
	if (data.number == 2) {
		let angle = (data.value/32768)*0.17;
		attitude.roll = angle;
		broadcast(attitude);
	} else if (data.number == 5) {
		let angle = (data.value/32768)*0.17;
		attitude.pitch = angle;
		broadcast(attitude);
	}
});
