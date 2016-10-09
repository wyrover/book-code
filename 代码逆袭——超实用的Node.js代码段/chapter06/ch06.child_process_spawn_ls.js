/**
 * Created by king on 15-3-30.
 *
 * ch06.child_process_spawn_ls.js
 */
console.info("------   child_process spawn ls   ------");
console.info();
var spawn = require('child_process').spawn; // TODO: 引入child_process模块
var ls_var = spawn('ls', ['-lh', '/usr']);  // TODO: 定义命令行‘ls -lh /usr’
/**
 * 捕获控制台输出对象stdout,输出捕获数据
 */
ls_var.stdout.on('data', function (data) {
    console.log('stdout: ' + data);
});
/**
 * 绑定系统‘error’事件
 */
ls_var.on('error', function (code) {
    console.log('child process error with code ' + code);
});
/**
 * 绑定系统‘close’事件
 */
ls_var.on('close', function (code) {
    console.log('child process closed with code ' + code);
});
/**
 * 绑定系统‘exit’事件
 */
ls_var.on('exit', function (code) {
    console.log('child process exited with code ' + code);
});