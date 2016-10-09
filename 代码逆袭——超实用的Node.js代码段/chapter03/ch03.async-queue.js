/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-queue.js
 */
console.info('------   Node.js异步编程: Async queue   ------');
console.info();
var async = require('async');
/**
 * 定义一个queue，设worker数量为2
 */
var q = async.queue(function(task, callback) {
    console.log('worker is processing task: ', task.name);
    callback();
}, 2);
/**
 * 独立加入5个任务
 */
q.push({name: 'foo'}, function (err) {
    console.log('finished processing foo');
});
q.push({name: 'bar'}, function (err) {
    console.log('finished processing bar');
});
q.push({name: 'cap'}, function (err) {
    console.log('finished processing cap');
});
q.push({name: 'egg'}, function (err) {
    console.log('finished processing egg');
});
q.push({name: 'app'}, function (err) {
    console.log('finished processing app');
});
/**
 * listen：当最后一个任务交给worker时，将调用该函数
 */
q.empty = function() {
    console.log('no more tasks wating');
}
/**
 * listen：当所有任务都执行完以后，将调用该函数
 */
q.drain = function() {
    console.log('all tasks have been processed');
}