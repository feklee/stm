/*jslint browser: true, maxerr: 50, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var canvas = document.querySelector('canvas');
    var ctx = canvas.getContext('2d');
    var offset = 2; // px

    function intensityString(intensity) {
        return 'rgb(0,' + Math.floor(255 * intensity) + ',0)';
    }

    function finish() {
        canvas.className = 'finished';
    }

    function setPixel(x, y, intensity) {
        ctx.fillStyle = intensityString(intensity);
        ctx.fillRect(offset + x, offset + y, 1, 1);
        canvas.className = 'scanning';
    }

    function clear() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
    }

    return {
        setPixel: setPixel,
        finish: finish,
        set sideLen(newSideLen) {
            canvas.setAttribute('width', newSideLen + 2 * offset);
            canvas.setAttribute('height', newSideLen + 2 * offset);
            document.body.appendChild(canvas);
        },
        clear: clear
    };
});
