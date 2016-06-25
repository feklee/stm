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
        var data;
        try {
            data = JSON.parse(json);
        } catch (e) {
            if (e.name === 'SyntaxError') {
                console.log('Data is not valid JSON: ' + json);
                return;
            }
        }
        settings.onData(data);
    });
}

function approachScanRetract() {
    port.write(JSON.stringify([{
        mode: 'approach',
        targetSignal: 1.0, // V
        coarsePiezoStepSize: 100,
        finePiezoStepSize: 10
    }, {
        mode: 'scan',
        sideLen: 128,
        maxAdjustmentSteps: 0,
        adjustmentDelta: 1000,
        targetSignal: 1.0 // V
    }, {
        mode: 'retract',
        targetSignal: 0.1 // V
    }]));
}

module.exports = {
    listSerialPorts: listSerialPorts,
    connect: connect,
    approachScanRetract: approachScanRetract
};
