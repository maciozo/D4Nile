// container for current attitude from server
let attitude = {
	roll: 0,
	pitch: 0,
	yaw: 0,
};

// handle WebScoket
const socket = new WebSocket("ws://"+location.host);
socket.onopen = function (event) {
	socket_send({command: 'hello'});
}
const messages_container = document.querySelector('#messages');
socket.onmessage = event => {
	const command = JSON.parse(event.data);
	console.log('received', command);
	messages_container.innerHTML += `${command}<br/>`;
	switch(command.command) {
		case 'ATTITUDE':
			attitude = command.data;
			break;
		case 'VOLTAGE':
			document.querySelector('#voltage').innerHTML = command.data + 'V';
			break;
		default:
			console.log('Unknown command from server');
	}
}
function socket_send(object) {
	socket.send(JSON.stringify(object));
}

document.onkeydown = function(event) {
	switch (event.keyCode) {
		case 37: // left
			socket_send({command: 'ROLL', data: -32000});
			break;
		case 38: // up
			socket_send({command: 'PITCH', data: 32000});
			break;
		case 39: // right
			socket_send({command: 'ROLL', data: 32000});
			break;
		case 40: // down
			socket_send({command: 'PITCH', data: -32000});
			break;
		case 87: // w
			socket_send({command: 'THROTTLE', data: 32000});
			break;
		case 65: // a
			socket_send({command: 'YAW', data: -32000});
			break;
		case 83: // s
			socket_send({command: 'THROTTLE', data: -32000});
			break;
		case 68: // d
			socket_send({command: 'YAW', data: 32000});
			break;
	}
}

document.onkeyup = function(event) {
	switch (event.keyCode) {
		case 37: // left
			socket_send({command: 'ROLL', data: 0});
			break;
		case 38: // up
			socket_send({command: 'PITCH', data: 0});
			break;
		case 39: // right
			socket_send({command: 'ROLL', data: 0});
			break;
		case 40: // down
			socket_send({command: 'PITCH', data: 0});
			break;
	}
}

let quad; // to hold quad object in order to rotate

// create new WebGL scene and camera
const scene = new THREE.Scene();
const camera = create_camera();
const renderer = create_renderer(600, 400);
document.querySelector('#rendering_container').appendChild(renderer.domElement);
add_drone_to_scene(scene);
add_lights_to_scene(scene);

// render loop - set rotation from server data on each loop
(function render() {
	requestAnimationFrame(render);
	if (quad) {
		quad.rotation.x = attitude.pitch * (Math.PI/180);
		quad.rotation.y = attitude.yaw * (Math.PI/180);
		quad.rotation.z = attitude.roll * (Math.PI/180);
	}
	renderer.render(scene, camera);
})();

function add_drone_to_scene(scene) {
	// load quad with Lambertian material asynchonously from server
	const loader = new THREE.JSONLoader();
	loader.load('drone.json', function(geometry, materials) {
		const material = new THREE.MeshLambertMaterial({color: 0xFF00FF});
		quad = new THREE.Mesh(geometry, material);
		scene.add(quad);
	});
}

function add_lights_to_scene(scene) {
	// create light sources to illuminate Lambertian material
	const light1 = new THREE.PointLight(0xFFFFFF);
	light1.position.set(50, 50, 50);
	scene.add(light1);
	const light2 = new THREE.PointLight(0xFFFFFF);
	light2.position.set(-50, -50, 50);
	scene.add(light2);
}

function create_camera() {
	const camera = new THREE.PerspectiveCamera(75, 1.5, 0.1, 1000);
	// camera distance from origin
	camera.position.z = 15;
	return camera;
}

function create_renderer(width, height) {
	// set canvas size and add to document
	const renderer = new THREE.WebGLRenderer();
	renderer.setSize(width, height);
	return renderer;
}
