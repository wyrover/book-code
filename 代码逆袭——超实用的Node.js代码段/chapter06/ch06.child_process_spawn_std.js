/**
 * Created by king on 15-4-3.
 *
 * ch06.child_process_spawn_std.js
 */
console.info("------   child_process spawn std   ------");
console.info();
var cp = require('child_process');  // TODO: 引入child_process模块
var cat = cp.spawn('cat');          // TODO: 定义命令行‘cat’
/**
 * 捕获控制台输出对象stdout的‘data’事件
 */
cat.stdout.on('data', function(d) {
    console.log(d.toString());
});
/**
 * 绑定系统‘exit’事件
 */
cat.on('exit', function() {
    console.log('cat on exit!');
});
/**
 * 绑定系统‘close’事件
 */
cat.on('close', function() {
    console.log('cat on close!');
});
cat.stdin.write('cat on data!');    // TODO: 通过控制台输入对象stdin写入数据
cat.stdin.end();    // TODO: 结束控制台输入对象stdin