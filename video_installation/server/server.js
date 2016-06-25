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
var onData;
var debug = false;
var wstream;
var fs = require('fs');
var dataLogIsEnabled = false;

if (dataLogIsEnabled) {
    wstream = fs.createWriteStream('log/data_' + Date.now() + '.json');
}

if (process.platform === "win32") {
    var rl = require("readline").createInterface({
        input: process.stdin,
        output: process.stdout
    });

    rl.on("SIGINT", function () {
        process.emit("SIGINT");
    });
}

process.on("SIGINT", function () {
    if (dataLogIsEnabled) {
        wstream.end();
    }
    process.exit();
});

var i = 0;
function simulatedData() {
    var j = 100;
    var positions = [];
    var x;
    var y;
    while (j > 0) {
        x = i % 128;
        y = Math.floor(i / 128);
        positions.push([x, y, 0, 3.3 * Math.random()]);
        j -= 1;
        i += 1;
        if (i >= 128 * 128) {
            i = 0;
        }
    }

    return {
        type: 'tipPositionLog',
        positions: positions
    };
}

function simulateData() {
    mode = 'scan';
    setInterval(function () {
        onData(JSON.stringify(simulatedData()));
    }, 20);
}

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

    if (debug) {
        simulateData();
    }
    stm.approachScanRetract();
}

function sendIfConnected(data) {
    if (browserConnection === null) {
        return;
    }
    browserConnection.sendUTF(JSON.stringify(data));
}

function sendAsGraphPoints(positions, graphIndex, index, scale) {
    if (scale === undefined) {
        scale = 1;
    }
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
            position[3] / 3.3 // intensity
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

function interpretNewMode(newMode) {
    mode = newMode;
    console.log("Mode: " + mode);
    var isFinished = mode === 'idle';
    if (isFinished) {
        setTimeout(stm.approachScanRetract, 500);
    }
}

onData = function (data) {
    if (dataLogIsEnabled) {
        data.timestamp = Date.now();
        wstream.write(JSON.stringify(data));
    }
    switch (data.type) {
    case 'tipPositionLog':
        interpretPositions(data.positions);
        break;
    case 'scanDuration':
        console.log('Scan duration: ' + data.value / 1000000 + ' s');
        break;
    case 'faderPosition':
        mixer.faderPosition = data.value;
        break;
    case 'newMode':
        interpretNewMode(data.value);
        break;
    case 'peakCoarseApproachSignal':
        console.log('Peak coarse approach signal: ' + data.value + ' V');
        break;
    case 'peakFineApproachSignal':
        console.log('Peak fine approach signal: ' + data.value + ' V');
        break;
    case 'error':
        console.log("STM error: " + data.value);
        break;
    }
};

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

if (debug) {
    onConnectedToStm();
}
