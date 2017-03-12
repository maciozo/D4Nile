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

wss.on('connection', ws => {
	ws.on('message', message => {
		message = JSON.parse(message);
		switch(message.command) {
			case 'ROLL_KP':
			case 'ROLL_KI':
			case 'ROLL_KD':
			case 'PITCH_KP':
			case 'PITCH_KI':
			case 'PITCH_KD':
			case 'YAW_KP':
			case 'YAW_KI':
			case 'YAW_KD':
			case 'ROLL':
			case 'PITCH':
				UART.send_message(message.command, message.data);
				break;
			default:
				console.log('Unknown command from client', message);
		}
	});
});
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
			attitude[command.command.toLowerCase()] = command.data;
			// send new attitude to connected clients
			broadcast({command: 'ATTITUDE', data: attitude});
			break;
		case 'VOLTAGE':
			broadcast({command: 'VOLTAGE', data: command.data});
			break;
		default:
			console.log('Unhandled command from flight control', command);
	}
});

const controller = new joystick(0,3500,350);
// analogue inputs: triggers, gyro, sticks
controller.on('axis', data => {
	if (data.number == 2) { // Right Stick horizontal
		// convert number to degrees between -10,+10
		const angle = (data.value/32768)*10;
		attitude.roll = angle;
		UART.send_message('ROLL', data.value);
	} else if (data.number == 5) { // Right Stick vertical
		// convert number to degrees between -10,+10
		const angle = (data.value/32768)*10;
		attitude.pitch = angle;
		UART.send_message('PITCH', data.value);
	} else if (data.number == 0) { // Left Stick horizontal
		// convert number to angular velocity in degrees/s between -45,45
		const rate = (data.value/32768)*45;
		UART.send_message('YAW', data.value);
	} else if (data.number == 1) { // Left Stick vertical
		// convert number to throttle between 0.95,1.0
		const throttle = (data.value/655360)+1;
		UART.send_message('THROTTLE', data.value); 
	}
});
controller.on('button', data => {
	if (data.number == 13) { // trackpad
		UART.send_message('KILL');
	}
});

//////////////// END PS4 CONTROLLER AND SERIAL ////////////////
