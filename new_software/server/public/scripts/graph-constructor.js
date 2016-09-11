/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(function () {
    "use strict";

    return function (spec) {
        var boundingBox = d3.select("svg").node().getBoundingClientRect();
        var positions = [];
        var padding = 50; // px

        if (spec.maxLength === undefined) {
            spec.maxLength = 100; // todo: remove
        }

        var xScale = d3.scaleLinear()
            .domain([0, spec.maxLength - 1])
            .range([0, boundingBox.width - 2 * padding]);
        var yScale = d3.scaleLinear()
            .domain([-0x1000, 0xffff + 0x1000])
            .range([boundingBox.height - 2 * padding, 0]);

        var svg = d3.select("body section." + spec.modeName + " svg");

        var xAxis = d3.axisBottom(xScale);
        var yAxis = d3.axisLeft(yScale).ticks(5);

        var yAxisGroup = svg
            .append("g")
            .attr("transform",
                    "translate(" + (padding - 5 - 1) + "," + padding + ")");
        yAxisGroup.call(yAxis);

        var xAxisGroup = svg
            .append("g")
            .attr("transform",
                    "translate(" + padding + "," +
                    (boundingBox.height - padding + 5) + ")");
        xAxisGroup.call(xAxis);

        var mainGroup = svg
            .append("g")
            .attr("transform", "translate(" + padding + "," + padding + ")")
            .attr("class", "main");

        var backgroundRect = mainGroup
            .append("rect")
            .attr("width", boundingBox.width - 2 * padding)
            .attr("height", boundingBox.height - 2 * padding);

        var onZoom = function () {
            var transform = d3.event.transform;
            var transformString = "translate(" +
                    (padding + transform.x) + "," +
                    "0) scale(" + transform.k + ",1)";
            mainGroup.attr("transform", transformString);
            updateScaling();
        };

        var zoomBehavior = d3.zoom()
            .scaleExtent([1, 10])
            .translateExtent([
                [padding - 1, padding - 1],
                [boundingBox.width - 1 - padding,
                        boundingBox.height - 1 - padding]
            ])
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
            boundingBox = d3.select("svg").node().getBoundingClientRect();
            zoomBehavior.translateExtent([
                [0, 0],
                [boundingBox.width - 1, boundingBox.height - 1]
            ]);
            backgroundRect.attr("width", boundingBox.width - 2 * padding);

            xScale
                .domain([0, spec.maxLength - 1])
                .range([0, boundingBox.width - 2 * padding]);
            xAxisGroup.call(xAxis);
        };

        var render = function () {
            updateScaling();
            mainGroup.selectAll("path").remove();
            mainGroup.append("path")
                .datum(positions)
                .attr("class", "z")
                .attr("d", zLine);
            mainGroup.append("path")
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
