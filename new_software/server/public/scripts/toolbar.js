/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(["log"], function (log) {
    "use strict";

    var client;
    var toolbarEl = d3.select("div.toolbar");

    var sendIdle = function () {
        var json = "[{\"mode\":\"idle\"}]";
        client.send(json);
        log.appendInput(json);
    };

    var onStopClick = function () {
        d3.event.preventDefault();
        if (client !== undefined) {
            sendIdle();
        }
        return false;
    };

    toolbarEl.select("button.stop").on("click", onStopClick);

    return {
        set client(newClient) {
            client = newClient;
        }
    };
});
