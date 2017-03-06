exports.code_to_command = function(buffer) {
	switch(buffer.readUInt8(0)) {
		case 0x22: return {command: 'ROLL', data: buffer.readFloatLE(1)};
		case 0x23: return {command: 'PITCH', data: buffer.readFloatLE(1)};
		case 0x20: return {command: 'YAW', data: buffer.readFloatLE(1)}; 
		default: 
			console.log('Unknown command');
			break;
	}
}

exports.command_to_code = function(command, data) {
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
