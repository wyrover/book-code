/**
 * Created by king on 15-4-10.
 *
 * ch06.child_process_exec_last.js
 */
console.info("------   child_process exec last   ------");
console.info();
/**
 * 创建子进程统计系统登录次数
 * @type {exports.exec|*}
 */
var exec = require('child_process').exec;
var last = exec('last | wc -l');
/**
 * 捕获控制台输出对象stdout的‘data’事件
 */
last.stdout.on('data', function (data) {
    console.log('系统登录次数统计: ' + data);
});
/**
 * 绑定系统‘exit’事件
 */
last.on('exit', function (code) {
    console.log('子进程已关闭,代码: ' + code);
});