// Interface to microcontroller, for measuring and controlling the hardware

/*jslint node: true, es6: true, maxlen: 80 */

"use strict";

var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var log = require("./log");
var port;

function listSerialPorts(callback) {
    serialport.list(function (ignore, ports) {
        callback(ports);
    });
}

function connect(settings) {
    port = new SerialPort(settings.comName, {
        baudRate: 115200,
        parser: serialport.parsers.readline("\n")
    });
    port.on("open", function () {
        log.appendInfo("Serial port opened");
        settings.onConnected();
    });
    port.on("close", function () {
        log.appendWarn("Serial port closed");
    });
    port.on("error", function () {
        log.appendError("Serial port error");
    });
    port.on("disconnected", function () {
        log.appendWarn("Serial port disconnected");
    });
    port.on("data", function (json) {
        var data;
        try {
            data = JSON.parse(json);
        } catch (e) {
            if (e.name === "SyntaxError") {
                log.appendError("Non JSON data from microcontroller");
                console.log(json);
                return;
            }
        }
        settings.onData(data);
    });
}

function sendJson(modeChainJson) {
    port.write(modeChainJson);
}

module.exports = {
    listSerialPorts: listSerialPorts,
    connect: connect,
    sendJson: sendJson
};
