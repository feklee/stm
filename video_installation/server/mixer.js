// Mixes the image and the data from the microscope.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var sideLen = 128;
var connection = null;
var image = require('./image');
var attenuationOfScan = 2;
var faderPosition;
var mixedPixels = [];

function sendIfConnected(data) {
    if (connection === null) {
        return;
    }
    connection.sendUTF(JSON.stringify(data));
}

function sendSideLen() {
    sendIfConnected({
        type: 'sideLen',
        sideLen: sideLen
    });
}

function visibilityOfScan(scanPixel) {
    if (faderPosition === 1.0) {
        // because attenuationOfScan does not really go to infinity
        return 0;
    } else {
        return Math.pow(2 * Math.abs(scanPixel.intensity - 0.5),
                        attenuationOfScan);
    }
}

function sendMixedPixels() {
    sendIfConnected({
        type: 'pixels',
        pixels: mixedPixels
    });
}

function onScanPixel(scanPixel) {
    var x = scanPixel.x;
    var y = scanPixel.y;
    var v = visibilityOfScan(scanPixel);

    mixedPixels.push({
        x: x,
        y: y,
        intensity: scanPixel.intensity * v + image.intensity(x, y) * (1 - v)
    });
}

function onScanPixels(scanPixels) {
    mixedPixels = [];
    scanPixels.forEach(onScanPixel);
    sendMixedPixels();
}

module.exports = {
    set connection(x) {
        connection = x;
        sendSideLen();
    },
    set faderPosition(x) {
        faderPosition = x;
        attenuationOfScan = Math.tan(x * Math.PI / 2);
    },
    onScanPixels: onScanPixels
};
