// Serves the game and assets.

/*jslint node: true, maxlen: 80 */

'use strict';

var WebSocketServer = require('websocket').server;
var http = require('http');
var simulator = require('./simulator');
var mixer = require('./mixer');

var server = http.createServer(function (request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});

server.listen(8080, function () {
    console.log((new Date()) + ' Server is listening on port 8080');
});

var wsServer = new WebSocketServer({
    httpServer: server,
    autoAcceptConnections: false
});

simulator.startScan(mixer.onScanPixel);

wsServer.on('request', function (request) {
    var connection = request.accept(null, request.origin);
    console.log((new Date()) + ' Connection accepted.');

    mixer.connection = connection;
});
