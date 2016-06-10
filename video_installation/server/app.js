// Serves the game and assets.

/*jslint node: true, maxlen: 80 */

'use strict';

var WebSocketServer = require('websocket').server;
var http = require('http');
var mixer = require('./mixer');
var stm = require('./stm');
var args = process.argv.slice(2);

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
        var browserConnection = request.accept(null, request.origin);
        console.log((new Date()) + ' Connection from browser accepted.');
        mixer.browserConnection = browserConnection;
    });

    setInterval(stm.startScan, 10000);
}

function interpretScanData(data) {
    var scanPixels = data.map(function (datum) {
        return {x: datum[0], y: datum[1], intensity: datum[2] / 0xffff};
    });
    mixer.onScanPixels(scanPixels);
}

function onData(data) {
    switch (data.type) {
    case 'scanData':
        interpretScanData(data.data);
        break;
    case 'faderUpdate':
        mixer.faderPosition = data.position;
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
