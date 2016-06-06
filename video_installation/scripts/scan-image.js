/*jslint browser: true, maxerr: 50, maxlen: 80 */

/*global define, window */

define(['beam'], function (beam) {
    'use strict';

    var canvas = document.querySelector('canvas.scan-image');
    var ctx = canvas.getContext('2d');
    var offset = 2; // px

    function intensityString(intensity) {
        return 'rgb(0,' + Math.floor(255 * intensity) + ',0)';
    }

    function finish() {
        canvas.className = 'finished';
    }

    function drawPixel(pixel) {
        ctx.fillStyle = intensityString(pixel.intensity);
        ctx.fillRect(offset + pixel.x, offset + pixel.y, 1, 1);
        beam.draw(pixel.x, pixel.y);
        canvas.className = 'scanning';
    }

    function drawPixels(pixels) {
        pixels.forEach(function (pixel) {
            drawPixel(pixel);
        });
    }

    function clear() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        beam.clear();
    }

    return {
        drawPixels: drawPixels,
        finish: finish,
        set sideLen(newSideLen) {
            canvas.setAttribute('width', newSideLen + 2 * offset);
            canvas.setAttribute('height', newSideLen + 2 * offset);
            beam.sideLen = newSideLen;
        },
        clear: clear
    };
});
