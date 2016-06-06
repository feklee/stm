// Simulates scanning.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var i = 0;
var sideLen = 128;
var startScan;
var onScanPixels;
var intensity = 0.5;

function updateIntensity() {
    intensity = Math.max(0, Math.min(1, intensity + 0.1 *
            (Math.random() - 0.5)));
}

function isLastPixel(pixel) {
    return pixel.x === sideLen - 1 && pixel.y === sideLen - 1;
}

function finishScan() {
    setTimeout(startScan, 5000);
}

function scanStep() {
    const chunkSize = 10;
    var x;
    var y;
    var scanPixels = [];
    do {
        x = i % sideLen;
        y = Math.floor(i / sideLen);
        scanPixels.push({
            x: x,
            y: y,
            intensity: intensity
        });
        if (isLastPixel({x: x, y: y})) {
            onScanPixels(scanPixels);
            finishScan();
            return;
        }
        updateIntensity();
        i += 1;
    } while (i % chunkSize !== 0);
    onScanPixels(scanPixels);
    setTimeout(scanStep, 1);
}

startScan = function (newOnScanPixels) {
    if (newOnScanPixels) {
        onScanPixels = newOnScanPixels;
    }
    i = 0;
    intensity = 0.5;
    scanStep();
};

module.exports = {
    startScan: startScan
};
