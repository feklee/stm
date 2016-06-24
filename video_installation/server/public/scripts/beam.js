/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var canvas = document.querySelector('canvas.scan.beam');
    var ctx = canvas.getContext('2d');
    var offset = 0; // px
    var timestampAtPreviousFrame = window.performance.now();
    var sideLen = 0;
    var pixelSize = 1; // px

    function resize() {
        var minimumOffset = 16; // px
        var availableWidth = 1080 - 2 * minimumOffset; // px
        pixelSize = Math.floor(availableWidth / sideLen);
        var actualWidth = sideLen * pixelSize;
        offset = Math.floor((1080 - actualWidth) / 2);
        canvas.setAttribute('width', actualWidth + 2 * offset);
        canvas.setAttribute('height', actualWidth + 2 * offset);
    }

    function intensityString(alpha) {
        return 'rgba(255,255,255,' + alpha + ')';
    }

    function gradient(pixel) {
        var x = Math.round(offset + pixel[0] * pixelSize + pixelSize / 2);
        var y = Math.round(offset + pixel[1] * pixelSize + pixelSize / 2);
        var g = ctx.createRadialGradient(
            x,
            y,
            0.5 * pixelSize,
            x,
            y,
            1.5 * pixelSize
        );
        g.addColorStop(0, intensityString(1));
        g.addColorStop(1, intensityString(0));
        return g;
    }

    function draw(pixel) {
        ctx.globalCompositeOperation = 'source-over';
        ctx.fillStyle = gradient(pixel);
        ctx.fillRect(
            offset + pixel[0] * pixelSize - pixelSize,
            offset + pixel[1] * pixelSize - pixelSize,
            3 * pixelSize,
            3 * pixelSize
        );
    }

    function clear() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
    }

    function fade(timestamp) {
        var alpha = (timestamp - timestampAtPreviousFrame) / 100;
        ctx.globalCompositeOperation = 'destination-out';
        ctx.fillStyle = 'rgba(0, 0, 0, ' + alpha + ')';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        timestampAtPreviousFrame = timestamp;
        window.requestAnimationFrame(fade);
    }

    window.requestAnimationFrame(fade);

    return {
        draw: draw,
        set sideLen(newSideLen) {
            sideLen = newSideLen;
            resize();
        },
        clear: clear
    };
});
