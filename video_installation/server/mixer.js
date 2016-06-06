// Simulates scanning.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var sideLen = 128;
var connection = null;
var image = require('./image');
var attenuationOfScan = 2;
var mixedPixels = [];

function sendIfConnected(data) {
    if (connection === null) {
        return;
    }
    connection.sendUTF(JSON.stringify(data));
}

function isFirstPixel(x, y) {
    return x === 0 && y === 0;
}

function isLastPixel(x, y) {
    return x === sideLen - 1 && y === sideLen - 1;
}

function sendSideLen() {
    sendIfConnected({
        type: 'sideLen',
        sideLen: sideLen
    });
}

function visibilityOfScan(scanPixel) {
    return Math.pow(2 * Math.abs(scanPixel.intensity - 0.5),
            attenuationOfScan);
}

function sendMixedPixels() {
    sendIfConnected({
        type: 'pixels',
        pixels: mixedPixels
    });
}

function appendMixedPixel(mixedPixel) {
    const chunkSize = 10;
    mixedPixels.push(mixedPixel);
    if (mixedPixels.length >= chunkSize) {
        sendMixedPixels();
        mixedPixels = [];
    }
}

function onScanPixel(scanPixel) {
    var x = scanPixel.x;
    var y = scanPixel.y;
    var v = visibilityOfScan(scanPixel);

    appendMixedPixel({
        x: x,
        y: y,
        intensity: scanPixel.intensity * v + image.intensity(x, y) * (1 - v)
    });

    if (isFirstPixel(x, y)) {
        sendIfConnected({
            type: 'started'
        });
    }

    if (isLastPixel(x, y)) {
        sendMixedPixels();
        sendIfConnected({
            type: 'finished'
        });
    }
}

module.exports = {
    set connection(newConnection) {
        connection = newConnection;
        sendSideLen();
    },
    onScanPixel: onScanPixel
};
