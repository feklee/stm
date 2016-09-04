/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define([
    "graph-constructor", "input-form", "log"
], function (graphConstructor, inputForm, log) {
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
        var json;
        if (typeof e.data === "string") {
            json = e.data;
            data = JSON.parse(json);
        } else {
            return;
        }

        switch (data.type) {
        case "tipPositionLog":
            if (graphs[modeName] !== undefined) {
                graphs[modeName].data = data.positions;
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
        case "faderPosition":
            break; // todo: eventually remove
        case "error":
            log.appendError(data.value);
            break;
        default:
            log.appendError("Unknown: " + json);
        }
    };

    inputForm.client = client;
});