/*jslint browser: true, maxerr: 50, maxlen: 80 */

/*global define, window */

define(['scan-image', 'beam'], function (scanImage, beam) {
    'use strict';

    var client = new window.WebSocket('ws://localhost:8080/', 'echo-protocol');

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
            beam.sideLen = data.sideLen;
            break;
        case 'pixel':
            scanImage.setPixel(data.x, data.y, data.intensity);
            beam.draw(data.x, data.y);
            break;
        case 'finished':
            scanImage.finish();
            break;
        case 'started':
            scanImage.clear();
            beam.clear();
            break;
        }
    };
});
