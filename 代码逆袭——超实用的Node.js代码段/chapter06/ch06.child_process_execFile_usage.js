/**
 * Created by king on 15-4-10.
 *
 * ch06.child_process_execFile_usage.js
 */
console.info("------   child_process execFile usage   ------");
console.info();
/**
 * child_process execFile
 * 查看当前目录内容
 * @type {exports}
 */
var child_process = require('child_process');   // TODO: 引入child_process模块
child_process.execFile('/bin/ls', ['-lh', '.'], function (error,stdout,stderr) {
    if (error !== null) {
        console.log('execFile error: ' + error);    // TODO: 打印输出 error
        console.log('execFile stderr: ' + stderr);    // TODO: 打印输出 error
    } else {
        console.info('当前目录内容:');
        console.info(stdout);   // TODO: 打印输出 stdout
    }
});