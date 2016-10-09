/**
 * Created by king on 15-4-9.
 *
 * ch06.child_process_spawn_free.js
 */
console.info("------   child_process spawn free   ------");
console.info();
/**
 * child_process spawn
 * 利用子进程获取系统内存使用情况
 * @type {exports.spawn|*}
 */
var spawn = require('child_process').spawn; // TODO: 引入child_process模块
var free = spawn('free', ['-m']);           // TODO: 定义命令行‘free -m’
/**
 * 捕获标准输出并将其打印到控制台
 */
free.stdout.on('data', function (data) {
    console.log('标准输出:\n' + data);
});
/**
 * 捕获标准错误输出并将其打印到控制台
 */
free.stderr.on('data', function (data) {
    console.log('标准错误输出:\n' + data);
});
/**
 * 注册子进程关闭事件
 */
free.on('exit', function (code, signal) {
    console.log('子进程已退出,代码: ' + code);
});