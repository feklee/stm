/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(function () {
    "use strict";

    return function (spec) {
        var width = d3.select("svg").node().getBoundingClientRect().width;
        var height = d3.select("svg").node().getBoundingClientRect().height;
        var positions = [];

        var xScale;
        var yScale = d3.scaleLinear()
            .domain([-0x1000, 0xffff + 0x1000])
            .range([height, 0]);

        var svg = d3.select("body section." + spec.modeName + " svg");

        var graphGroup = svg.append("g");

        var onZoom = function () {
            var transform = d3.event.transform;
            var transformString = "translate(" + transform.x + "," +
                    "0) scale(" + transform.k + ",1)";
            graphGroup.attr("transform", transformString);
        };

        var zoomBehavior = d3.zoom()
            .scaleExtent([1, 10])
            .translateExtent([[0, 0], [width - 1, height - 1]])
            .on("zoom", onZoom);

        svg.call(zoomBehavior);

        var zLine = d3.line()
            .x(function (ignore, i) {
                return xScale(i);
            })
            .y(function (d) {
                return yScale(d[2]);
            });

        var currentSignalLine = d3.line()
            .x(function (ignore, i) {
                return xScale(i);
            })
            .y(function (d) {
                return yScale(d[3]);
            });

        var updateScaling = function () {
            width = d3.select("svg").node().getBoundingClientRect().width;
            zoomBehavior.translateExtent([[0, 0], [width - 1, height - 1]]);
            xScale = d3.scaleLinear()
                .domain([0, positions.length - 1])
                .range([0, width]);
        };

        var render = function () {
            updateScaling();
            graphGroup.selectAll("path").remove();
            graphGroup.append("path")
                .datum(positions)
                .attr("class", "z")
                .attr("d", zLine);
            graphGroup.append("path")
                .datum(positions)
                .attr("class", "current-signal")
                .attr("d", currentSignalLine);
        };

        var appendPositions = function (newPositions) {
            positions.push(...newPositions);
            if (spec.maxLength !== undefined) {
                while (positions.length > spec.maxLength) {
                    positions.shift();
                }
            }
            render();
        };

        var clear = function () {
            positions = [];
        };

        var iframeWindowEl = d3.select(d3.select("main>iframe").node()
            .contentWindow);
        iframeWindowEl.on("resize." + spec.modeName, render);

        return {
            clear: clear,
            appendPositions: appendPositions
        };
    };
});
