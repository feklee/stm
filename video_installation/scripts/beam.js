/*jslint browser: true, maxerr: 50, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var canvas = document.querySelector('canvas.beam');
    var ctx = canvas.getContext('2d');
    var offset = 2; // px

    function draw(x, y) {
        ctx.globalCompositeOperation = 'source-over';
        ctx.fillStyle = 'white';
        ctx.fillRect(offset + x, offset + y, 1, 1);
    }

    function clear() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
    }

    function fade() { // fixme: use time
        ctx.globalCompositeOperation = 'destination-out';
        ctx.fillStyle = 'rgba(0, 0, 0, 0.2)';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        window.requestAnimationFrame(fade);
    }

    draw(10, 10);

    fade();

    return {
        draw: draw,
        set sideLen(newSideLen) {
            canvas.setAttribute('width', newSideLen + 2 * offset);
            canvas.setAttribute('height', newSideLen + 2 * offset);
            document.body.appendChild(canvas);
        },
        clear: clear
    };
});
