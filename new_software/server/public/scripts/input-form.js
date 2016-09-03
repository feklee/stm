/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(["log"], function (log) {
    "use strict";

    var submitButtonEl = document.querySelector("form>input[type=\"submit\"]");
    var inputEl =
            document.querySelector("form>input[name=\"mode-chain-json\"]");
    var client;

    var sendModeChainJson = function (modeChainJson) {
        client.send(modeChainJson);
        log.appendInput(modeChainJson);
    };

    var submit = function (e) {
        if (client !== undefined) {
            sendModeChainJson(inputEl.value);
        }
        e.preventDefault();
        document.querySelector("form").reset();
    };

    submitButtonEl.addEventListener("click", submit, false);

    return {
        set client(newClient) {
            client = newClient;
        }
    };
});
