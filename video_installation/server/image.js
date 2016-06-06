// Simulates scanning.

/*jslint node: true, es6: true, maxlen: 80 */

'use strict';

var pngJs = require('png-js');
var imgData;
var imgWidth;

function intensity(x, y) {
    if (imgData) {
        return imgData[4 * (y * imgWidth + x)] / 255;
    }
    return 0;
}

function load() {
    var png = pngJs.load('image.png');

    png.decode(function (x) {
        imgData = x;
        imgWidth = png.width;
    });
}

load('image.png');

module.exports = {
    intensity: intensity
};
