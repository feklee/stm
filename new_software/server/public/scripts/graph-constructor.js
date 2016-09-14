/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(["util"], function (util) {
    "use strict";

    return function (spec) {
        var positions = [];
        var startTime = function () {
            if (positions.length === 0) {
                return 0;
            }
            return positions[0][4];
        };
        var endTime = function () {
            if (positions.length === 0) {
                return 0;
            }
            return positions[positions.length - 1][4];
        };
        var timeSpan = function () {
            return endTime() - startTime();
        };
        var margin = {
            top: 0, // px
            left: 30, // px
            bottom: 20, // px
            right: 50 // px
        };
        var figure = d3.select("body section." + spec.modeName + " figure");
        var svgEl = figure.select("svg");
        var boundingBox = function () {
            return svgEl.node().getBoundingClientRect();
        };
        var mainWidth = 100; // px
        var mainHeight =
                boundingBox().height - margin.top - margin.bottom; // px
        var axisPadding = 2; // px
        var stretchFactor = 1;
        var stretchFactorInputEl = figure.select("input.stretch-factor");
        var timeDomainIsSelected;
        var horizontalDomainEnd;

        var mainGroup = svgEl
            .select("g.main")
            .attr("transform",
                    "translate(" + margin.left + "," + margin.top + ")")
            .attr("class", "main");

        var zoomableGroup = mainGroup.select("g.zoomable");

        var addAxisGroup = function (spec) {
            var group = svgEl
                .select("g." + spec.class + ".axis")
                .attr("transform",
                        "translate(" + spec.left + "," + spec.top + ")");
            group.call(spec.axis);
            return group;
        };

        var horizontalScale = d3.scaleLinear();
        var horizontalAxis = d3.axisBottom(horizontalScale).ticks(5);
        var horizontalAxisGroup = addAxisGroup({
            "class": "horizontal",
            top: margin.top + mainHeight + axisPadding,
            left: margin.left,
            axis: horizontalAxis
        });

        var currentSignalScale = d3.scaleLinear()
            .domain([0, 3.5])
            .range([mainHeight, 0]);
        var currentSignalAxis = d3.axisLeft(currentSignalScale).ticks(3);
        var currentSignalAxisGroup = addAxisGroup({
            "class": "current-signal",
            top: margin.top,
            left: margin.left - axisPadding - 1,
            axis: currentSignalAxis
        });

        var zScale = d3.scaleLinear()
            .domain([0, 0xffff + 0x1000])
            .range([mainHeight, 0]);
        var zAxis = d3.axisRight(zScale).ticks(3);
        var zAxisGroup = addAxisGroup({
            "class": "z",
            top: margin.top,
            left: margin.left + mainWidth + axisPadding,
            axis: zAxis
        });

        var transform = d3.zoomIdentity;
        var zoomed = function () {
            zoomableGroup.attr("transform", d3.event.transform);
            transform = d3.event.transform;
            horizontalAxisGroup.call(
                horizontalAxis.scale(transform.rescaleX(horizontalScale))
            );
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
        svgEl.call(zoom);

        var x = function (d, i) {
            return horizontalScale(
                timeDomainIsSelected
                    ? d[4] - startTime()
                    : i
            );
        };

        var zLine = d3.line()
            .x(x)
            .y(function (d) {
                return zScale(d[2]);
            });

        var currentSignalLine = d3.line()
            .x(x)
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
            svgEl.select("clipPath rect")
                .attr("width", mainWidth)
                .attr("height", mainHeight);
        };

        var updateHorizontalScale = function () {
            horizontalScale
                .domain([0, horizontalDomainEnd / stretchFactor])
                .range([0, mainWidth]);
            svgEl.call(zoom.transform, transform);
            horizontalAxis.scale(transform.rescaleX(horizontalScale));
            horizontalAxisGroup.call(horizontalAxis);
            zAxisGroup.attr("transform",
                    "translate(" + (margin.left + mainWidth + axisPadding) +
                    "," + margin.top + ")");
        };

        var setStretchFactor = function (newStretchFactor) {
            var stretchFactorChange = newStretchFactor / stretchFactor;
            stretchFactor = newStretchFactor;
            transform.x = transform.x * stretchFactorChange;
            updateHorizontalScale();
        };

        var setWidth = function (newWidth) {
            var newMainWidth = newWidth - margin.left - margin.right;
            var resizeFactor = newMainWidth / mainWidth;
            mainWidth = newMainWidth;
            transform.x = transform.x * resizeFactor;
            updateHorizontalScale();
            updateClipPath();
        };

        var trimPositions = function () {
            while (positions.length > spec.maxNoOfPositions) {
                positions.shift();
            }
        };

        var appendPositions = function (newPositions) {
            positions.push(...newPositions);
            trimPositions();
        };

        var setMaxNoOfPositions = function (newMaxNoOfPositions) {
            spec.maxNoOfPositions = newMaxNoOfPositions;
            trimPositions();
        };

        var clear = function () {
            positions = [];
        };

        var resetStretchFactor = function () {
            stretchFactor = 1;
            stretchFactorInputEl.node().value = stretchFactor;
        };

        var resetZoom = function () {
            svgEl.call(zoom.transform, d3.zoomIdentity);
        };

        var fitTimeSpan = function () {
            if (timeSpan() > 0) {
                horizontalDomainEnd = timeSpan();
            }
        };

        var fitIndex = function () {
            if (positions.length > 0) {
                horizontalDomainEnd = positions.length - 1;
            }
        };

        var fitWidth = function () {
            resetStretchFactor();
            resetZoom();
            if (timeDomainIsSelected) {
                fitTimeSpan();
            } else {
                fitIndex();
            }
            updateHorizontalScale();
        };

        var selectTimeDomain = function () {
            horizontalDomainEnd = spec.timeDomainEnd;
            timeDomainIsSelected = true;
            resetStretchFactor();
            resetZoom();
            updateHorizontalScale();
        };

        var selectIndexDomain = function () {
            horizontalDomainEnd = spec.indexDomainEnd;
            timeDomainIsSelected = false;
            resetStretchFactor();
            resetZoom();
            updateHorizontalScale();
        };

        var iframeWindowEl = d3.select(d3.select("main>iframe").node()
            .contentWindow);
        iframeWindowEl.on("resize." + spec.modeName, function () {
            setWidth(boundingBox().width);
            render();
        });

        stretchFactorInputEl.on("change", function () {
            setStretchFactor(stretchFactorInputEl.node().value);
            render();
        });

        figure.select("button.fit-width").on("click", function () {
            fitWidth();
            render();
        });

        figure.select("#" + spec.modeName + "-horizontal-time")
            .on("click", function () {
                selectTimeDomain();
                render();
            });

        figure.select("#" + spec.modeName + "-horizontal-index")
            .on("click", function () {
                selectIndexDomain();
                render();
            });

        selectTimeDomain();
        setWidth(boundingBox().width);
        render();

        return {
            clear: function () {
                clear();
                render();
            },
            appendPositions: function (...a) {
                appendPositions(...a);
                render();
            },
            set maxNoOfPositions(newMaxNoOfPositions) {
                setMaxNoOfPositions(newMaxNoOfPositions);
                render();
            }
        };
    };
});
