/*jslint node: true, maxlen: 80 */

'use strict';

var serialPort = require("serialport").SerialPort;

serialPort.list(function (ignore, ports) {
    ports.forEach(function (port) {
        console.log(port.comName);
        console.log(port.pnpId);
        console.log(port.manufacturer);
    });
});
