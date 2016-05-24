/*jslint browser: true, maxerr: 50, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var i = 0;
    var lastTimestamp = 0;

    function addPixel(x, y, intensity) {
        var rect = document.createElementNS(
            "http://www.w3.org/2000/svg",
            'rect'
        );
        rect.setAttribute('x', x);
        rect.setAttribute('y', y);
        rect.setAttribute('height', 1);
        rect.setAttribute('width', 1);
        var animate = document.createElementNS(
            "http://www.w3.org/2000/svg",
            'animate'
        );
        animate.setAttribute('attributeName', 'fill');
        animate.setAttribute('from', 'white');
        var color = 'rgb(0,' + Math.floor(255 * intensity) + ',0)';
        animate.setAttribute('to', color);
        animate.setAttribute('dur', '1s');
        animate.setAttribute('fill', 'freeze');
        animate.setAttribute('begin', 'DOMNodeInserted');
        rect.appendChild(animate);
        document.querySelector('svg').appendChild(rect);
    }

    function step(timestamp) {
        if (timestamp - lastTimestamp > 10) {
            do {
                addPixel(i % 128, Math.floor(i / 128), 0.5);
                i += 1;
            } while (i % 10 !== 0);
            lastTimestamp = timestamp;
        }
        window.requestAnimationFrame(step);
    }

    window.requestAnimationFrame(step);
});
