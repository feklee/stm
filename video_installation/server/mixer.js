// Mixes the image ("ideal") and the data from the microscope ("real").

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var sideLen = 128;
var browserConnection = null;
var image = require('./image');
var attenuationOfScan = 0;
var faderPosition = 0;
var mixedPixels = [];

function sendIfConnected(data) {
    if (browserConnection === null) {
        return;
    }
    browserConnection.sendUTF(JSON.stringify(data));
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
        type: 'mixedPixels',
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
    set browserConnection(x) {
        browserConnection = x;
        sendSideLen();
    },
    set faderPosition(x) {
        faderPosition = x;
        attenuationOfScan = Math.tan(x * Math.PI / 2);
    },
    onScanPixels: onScanPixels
};
