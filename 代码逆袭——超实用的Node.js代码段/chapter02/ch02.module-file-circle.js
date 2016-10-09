/**
 * Created by king on 15-2-28.
 *
 * ch02.module-file-circle.js
 */
var PI = Math.PI;
exports.area = function(r) {
    return PI * r * r;
};
exports.circumference = function(r) {
    return 2 * PI * r;
};