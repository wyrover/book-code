/**
 * Created by king on 15-4-10.
 *
 * ch06.child_process_fib_main.js
 */
console.info("------   child_process fib main   ------");
console.info();
/**
 * child_process fork
 * @type {exports}
 */
var cp = require('child_process');  // TODO: 引入child_process模块
var child = cp.fork('ch06.child_process_fib_sub.js');  // TODO: 每个请求都单独生成一个新的子进程
/**
 * fork message event
 */
child.on('message', function(m) {
    console.info('斐波那契数列: ' + m.result + '\n');
});
/**
 * child_process send message
 */
var input = parseInt('10');
child.send({input : 1});    // TODO: send message
for(var i = 1; i < input; i++) {
    child.send({input : i});    // TODO: send message
}
