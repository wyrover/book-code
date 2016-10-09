/**
 * Created by king on 15-4-3.
 *
 * ch06.child_process_exec_std.js
 */
console.info("------   child_process exec std   ------");
console.info();
/**
 * child_process exec
 * @type {exports.exec|*}
 */
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child = exec('cat ch06.child_process_exec_usage',
    function (error, stdout, stderr) {
        console.info('cat ch06.child_process_exec_usage.js stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.info('cat ch06.child_process_exec_usage.js stderr: ');
        console.log(stderr);   // TODO: 打印输出 stderr
        if (error !== null) {
            console.info('cat ch06.child_process_exec_usage.js error: ');
            console.log(error);    // TODO: 打印输出错误信息
        }
    });