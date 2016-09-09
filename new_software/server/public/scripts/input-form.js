/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(["log", "toolbar"], function (log, toolbar) {
    "use strict";

    var client;

    var sendModeChainJson = function (modeChainJson) {
        client.send(modeChainJson);
        log.appendInput(modeChainJson);
    };

    var submit = function () {
        d3.event.preventDefault();
        if (client !== undefined) {
            var inputNode = d3.select("input[name='mode-chain-json']").node();
            sendModeChainJson(inputNode.value);
        }
        document.querySelector("form").reset();
        return false;
    };

    d3.select("input[type='submit']").on("click", submit);

    return {
        set client(newClient) {
            client = newClient;
            toolbar.client = client;
        }
    };
});
