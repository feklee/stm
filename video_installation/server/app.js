// Serves the game and assets.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var WebSocketServer = require('websocket').server;
var http = require('http');
var mixer = require('./mixer');
var stm = require('./stm');
var args = process.argv.slice(2);
var browserConnection = null;

function onConnectedToStm() {
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

    wsServer.on('request', function (request) {
        browserConnection = request.accept(null, request.origin);
        console.log((new Date()) + ' Connection from browser accepted.');
        mixer.browserConnection = browserConnection;
    });

    setInterval(stm.startScan, 10000);
}

function sendIfConnected(data) {
    if (browserConnection === null) {
        return;
    }
    browserConnection.sendUTF(JSON.stringify(data));
}

function sendAsGraphPoints(positions, graphIndex, index, scale = 1) {
    var points = positions.map(function (position) {
        return scale * position[index];
    });
    sendIfConnected({
        type: 'graphPoints',
        index: graphIndex,
        points: points
    });
}

function interpretPositionLog(positions) {
    var scanPixels = positions.map(function (position) {
        return [
            position[0], // x
            position[1], // y
            position[2] / 0xffff // intensity
        ];
    });
    mixer.onScanPixels(scanPixels);
    sendAsGraphPoints(positions, 0, 2, 1 / 0xffff); // z
    sendAsGraphPoints(positions, 1, 3, 1 / 3.3); // signal
}

function onData(data) {
    switch (data.type) {
    case 'positionLog':
        interpretPositionLog(data.positions);
        break;
    case 'scanDuration':
        console.log('Scan duration: ' + data.value);
        break;
    case 'faderPosition':
        mixer.faderPosition = data.value;
        break;
    case 'error':
        console.log("STM error: " + data.message);
        break;
    }
}

if (args.length === 0) {
    stm.listSerialPorts(
        function (ports) {
            console.log('Specify com port name as first argument.');
            console.log('');
            console.log('Available ports:');
            ports.forEach(function (port) {
                console.log('');
                console.log('  * ' + port.comName);
            });
        }
    );
} else {
    stm.connect({
        comName: args[0],
        onConnected: onConnectedToStm,
        onData: onData
    });
}
