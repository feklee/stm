/*jslint browser: true, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var canvas = document.querySelector('canvas.scan-image');
    var ctx = canvas.getContext('2d');
    var offset = 2; // px
    var sideLen = 0;

    function intensityString(intensity) {
        return 'rgb(0,' + Math.floor(255 * intensity) + ',0)';
    }

    function finish() {
        canvas.classList.add('finished');
    }

    function isLastPixel(pixel) {
        return pixel.x === sideLen - 1 && pixel.y === sideLen - 1;
    }

    function isFirstPixel(pixel) {
        return pixel.x === 0 && pixel.y === 0;
    }

    function clear() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        beam.clear();
    }

    function drawPixel(pixel) {
        canvas.classList.remove('finished');
        if (isLastPixel(pixel)) {
            finish();
        }
        if (isFirstPixel(pixel)) {
            clear();
        }
        ctx.fillStyle = intensityString(pixel.intensity);
        ctx.fillRect(offset + pixel.x, offset + pixel.y, 1, 1);
        beam.draw(pixel.x, pixel.y);
    }

    function drawPixels(pixels) {
        pixels.forEach(function (pixel) {
            drawPixel(pixel);
        });
    }

    return {
        drawPixels: drawPixels,
        set sideLen(newSideLen) {
            sideLen = newSideLen;
            canvas.setAttribute('width', sideLen + 2 * offset);
            canvas.setAttribute('height', sideLen + 2 * offset);
            beam.sideLen = sideLen;
        },
        clear: clear
    };
});
