/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var canvas = document.querySelector('canvas.scan-graph');
    var ctx = canvas.getContext('2d');
    var offset = 2; // px

    function draw() {
        ctx.fillStyle = 'rgb(0, ' + Math.round(255 * Math.random()) + ', 0)';
        ctx.fillRect(offset, offset,
                canvas.width - 2 * offset,
                canvas.height - 2 * offset);
        window.requestAnimationFrame(draw);
    }

    canvas.setAttribute('width', 100);
    canvas.setAttribute('height', 900);

    draw();

    return {
        appendPoints: function (points) {
        }
    };
});
