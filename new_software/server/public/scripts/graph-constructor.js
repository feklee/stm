/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(["util"], function (util) {
    "use strict";

    return function (spec) {
        var positions = [
            [0, 0, 0, 6649], [0, 0, 0, 13115], [0, 0, 0, -21058],
            [0, 0, 0, 16754], [0, 0, 0, 15260], [0, 0, 0, 18709],
            [0, 0, 0, 24235]
        ]; // todo: reset
        var margin = {
            top: 0, // px
            left: 30, // px
            bottom: 30, // px
            right: 50 // px
        };
        var svg = d3.select("body section." + spec.modeName + " svg");
        var boundingBox = function () {
            return svg.node().getBoundingClientRect();
        };
        var mainWidth = function () {
            return boundingBox().width - margin.left - margin.right;
        };
        var mainHeight = function () {
            return boundingBox().height - margin.top - margin.bottom;
        };
        var axisPadding = 1; // px
        var stretchFactor = 1;

        var mainGroup = svg
            .select("g.main")
            .attr("transform",
                    "translate(" + margin.left + "," + margin.top + ")")
            .attr("class", "main");

        var zoomableGroup = mainGroup.select("g.zoomable");

        var addAxisGroup = function (spec) {
            var group = svg
                .select("g." + spec.class + ".axis")
                .attr("transform",
                        "translate(" + spec.left + "," + spec.top + ")");
            group.call(spec.axis);
            return group;
        };

        var xScale = d3.scaleLinear()
            .range([0, mainWidth()]);
        var xAxis = d3.axisBottom(xScale).ticks(5);
        var xAxisGroup = addAxisGroup({
            "class": "x",
            top: margin.top + mainHeight() + axisPadding,
            left: margin.left,
            axis: xAxis
        });

        var currentSignalScale = d3.scaleLinear()
            .domain([-0.5, 3.5])
            .range([mainHeight(), 0]);
        var currentSignalAxis = d3.axisLeft(currentSignalScale).ticks(5);
        var currentSignalAxisGroup = addAxisGroup({
            "class": "current-signal",
            top: margin.top,
            left: margin.left - axisPadding - 1,
            axis: currentSignalAxis
        });

        var zScale = d3.scaleLinear()
            .domain([-0x1000, 0xffff + 0x1000])
            .range([mainHeight(), 0]);
        var zAxis = d3.axisRight(zScale).ticks(5);
        var zAxisGroup = addAxisGroup({
            "class": "z",
            top: margin.top,
            left: margin.left + mainWidth() + axisPadding,
            axis: zAxis
        });

        var transform = d3.zoomIdentity;
        var zoomed = function () {
            zoomableGroup.attr("transform", d3.event.transform);
            transform = d3.event.transform;
            xAxisGroup.call(xAxis.scale(transform.rescaleX(xScale)));
            currentSignalAxisGroup.call(
                currentSignalAxis.scale(d3.event.transform.rescaleY(
                    currentSignalScale
                ))
            );
            zAxisGroup.call(
                zAxis.scale(d3.event.transform.rescaleY(zScale))
            );
        };
        var zoom = d3.zoom().on("zoom", zoomed);
        svg.call(zoom);

        var zLine = d3.line()
            .x(function (ignore, i) {
                return xScale(i);
            })
            .y(function (d) {
                return zScale(d[2]);
            });

        var currentSignalLine = d3.line()
            .x(function (ignore, i) {
                return xScale(i);
            })
            .y(function (d) {
                return currentSignalScale(util.voltFromInteger(d[3]));
            });

        var render = function () {
            zoomableGroup.selectAll("path").remove();
            zoomableGroup.append("path")
                .datum(positions)
                .attr("class", "z")
                .attr("d", zLine);
            zoomableGroup.append("path")
                .datum(positions)
                .attr("class", "current-signal")
                .attr("d", currentSignalLine);
        };

        var updateClipPath = function () {
            svg.select("clipPath rect")
                .attr("width", mainWidth())
                .attr("height", mainHeight());
        };

        var updateXScale = function () {
            updateClipPath();

            var oldWidth = xScale.range()[1];
            var newWidth = mainWidth();
            var resizeFactor = newWidth / oldWidth;
            var oldStretchFactor = spec.maxNoOfPositions / xScale.domain()[1];
            var stretchFactorChange = stretchFactor / oldStretchFactor;
            xScale
                .domain([0, spec.maxNoOfPositions / stretchFactor])
                .range([0, newWidth]);
            transform.x = transform.x * resizeFactor * stretchFactorChange;
            svg.call(zoom.transform, transform);
            xAxis.scale(transform.rescaleX(xScale));
            xAxisGroup.call(xAxis);
            zAxisGroup.attr("transform",
                    "translate(" + (margin.left + mainWidth() + axisPadding) +
                    "," + margin.top + ")");
            render();
        };

        var appendPositions = function (newPositions) {
            positions.push(...newPositions);
            while (positions.length > spec.maxNoOfPositions) {
                positions.shift();
            }
            render();
        };

        var clear = function () {
            positions = [];
        };

        var iframeWindowEl = d3.select(d3.select("main>iframe").node()
            .contentWindow);
        iframeWindowEl.on("resize." + spec.modeName, updateXScale);

        updateXScale();

        return {
            clear: clear,
            appendPositions: appendPositions,
            set maxNoOfPositions(newMaxNoOfPositions) {
                spec.maxNoOfPositions = newMaxNoOfPositions;
                updateXScale();
            }
        };
    };
});
