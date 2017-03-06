exports.code_to_command = function(buffer) {
	switch(buffer.readUInt8(0)) {
		case 0x34: return 'ROLL ' + buffer.readFloatLE(1);
		default: 
			console.log('Unknown command');
			break;
	}
}

exports.command_to_code = function(command, data) {
	let buf = Buffer.alloc(6);
	switch(command) {
		case 'ROLL': 
			buf.writeUInt8(0x34, 0);
			buf.writeFloatLE(data, 1);
			break;
		default: 
			console.log('Unknown command');
			break;
	}
	buf.write('\n', 5);
	return buf;
}
