// Serves the game and assets.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var WebSocketServer = require('websocket').server;
var http = require('http');
var mixer = require('./mixer');
var stm = require('./stm');
var args = process.argv.slice(2);
var browserConnection = null;
var nodeStatic = require('node-static');
var fileServer = new nodeStatic.Server('./public', {cache: 0});
var mode = '';

function onConnectedToStm() {
    var port = 8080;
    var server = http.createServer(function (request, response) {
        request.addListener('end', function () {
            fileServer.serve(request, response);
        }).resume();
    }).listen(port);

    server.listen(port, function () {
        console.log('Server is listening on port ' + port);
    });

    var wsServer = new WebSocketServer({
        httpServer: server,
        autoAcceptConnections: false
    });

    wsServer.on('request', function (request) {
        browserConnection = request.accept(null, request.origin);
        console.log('Connection from browser accepted');
        mixer.browserConnection = browserConnection;
    });

    setInterval(stm.approachScanRetract, 20000);
}

function sendIfConnected(data) {
    if (browserConnection === null) {
        return;
    }
    browserConnection.sendUTF(JSON.stringify(data));
}

function sendAsGraphPoints(positions, graphIndex, index, scale = 1) {
    var points = positions.map(function (tipPosition) {
        return scale * tipPosition[index];
    });
    sendIfConnected({
        type: 'graphPoints',
        index: graphIndex,
        points: points
    });
}

function interpretScanPositions(positions) {
    var scanPixels = positions.map(function (position) {
        return [
            position[0], // x
            position[1], // y
            position[2] / 0xffff // intensity
        ];
    });
    mixer.onScanPixels(scanPixels);
    sendAsGraphPoints(positions, 0, 2, 1 / 0xffff); // z
    sendAsGraphPoints(positions, 1, 3, 1 / 3.3); // current signal
}

function interpretApproachRetractPositions(positions) {
    sendAsGraphPoints(positions, 2, 2, 1 / 0xffff); // z
    sendAsGraphPoints(positions, 3, 3, 1 / 3.3); // current signal
}

function interpretPositions(positions) {
    switch (mode) {
    case 'scan':
        interpretScanPositions(positions);
        break;
    case 'approach':
        interpretApproachRetractPositions(positions);
        break;
    case 'retract':
        interpretApproachRetractPositions(positions);
        break;
    }
}

function onData(data) {
    switch (data.type) {
    case 'tipPositionLog':
        interpretPositions(data.positions);
        break;
    case 'scanDuration':
        console.log('Scan duration: ' + data.value);
        break;
    case 'faderPosition':
        mixer.faderPosition = data.value;
        break;
    case 'newMode':
        mode = data.value;
        console.log("New mode: " + mode);
        break;
    case 'error':
        console.log("STM error: " + data.message);
        break;
    }
}

if (args.length === 0) {
    stm.listSerialPorts(
        function (ports) {
            console.log('Specify com port name as first argument');
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
