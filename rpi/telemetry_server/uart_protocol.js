const SerialPort = require('serialport');

let port;

// Open the serial port
exports.init_serial = function(location) {
	port = new SerialPort(location, {
		baudRate: 115200,
		parser: SerialPort.parsers.byteDelimiter([0x0A]) // Produces a byte array delimited by a new line
	});
}

// register a handler to handle serial messages
exports.on_message = function(handler) {
	port.on('data', data => {
		// print byte array
		console.log('received message', data);
		// call handler
		callback(code_to_command(data));
	});
}

// serialise command to binary and send over serial
exports.send_message = function(command, data) {
	port.write(command_to_code(command, data));
}

// convert a binary buffer into an Object with the command
function code_to_command(buffer) {
	switch(buffer.readUInt8(0)) {
		case 0x22: return {command: 'ROLL', data: buffer.readFloatLE(1)};
		case 0x23: return {command: 'PITCH', data: buffer.readFloatLE(1)};
		case 0x20: return {command: 'YAW', data: buffer.readFloatLE(1)}; 
		default: 
			console.log('Unknown command');
			break;
	}
}

// serialise a command to binary
function command_to_code(command, data) {
	let buf = Buffer.alloc(6);
	switch(command) {
		case 'ROLL': 
			buf.writeUInt8(0x22, 0);
			buf.writeFloatLE(data, 1);
			break;
		case 'PITCH': 
			buf.writeUInt8(0x23, 0);
			buf.writeFloatLE(data, 1);
			break;
		case 'YAW': 
			buf.writeUInt8(0x20, 0);
			buf.writeFloatLE(data, 1);
			break;
		case 'THROTTLE': 
			buf.writeUInt8(0x21, 0);
			buf.writeFloatLE(data, 1);
			break;
		default: 
			console.log('Unknown command');
			break;
	}
	buf.write('\n', 5);
	return buf;
}
