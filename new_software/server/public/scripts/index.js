/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define([
    "graph-constructor", "input-form"
], function (graphConstructor, inputForm) {
    "use strict";

    var hostname = window.location.hostname;
    var client = new window.WebSocket("ws://" + hostname + ":8080/");
    var drawRate = 15; // items / ms
    var graphIndexes = [0, 1, 2, 3];

    var graphs = graphIndexes.map(function (index) {
        return graphConstructor({
            index: index,
            pointDrawRate: drawRate, // points / ms
            verticalStretchFactor: 3
        });
    });

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
        case "graphPoints":
            graphs[data.index].appendPoints(data.points);
            break;
        }
    };

    inputForm.client = client;
});
