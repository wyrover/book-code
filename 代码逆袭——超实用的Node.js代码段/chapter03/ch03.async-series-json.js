/**
 * Created by king on 15-2-23.
 *
 * ch03.asyn-series-json.js
 */
console.info('------   Node.js异步编程: Async series   ------');
console.info();
var async = require('async');   // TODO: 引用'async'包
/**
 * TODO: 使用series函数方法
 */
async.series({
    one: function(callback) {
        callback(null, 'hello');
    },
    two: function(callback) {
        callback(null, 'async');
    },
    three: function(callback) {
        callback(null, 'series');
    }
},function(err, results) {
    console.log(results);
});
console.info();
console.info('------   Node.js异步编程: Async series   ------');