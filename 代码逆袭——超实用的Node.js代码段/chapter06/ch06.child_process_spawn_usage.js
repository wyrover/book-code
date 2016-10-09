/**
 * Created by king on 15-4-3.
 *
 * ch06.child_process_spawn_usage.js
 */
console.info("------   child_process spawn usage   ------");
console.info();
/**
 * child_process spawn
 * @type {exports.spawn|*}
 */
var spawn = require('child_process').spawn; // TODO: 引入child_process模块
var ls_var = spawn('ls', ['-lh', '/var']);  // TODO: 定义命令行‘ls -lh /var’
/**
 * 捕获控制台输出对象stdout,输出捕获数据
 */
ls_var.stdout.on('data', function (data) {
    console.log('stdout: ' + data);         // TODO: 打印输出 /var 目录清单
});
