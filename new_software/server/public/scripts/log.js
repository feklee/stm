/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(function () {
    "use strict";

    var logEl = d3.select("div.log");

    return {
        append(text) {
            var html = "<pre>&gt; </pre><pre>" +
                    text + "</pre>"; // todo: escape HTML
            logEl.html(logEl.html() + html);
        }
    };
});
