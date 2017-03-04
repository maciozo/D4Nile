const app = require('express')();
const http = require('http');
const WebSocket = require('ws');

// send index.html for root route
app.get('/', function (req, res) {
	res.sendFile(__dirname+'/index.html');
});
app.get('/dronemodel', function(req, res) {
	res.sendFile(__dirname+'/drone.json');
});

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
	wss.clients.forEach((client) => {
		if (client.readyState === WebSocket.OPEN) {
			client.send(JSON.stringify(attitude));
		}
	});
}, 500);
