// Interfaces with the STM, the scanning tunneling microscope.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var serialPort = require('serialport');
var onOutput;

function listSerialPorts(callback) {
    serialPort.list(function (ignore, ports) {
        callback(ports);
    });
}

module.exports = {
    listSerialPorts: listSerialPorts,
    set onOutput(newOnOutput) {
        onOutput = newOnOutput;
    }
};
