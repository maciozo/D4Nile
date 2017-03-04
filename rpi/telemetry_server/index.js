const app = require('express')();
const http = require('http');
const WebSocket = require('ws');

app.get('/', function (req, res) {
	res.sendFile(__dirname+'/index.html');
});

const server = http.createServer(app);
const wss = new WebSocket.Server({server});

wss.on('connection', function connection(ws) {
	console.log('received connection!');
	ws.on('message', function incoming(message) {
		console.log('received', message);
	});
});

server.listen(8080, function listening() {
	console.log('listening on', server.address().port);
});

setInterval(() => {
	wss.clients.forEach((client) => {
		if (client.readyState === WebSocket.OPEN) {
			client.send('STATUSUPDATE: sensors and shit');
		}
	});
}, 3000);
