/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-until.js
 */
var async = require('async');
var count = 5;
async.until(
    function () { return count < 0; },
    function (callback) {
        count--;
        setTimeout(callback, 1000);
    },
    function (err) {
        console.log();
    }
);
//until(test, fn, callback)
//until与whilst正好相反，当test条件函数返回值为false时继续循环，与true时跳出。其它特性一致。
//和whilst类似的效果，根据实际需要选择until或whilst。