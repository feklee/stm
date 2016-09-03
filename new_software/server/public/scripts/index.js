/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define([
    "graph-constructor", "input-form"
], function (graphConstructor, inputForm) {
    "use strict";

    var hostname = window.location.hostname;
    var client = new window.WebSocket("ws://" + hostname + ":8080/");
    var graphs = {
        "scan": graphConstructor("scan"),
        "approach": graphConstructor("approach"),
        "retract": graphConstructor("retract")
    };
    var modeName = "";

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
            if (graphs[modeName] !== undefined) {
                graphs[modeName].render(data.positions);
            }
            break;
        case "scanDuration":
            d3.select(".scan-duration").text(d3.format(".3f")(
                data.value / 1000000
            ));
            break;
        case "newMode":
            d3.select(".mode-name").text(data.value);
            modeName = data.value;
            break;
        case "biasVoltage":
            d3.select(".bias-voltage").text(d3.format(".1f")(data.value));
            break;
        case "currentSignal":
            d3.select(".current-signal").text(d3.format(".2f")(data.value));
            break;
        case "peakCoarseApproachSignal":
            d3.select(".peak-coarse-approach-signal").text(d3.format(".2f")(
                data.value
            ));
            break;
        case "peakFineApproachSignal":
            d3.select(".peak-fine-approach-signal").text(d3.format(".2f")(
                data.value
            ));
            break;
        case "error":
            window.console.log("STM error: " + data.value);
            break;
        }
    };

    inputForm.client = client;
});
