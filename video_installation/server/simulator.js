// Simulates scanning.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var i = 0;
var sideLen = 128;
var startScan;
var onScanPixel;
var intensity = 0.5;

function updateIntensity() {
    intensity = Math.max(0, Math.min(1, intensity + 0.1 *
            (Math.random() - 0.5)));
}

function isLastPixel(x, y) {
    return x === sideLen - 1 && y === sideLen - 1;
}

function finishScan() {
    setTimeout(startScan, 5000);
}

function scanStep() {
    var x = i % sideLen;
    var y = Math.floor(i / sideLen);
    onScanPixel({
        x: x,
        y: y,
        intensity: intensity
    });
    if (isLastPixel(x, y)) {
        finishScan();
        return;
    }
    updateIntensity();
    i += 1;
    setTimeout(scanStep, 1);
}

startScan = function (newOnScanPixel) {
    if (newOnScanPixel) {
        onScanPixel = newOnScanPixel;
    }
    i = 0;
    intensity = 0.5;
    scanStep();
};

module.exports = {
    startScan: startScan
};
