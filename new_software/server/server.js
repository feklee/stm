// Serves the game and assets.

/*jslint node: true, es6: true, maxlen: 80 */

"use strict";

var WebSocketServer = require("websocket").server;
var http = require("http");
var microcontroller = require("./microcontroller");
var args = process.argv.slice(2);
var browserConnection = null;
var nodeStatic = require("node-static");
var fileServer = new nodeStatic.Server("./public", {cache: 0});
var onData;
var debug = false;

if (process.platform === "win32") {
    var rl = require("readline").createInterface({
        input: process.stdin,
        output: process.stdout
    });

    rl.on("SIGINT", function () {
        process.emit("SIGINT");
    });
}

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
        type: "tipPositionLog",
        positions: positions
    };
}

function simulateData() {
    mode = "scan";
    setInterval(function () {
        onData(JSON.stringify(simulatedData()));
    }, 20);
}

function onConnectedToMicrocontroller() {
    var port = 8080;
    var server = http.createServer(function (request, response) {
        request.addListener("end", function () {
            fileServer.serve(request, response);
        }).resume();
    }).listen(port);

    server.listen(port, function () {
        console.log("Server is listening on port " + port);
    });

    var wsServer = new WebSocketServer({
        httpServer: server,
        autoAcceptConnections: false
    });

    wsServer.on("request", function (request) {
        browserConnection = request.accept(null, request.origin);
        console.log("Connection from browser accepted");

        browserConnection.on("message", function (message) {
            if (message.type === "utf8") {
                microcontroller.sendJson(message.utf8Data);
            }
        });
    });

    if (debug) {
        simulateData();
    }
}

onData = function (data) {
    if (browserConnection !== null) {
        browserConnection.sendUTF(JSON.stringify(data));
    }
};

if (args.length === 0) {
    microcontroller.listSerialPorts(
        function (ports) {
            console.log("Specify com port name as first argument");
            console.log("");
            console.log("Available ports:");
            ports.forEach(function (port) {
                console.log("");
                console.log("  * " + port.comName);
            });
        }
    );
} else {
    microcontroller.connect({
        comName: args[0],
        onConnected: onConnectedToMicrocontroller,
        onData: onData
    });
}

if (debug) {
    onConnectedToMicrocontroller();
}
