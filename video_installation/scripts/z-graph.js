/*jslint browser: true, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var canvas = document.querySelector('canvas.z-graph');
    var ctx = canvas.getContext('2d');
    var offset = 2; // px

    function draw() {
        ctx.fillStyle = 'rgba(0, 255, 0)';
        ctx.fillRect(offset, offset,
                canvas.width - 2 * offset,
                canvas.height - 2 * offset);
        window.requestAnimationFrame(fade);
    }

    draw();

    return {
        appendPositions: function () {}
    };
});
