/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-doUntil.js
 */
var async = require('async');
var count = 5;
async.doUntil(
    function (callback) {
        count--;
        setTimeout(callback, 1000);
    },
    function () { return count < 0; },
    function (err) {
        console.log();
    }
);
//doUntil(fn, test, callback)
//doUntil与doWhilst正好相反，当test为false时循环，与true时跳出。其它特性一致。
//和doWhilst类似的效果，根据实际需要选择doUntil或doWhilst。