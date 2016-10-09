/**
 * Created by king on 15-4-10.
 *
 * ch06.child_process_fork_main.js
 */
console.info("------   child_process fork main   ------");
console.info();
/**
 * child_process fork
 * @type {exports}
 */
var cp = require('child_process');  // TODO: 引入child_process模块
var n = cp.fork('ch06.child_process_fork_sub.js');  // TODO: fork 子进程
/**
 * fork message event
 */
n.on('message', function(m) {
    console.log('PARENT got message:', m);
});
/**
 * child_process send message
 */
n.send({ main: 'sub' }); // TODO: send message
console.info();