/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(['scan-image', 'scan-graph'], function (scanImage, scanGraph) {
    'use strict';

    var client = new window.WebSocket('ws://localhost:8080/');
    var drawRate = 20; // items / ms

    scanImage.pixelDrawRate = drawRate;
    scanGraph.pointDrawRate = drawRate;
    scanGraph.verticalStretchFactor = 3;

    client.onerror = function () {
        window.console.log('Connection error');
    };

    client.onopen = function () {
        window.console.log('Client connected');
    };

    client.onclose = function () {
        window.console.log('Client closed');
    };

    client.onmessage = function (e) {
        var data;
        if (typeof e.data === 'string') {
            data = JSON.parse(e.data);
        } else {
            return;
        }

        switch (data.type) {
        case 'sideLen':
            scanImage.sideLen = data.sideLen;
            break;
        case 'mixedPixels':
            scanImage.appendPixels(data.pixels);
            break;
        case 'graphPoints':
            scanGraph.appendPoints(data.points);
            break;
        }
    };
});
