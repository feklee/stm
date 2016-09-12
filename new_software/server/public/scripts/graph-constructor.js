/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(["util"], function (util) {
    "use strict";

    return function (spec) {
        var positions = [];
        var margin = {
            top: 0, // px
            left: 30, // px
            bottom: 30, // px
            right: 50 // px
        };
        var svg = d3.select("body section." + spec.modeName + " svg");
        var boundingBox = svg.node().getBoundingClientRect();
        var getMainWidth = function () {
            return boundingBox.width - margin.left - margin.right;
        };
        var mainHeight = boundingBox.height - margin.top - margin.bottom;
        var mainWidth = getMainWidth();
        var axisPadding = 5; // px

        var mainGroup = svg
            .append("g")
            .attr("transform",
                    "translate(" + margin.left + "," + margin.top + ")")
            .attr("class", "main");

        var backgroundRect = mainGroup
            .append("rect")
            .attr("width", mainWidth)
            .attr("height", mainHeight);

        var addAxis = function (spec) {
            var axisName = "axis" + util.ucfirst(spec.orientation);
            var axis = d3[axisName](spec.scale).ticks(5);
            var group = svg
                .append("g")
                .attr("class", spec.class + " axis")
                .attr("transform",
                        "translate(" + spec.left + "," + spec.top + ")");
            group.call(axis);
            return axis;
        };

        var xScale = d3.scaleLinear()
            .range([0, mainWidth]);
        var xAxis = addAxis({
            scale: xScale,
            top: margin.top + mainHeight + axisPadding,
            left: margin.left,
            orientation: "bottom",
            "class": "x"
        });

        var currentSignalScale = d3.scaleLinear()
            .domain([-0.5, 3.5])
            .range([mainHeight, 0]);
        addAxis({
            scale: currentSignalScale,
            top: margin.top,
            left: margin.left - axisPadding - 1,
            orientation: "left",
            "class": "current-signal"
        });

        var zScale = d3.scaleLinear()
            .domain([-0x1000, 0xffff + 0x1000])
            .range([mainHeight, 0]);
        addAxis({
            scale: zScale,
            top: margin.top,
            left: margin.left + mainWidth + axisPadding,
            orientation: "right",
            "class": "z"
        });

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

        var updateXScale = function () {
            boundingBox = svg.node().getBoundingClientRect();
            mainWidth = getMainWidth();
            backgroundRect.attr("width", mainWidth);
            xScale
                .domain([0, spec.maxNoOfPositions - 1])
                .range([0, mainWidth]);
            svg.select("g.x.axis").call(xAxis);
            svg.select("g.z.axis").attr("transform",
                    "translate(" + (margin.left + mainWidth + axisPadding) +
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
