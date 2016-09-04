/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(function () {
    "use strict";

    var logEl = d3.select("ul.log");

    var append = function (type, text) {
        logEl.append("li").attr("class", type).text(text);
    };

    return {
        appendInput(text) {
            append("input", text);
        },
        appendError(text) {
            append("error", text);
        }
    };
});
