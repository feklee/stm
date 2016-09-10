// Interface to log shown in the browser

/*jslint node: true, es6: true, maxlen: 80 */

"use strict";

var browserConnection;

var append = function (type, text) {
    if (type === "error" || type === "warn") {
        console.error(text);
    } else {
        console.log(text);
    }

    if (browserConnection === undefined) {
        return;
    }

    var data = {
        type: type,
        value: text
    };
    browserConnection.sendUTF(JSON.stringify(data));
};

module.exports = {
    set browserConnection(newBrowserConnection) {
        browserConnection = newBrowserConnection;
    },
    appendInfo: function (text) {
        append("info", text);
    },
    appendWarn: function (text) {
        append("warn", text);
    },
    appendError: function (text) {
        append("error", text);
    }
};
