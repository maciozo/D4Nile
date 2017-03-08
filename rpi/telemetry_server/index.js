const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const UART = require('./uart_protocol.js');
const joystick = require('joystick');


//////////////// BEGIN WEB SERVER AND WEBSOCKETS ////////////////
// serve files directly from /public 
const app = express();
app.use(express.static('public'));

const server = http.createServer(app);
const wss = new WebSocket.Server({server});

// listen on 8080
server.listen(8080, function listening() {
	console.log('listening on', server.address().port);
});

function broadcast(argument) {
	wss.clients.forEach(client => {
		if (client.readyState === WebSocket.OPEN)
			client.send(JSON.stringify(argument));
	});
}
//////////////// END WEB SERVER AND WEBSOCKETS ////////////////

//////////////// BEGIN PS4 CONTROLLER AND SERIAL ////////////////
const attitude = {
	roll: 0,
	pitch: 0,
	yaw: 0
};

UART.init_serial('/dev/serial0');
UART.on_message(command => {
	switch(command.command) {
		case 'ROLL':
		case 'PITCH':
		case 'YAW':
			// update relevant attitude field
			attitude[command.command] = command.data;
			// send new attitude to connected clients
			broadcast(JSON.stringify(attitude));
			break;
		default:
			console.log('Unhandled command', command);
	}
});

const controller = new joystick(0,3500,350);
// analogue inputs: triggers, gyro, sticks
controller.on('axis', data => {
	if (data.number == 2) { // Right Stick horizontal
		// convert number to radians between -0.17,+0.17
		let angle = (data.value/32768)*0.17;
		attitude.roll = angle;
		UART.send_message('ROLL', angle);
		broadcast(attitude); // temp
	} else if (data.number == 5) { // Right Stick vertical
		// convert number to radians between -0.17,+0.17
		let angle = (data.value/32768)*0.17;
		attitude.pitch = angle;
		UART.send_message('PITCH', angle);
		broadcast(attitude); // temp
	} 
	// TODO: work out which way round axis 0 and 1 are (yaw vs throttle)
});
// TODO: digital inputs (buttons)

//////////////// END PS4 CONTROLLER AND SERIAL ////////////////
