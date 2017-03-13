const SerialPort = require('serialport');

let port;

// Open the serial port
exports.init_serial = function(location) {
	port = new SerialPort(location, {
		baudRate: 115200,
		parser: SerialPort.parsers.byteLength(3) // 3 byte chunks
	});
	port.on('error', error => {
		throw new Error('serial not connected');
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
	console.log('sending message', command, data);
	let buf = command_to_code(command, data);
	console.log(buf.toString('hex'));
	port.write(buf);
}

// convert a binary buffer into an Object with the command
function code_to_command(buffer) {
	switch(buffer.readUInt8(0)) {
		case 0x22: return {command: 'ROLL', data: buffer.readUInt16BE(1)};
		case 0x23: return {command: 'PITCH', data: buffer.readUInt16BE(1)};
		case 0x20: return {command: 'YAW', data: buffer.readUInt16BE(1)}; 
		case 0x31: return {command: 'VOLTAGE', data: buffer.readUInt16BE(1)};
		default: 
			console.log('Unknown command');
			break;
	}
}

// serialise a command to binary
function command_to_code(command, data) {
	let buf = Buffer.alloc(3);
	switch(command) {
		case 'ROLL': 
			buf.writeUInt8(0x22, 0);
			buf.writeInt16BE(data, 1);
			break;
		case 'PITCH': 
			buf.writeUInt8(0x23, 0);
			buf.writeInt16BE(data, 1);
			break;
		case 'YAW': 
			buf.writeUInt8(0x20, 0);
			buf.writeInt16BE(data, 1);
			break;
		case 'THROTTLE': 
			buf.writeUInt8(0x21, 0);
			buf.writeInt16BE(data, 1);
			break;
		case 'ROLL_KP':
			buf.writeUInt8(0x28,0); // TODO: work out opcodes for PID commands
			buf.writeInt16BE(data, 1);
			break;
		case 'ROLL_KI':
			buf.writeUInt8(0x29,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'ROLL_KD':
			buf.writeUInt8(0x2A,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'PITCH_KP':
			buf.writeUInt8(0x2B,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'PITCH_KI':
			buf.writeUInt8(0x2C,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'PITCH_KD':
			buf.writeUInt8(0x2D,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'YAW_KP':
			buf.writeUInt8(0x2E,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'YAW_KI':
			buf.writeUInt8(0x2F,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'YAW_KD':
			buf.writeUInt8(0x30,0);
			buf.writeInt16BE(data, 1);
			break;
		case 'KILL':
			buf.writeUInt8(0x7F,0);
			buf.writeInt16BE(1, 1);
			break;
		default: 
			console.log('Unknown command');
			break;
	}
	return buf;
}
