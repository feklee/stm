// Simulates scanning under ideal conditions.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

function intensity(x, y) {
    // Formula for hexagonal egg-carton modulation from arXiv:1203.5343v2:
    // M / 3 (2 cos(Qx) cos(1 / √3 Qy) + cos(2 / √3 Qy))

    var Q = 0.4;
    return (1 / 3 * (2 * Math.cos(Q * x) * Math.cos(1 / Math.sqrt(3) * Q * y) +
            Math.cos(2 / Math.sqrt(3) * Q * y)) + 0.5) / 1.5;
}

module.exports = {
    intensity: intensity
};
