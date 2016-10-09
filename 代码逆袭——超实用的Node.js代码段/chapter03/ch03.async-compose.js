/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-compose.js
 */
var async = require('async');
function fn1(n, callback) {
    setTimeout(function () {
        callback(null, n + 1);
    }, 1000);
}
function fn2(n, callback) {
    setTimeout(function () {
        callback(null, n * 3);
    }, 1000);
}
var demo = async.compose(fn1, fn2);
demo(4, function (err, result) {
    // result now equals 15
});
//compose(fn1, fn2...);
//创建一个包括一组异步函数的函数集合，每个函数会消费上一次函数的返回值。把f(),g(),h()异步函数，组合成f(g(h()))的形式，通过callback得到返回值。
//执行示例代码，将result结果输出到控制台，观察运行消耗的时间。
//compose和waterfall作用相仿，不过compose更加灵活，可以自定义传入参数。