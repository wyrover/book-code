/**
 * Created by king on 15-3-30.
 *
 * ch06.child_process_spawn_lserr.js
 */
console.info("------   child_process spawn lserr   ------");
console.info();
var spawn = require('child_process').spawn; // TODO: 引入child_process模块
var ls_err = spawn('ls', ['-lh', '/err']);  // TODO: 定义命令行‘ls -lh /usr’
/**
 * 捕获控制台错误对象stderr,输出捕获数据
 */
ls_err.stderr.on('data', function (data) {
    console.log('stderr: ' + data);
});
/**
 * 绑定系统‘error’事件
 */
ls_err.on('error', function (code) {
    console.log('child process error with code ' + code);
});
/**
 * 绑定系统‘close’事件
 */
ls_err.on('close', function (code) {
    console.log('child process closed with code ' + code);
});
/**
 * 绑定系统‘exit’事件
 */
ls_err.on('exit', function (code) {
    console.log('child process exited with code ' + code);
});