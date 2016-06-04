// Simulates scanning.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var i = 0;
var sideLen = 32;
var disturbanceInterval = [0, 0];
var connection = null;
var startScan;

function sendIfConnected(data) {
    if (connection === null) {
        return;
    }
    connection.sendUTF(JSON.stringify(data));
}

function isInDisturbanceInterval() {
    return (i >= disturbanceInterval[0] && i < disturbanceInterval[1]);
}

function isPastDisturbanceInterval() {
    return i >= disturbanceInterval[1];
}

function createNextDisturbanceInterval() {
    disturbanceInterval[0] = i + Math.random() * 200;
    disturbanceInterval[1] = disturbanceInterval[0] + Math.random() * 50;
}

function isDisturbed() {
    if (isInDisturbanceInterval()) {
        return true;
    } else {
        if (isPastDisturbanceInterval()) {
            createNextDisturbanceInterval();
        }
        return false;
    }
}

function disturbance() {
    return 0.25 * Math.random();
}

function undisturbedIntensity(x, y) {
    return (x * x + y * y) / 100 % 1;
}

function intensity(x, y) {
    return isDisturbed()
        ? disturbance()
        : undisturbedIntensity(x, y);
}

function finishScan() {
    sendIfConnected({
        type: 'finished'
    });
    setTimeout(startScan, 5000);
}

function isLastPixel(x, y) {
    return x === sideLen - 1 && y === sideLen - 1;
}

function sendPixel(x, y) {
    sendIfConnected({
        type: 'pixel',
        x: x,
        y: y,
        intensity: intensity(x, y)
    });
}

function scanStep() {
    var x = i % sideLen;
    var y = Math.floor(i / sideLen);
    sendPixel(x, y);
    if (isLastPixel(x, y)) {
        finishScan();
    } else {
        i += 1;
        setTimeout(scanStep, 1);
    }
}

startScan = function () {
    sendIfConnected({
        type: 'started'
    });
    i = 0;
    scanStep();
};

function sendSideLen() {
    sendIfConnected({
        type: 'sideLen',
        sideLen: sideLen
    });
}

startScan();

module.exports = {
    set connection(newConnection) {
        connection = newConnection;
        sendSideLen();
    }
};
