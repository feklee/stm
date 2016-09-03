/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(function () {
    "use strict";

    var logEl = d3.select("ul.log");

    var append = function (type, icon, text) {
        var html = "<li class=\"" + type + "\"><span>" + icon + "</span> " +
                text + "</li>"; // todo: escape HTML
        logEl.html(logEl.html() + html);
    };

    return {
        appendInput(text) {
            append("input", "&gt;", text);
        },
        appendError(text) {
            append("error", "×", text);
        }
    };
});
