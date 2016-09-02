/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(function () {
    "use strict";

    function constructor(modeName) {
        var width = d3.select("svg").node().getBoundingClientRect().width;
        var height = d3.select("svg").node().getBoundingClientRect().height;

        var yScale = d3.scaleLinear()
            .domain([-0.5, 3.5])
            .range([height, 0]);

        var svg = d3.select("body svg." + modeName);

        var graphGroup = svg.append("g");

        var onZoom = function () {
            graphGroup.attr("transform", d3.event.transform);
        };

        var zoomBehavior = d3.zoom()
            .scaleExtent([1, 10])
            .translateExtent([[0, 0], [width - 1, height - 1]])
            .on("zoom", onZoom);

        svg.call(zoomBehavior);

        var xScale;

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

        var render = function (data) {
            xScale = d3.scaleLinear()
                .domain([0, data.length - 1])
                .range([0, width]);

            graphGroup.selectAll("path").remove();
            graphGroup.append("path")
                .datum(data)
                .attr("class", "z")
                .attr("d", zLine);
            graphGroup.append("path")
                .datum(data)
                .attr("class", "current-signal")
                .attr("d", currentSignalLine);
        };

        return {
            render: render
        };
    }

    return constructor;
});
