/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-waterfall.js
 */
console.info('------   Node.js异步编程: Async waterfall   ------');
console.info();
var async = require('async');   // TODO: 引用'async'包
/**
 * TODO: 使用waterfall函数方法
 */
async.waterfall([
    function(callback) {
        callback(null, 1);    // TODO: 当回调函数的第一个参数为非空值时，waterfall会停止执行剩余任务
    },
    function(data, callback) {
        console.info(data);
        callback('test', 2);
    },
    function(data, callback) {
        console.info(data);
        callback(null, 3);
    }
],function(err, results) {
    console.log(results);
});
console.info();
console.info('------   Node.js异步编程: Async waterfall   ------');