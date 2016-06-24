/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define([
    'image',
    'graph-constructor'
], function (image, graphConstructor) {
    'use strict';

    var hostname = window.location.hostname;
    var client = new window.WebSocket('ws://' + hostname + ':8080/');
    var drawRate = 5; // items / ms
    var graphIndexes = [0, 1, 2, 3];

    var graphs = graphIndexes.map(function (index) {
        return graphConstructor({
            index: index,
            pointDrawRate: drawRate, // points / ms
            verticalStretchFactor: 3
        });
    });

    image.pixelDrawRate = drawRate;

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
            image.sideLen = data.sideLen;
            break;
        case 'mixedPixels':
            image.appendPixels(data.pixels);
            break;
        case 'graphPoints':
            graphs[data.index].appendPoints(data.points);
            break;
        }
    };
});
