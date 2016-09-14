/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define([
    "graph-constructor", "input-form", "log", "util"
], function (graphConstructor, inputForm, log, util) {
    "use strict";

    var hostname = window.location.hostname;
    var client = new window.WebSocket("ws://" + hostname + ":8080/");
    var graphs = {
        "approach": graphConstructor({
            indexDomainEnd: 20000,
            timeDomainEnd: 200000,
            modeName: "approach",
            maxNoOfPositions: 20000
        }),
        "scan": graphConstructor({
            indexDomainEnd: 20000,
            timeDomainEnd: 200000,
            modeName: "scan",
            maxNoOfPositions: 100
        }),
        "retract": graphConstructor({
            indexDomainEnd: 2000,
            timeDomainEnd: 20000,
            modeName: "retract",
            maxNoOfPositions: 500
        })
    };
    var modeName = "";

    client.onerror = function () {
        log.appendError("WebSocket error");
    };

    client.onopen = function () {
        log.appendInfo("WebSocket opened");
    };

    client.onclose = function () {
        log.appendWarn("WebSocket closed");
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
                graphs[modeName].appendPositions(data.positions);
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
            if (graphs[modeName] !== undefined) {
                graphs[modeName].clear();
            }
            break;
        case "sideLen":
            graphs.scan.maxNoOfPositions = data.value * data.value;
            break;
        case "biasVoltage":
            d3.select(".bias-voltage").text(d3.format(".1f")(data.value));
            break;
        case "currentSignal":
            d3.select(".current-signal").text(d3.format(".2f")(
                util.voltFromInteger(data.value)
            ));
            break;
        case "peakCoarseApproachSignal":
            d3.select(".peak-coarse-approach-signal").text(d3.format(".2f")(
                util.voltFromInteger(data.value)
            ));
            break;
        case "peakFineApproachSignal":
            d3.select(".peak-fine-approach-signal").text(d3.format(".2f")(
                util.voltFromInteger(data.value)
            ));
            break;
        case "targetSignal":
            d3.select("section." + modeName).select(".target-signal")
                .text(d3.format(".2f")(data.value));
            break;
        case "error":
            log.appendError(data.value);
            break;
        default:
            log.appendError("Unknown: " + json);
        }
    };

    inputForm.client = client;
});
