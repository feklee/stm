/*jslint browser: true, es6: true, maxlen: 80 */

/*global define, window, d3 */

define(function () {
    "use strict";

    return {
        // converts to Volts, from integer representation of voltage in units
        // 0xffff/3.3 V
        voltFromInteger(x) {
            return x / (0xffff / 3.3);
        }
    };
});
