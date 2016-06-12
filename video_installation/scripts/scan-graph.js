/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(function () {
    'use strict';

    var canvas = document.querySelector('canvas.scan.graph');
    var ctx = canvas.getContext('2d');
    var lastTimestamp = window.performance.now();
    var points = [];
    var pointDrawRate = 0; // points / ms
    var verticalStretchFactor = 1;

    function maxNumberOfVisiblePoints() {
        return Math.ceil(
            canvas.height / verticalStretchFactor
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
        return [100 * x, verticalStretchFactor * y];
    }

    function drawProperty(name) {
        var point;
        var n = numberOfPointsToDraw();
        var i = 0;

        if (n === 0) {
            return;
        }

        ctx.beginPath();
        ctx.moveTo(...canvasPoint(points[0][name], 0));
        while (i < n) {
            point = points[i];
            ctx.lineTo(...canvasPoint(point[name], i));
            i += 1;
        }
        ctx.strokeStyle = 'green';
        ctx.stroke();
    }

    function draw(timestamp) {
        clearCanvas();
        drawProperty(0); // z
        drawProperty(1); // voltage
        scrollUp((timestamp - lastTimestamp) * pointDrawRate);
        lastTimestamp = timestamp;
        window.requestAnimationFrame(draw);
    }

    function appendPoints(newPoints) {
        points.push(...newPoints);
    }

    window.requestAnimationFrame(draw);

    return {
        appendPoints: appendPoints,
        set pointDrawRate(newPointDrawRate) {
            pointDrawRate = newPointDrawRate;
        },
        set verticalStretchFactor(newVerticalStretchFactor) {
            verticalStretchFactor = newVerticalStretchFactor;
        }
    };
});
