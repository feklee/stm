/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window */

define(function () {
    "use strict";

    var submitButtonEl = document.querySelector("form>input[type=\"submit\"]");
    var commandTextEl = document.querySelector("form>input[name=\"command\"]");
    var logEl = document.querySelector("form>pre");
    var client;

    var appendToLog = function (command) {
        logEl.innerHTML += command + "<br />";
        logEl.scrollTop = logEl.scrollHeight;
    };

    var submitCommand = function (command) {
        client.send(command);
        appendToLog(command);
    };

    var submit = function (e) {
        if (client !== undefined) {
            submitCommand(commandTextEl.value);
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
