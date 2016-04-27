/*jslint node: true, maxlen: 80 */

'use strict';

var serialPort = require('serialport');
serialPort.list(function (ignore, ports) {
    ports.forEach(function (port) {
        console.log(port.comName);
        console.log(port.pnpId);
        console.log(port.manufacturer);
    });
});

var express = require('express');
var app = express();

app.get('/', function (ignore, res) {
    res.send('Hello World!');
});

app.listen(3000, function () {
    console.log('Example app listening on port 3000!');
});

app.use(express.static('public'));
