/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(function () {
    "use strict";

    var submitButtonEl = document.querySelector("form>input[type=\"submit\"]");
    var inputEl =
            document.querySelector("form>input[name=\"mode-chain-json\"]");
    var logEl = document.querySelector("form>pre");
    var client;

    var appendToLog = function (modeChainJson) {
        logEl.innerHTML += modeChainJson + "<br />";
        logEl.scrollTop = logEl.scrollHeight;
    };

    var sendModeChainJson = function (modeChainJson) {
        client.send(modeChainJson);
        appendToLog(modeChainJson);
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
