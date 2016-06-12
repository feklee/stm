/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    function constructor(spec) {
        var canvas = document.querySelector(
            'canvas.graph.index-' + spec.index
        );
        var ctx = canvas.getContext('2d');
        var lastTimestamp = window.performance.now();
        var points = [];

        function maxNumberOfVisiblePoints() {
            return Math.ceil(
                canvas.height / spec.verticalStretchFactor
            );
        }

        function scrollUp(n) {
            var minLength = maxNumberOfVisiblePoints();
            while (n > 0 && points.length > minLength) {
                points.shift();
                n -= 1;
            }
        }

        function clearCanvas() {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
        }

        function numberOfPointsToDraw() {
            return Math.max(Math.min(
                maxNumberOfVisiblePoints(),
                points.length
            ), 0);
        }

        function canvasPoint(x, y) {
            return [
                100 * (x - 0.5) + canvas.width / 2,
                spec.verticalStretchFactor * y
            ];
        }

        function draw() {
            var point;
            var n = numberOfPointsToDraw();
            var i = 0;

            if (n === 0) {
                return;
            }

            ctx.beginPath();
            ctx.moveTo(...canvasPoint(points[0], 0));
            while (i < n) {
                point = points[i];
                ctx.lineTo(...canvasPoint(point, i));
                i += 1;
            }
            ctx.strokeStyle = 'green';
            ctx.stroke();
        }

        function animationFrame(timestamp) {
            clearCanvas();
            draw();
            scrollUp((timestamp - lastTimestamp) * spec.pointDrawRate);
            lastTimestamp = timestamp;
            window.requestAnimationFrame(animationFrame);
        }

        function appendPoints(newPoints) {
            points.push(...newPoints);
        }

        window.requestAnimationFrame(animationFrame);

        return {
            appendPoints: appendPoints
        };
    }

    return constructor;
});
