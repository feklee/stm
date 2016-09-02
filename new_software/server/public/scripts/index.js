/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define([
    "graph-constructor", "input-form"
], function (graphConstructor, inputForm) {
    "use strict";

    var hostname = window.location.hostname;
    var client = new window.WebSocket("ws://" + hostname + ":8080/");
    var scanGraph = graphConstructor("scan");

    client.onerror = function () {
        window.console.log("Connection error");
    };

    client.onopen = function () {
        window.console.log("Client connected");
    };

    client.onclose = function () {
        window.console.log("Client closed");
    };

    client.onmessage = function (e) {
        var data;
        if (typeof e.data === "string") {
            window.console.log(e.data);
            data = JSON.parse(e.data);
        } else {
            return;
        }

        switch (data.type) {
        case "tipPositionLog":
            window.console.log(data.positions);
            scanGraph.render(data.positions);
            break;
        case "scanDuration":
            window.console.log("Scan duration: " + data.value / 1000000 +
                    " s");
            break;
        case "newMode":
            d3.select(".mode").text(data.value);
            break;
        case "peakCoarseApproachSignal":
            window.console.log("Peak coarse approach signal: " + data.value +
                    " V");
            break;
        case "peakFineApproachSignal":
            window.console.log("Peak fine approach signal: " + data.value +
                    " V");
            break;
        case "error":
            window.console.log("STM error: " + data.value);
            break;
        }
    };

    inputForm.client = client;
});
