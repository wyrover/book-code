/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-parallel.js
 */
console.info('------   Node.js异步编程: Async parallel   ------');
console.info();
var async = require('async');   // TODO: 引用'async'包
/**
 * TODO: 使用parallel函数方法
 */
async.parallel([
    function(callback) {
        setTimeout(function() {
            callback(null, 'one');
        }, 2000);
    },
    function(callback) {
        setTimeout(function() {
            callback(null, 'two');
        }, 1000);
    }
],
function(err, results) {
    console.log(results);
});
console.info();
console.info('------   Node.js异步编程: Async parallel   ------');