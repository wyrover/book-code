/**
 * Created by king on 15-4-7.
 *
 * ch06.child_process_fork_usage.js
 */
console.info("------   child_process fork usage   ------");
console.info();
/**
 * child_process fork
 * @type {exports.fork}
 */
var fork = require('child_process').fork;   // TODO: 引入child_process模块
/**
 * 获取当前机器的 CPU 内核数量
 */
var cpus = require('os').cpus();
console.info('当前机器CPU内核数量: ' + cpus.length);  // TODO: 获取 CPU 内核数量
for (var i = 0; i < cpus.length; i++) {
    /**
     * 通过fork()方法创建新的子进程
     */
    console.log('Fork a new child_process now...');
    fork('./worker.js');    // TODO: 生成新子进程
}