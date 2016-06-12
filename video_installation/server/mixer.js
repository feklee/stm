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

function visibilityOfScan(scanPixelIntensity) {
    if (faderPosition === 1.0) {
        // because attenuationOfScan does not really go to infinity
        return 0;
    } else {
        return Math.pow(
            2 * Math.abs(scanPixelIntensity - 0.5),
            attenuationOfScan
        );
    }
}

function sendMixedPixels() {
    sendIfConnected({
        type: 'mixedPixels',
        pixels: mixedPixels
    });
}

function onScanPixel(scanPixel) {
    var x = scanPixel[0];
    var y = scanPixel[1];
    var intensity = scanPixel[2];
    var v = visibilityOfScan(intensity);

    mixedPixels.push([
        x,
        y,
        intensity * v + image.intensity(x, y) * (1 - v)
    ]);
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
