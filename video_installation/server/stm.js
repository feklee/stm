// Interfaces with the STM, the scanning tunneling microscope.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var serialport = require('serialport');
var SerialPort = serialport.SerialPort;
var port;

function listSerialPorts(callback) {
    serialport.list(function (ignore, ports) {
        callback(ports);
    });
}

function connect(settings) {
    port = new SerialPort(settings.comName, {
        baudRate: 115200,
        parser: serialport.parsers.readline('\n')
    });
    port.on('open', function () {
        console.log('Serial port opened');
        settings.onConnected();
    });
    port.on('close', function () {
        console.log('Serial port closed');
    });
    port.on('error', function () {
        console.log('Serial port error');
    });
    port.on('disconnected', function () {
        console.log('Serial port disconnected');
    });
    port.on('data', function (json) {
        try {
            var data = JSON.parse(json);
            settings.onData(data);
        } catch (e) {
            if (e.name === 'SyntaxError') {
                console.log('Data is not valid JSON: ' + json);
            }
        }
    });
}

function approachScanRetract() {
    port.write(JSON.stringify([{
        mode: 'approach',
        targetCurrentSignal: 1.5
    }, {
        mode: 'scan',
        sideLen: 128,
        maxAdjustmentSteps: 0,
        adjustmentDelta: 1000,
        targetCurrentSignal: 1.5 // V
    }, {
        mode: 'retract',
        targetCurrentSignal: 0.1
    }]));
}

module.exports = {
    listSerialPorts: listSerialPorts,
    connect: connect,
    approachScanRetract: approachScanRetract
};
