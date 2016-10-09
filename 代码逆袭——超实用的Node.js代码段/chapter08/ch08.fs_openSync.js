/**
 * Created by king on 15-4-20.
 *
 * ch08.fs_openSync.js
 */
console.info("------   fs openSync()   ------");
console.info();
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/openSync.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/openSync.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
    });
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var fd = fs.openSync('txt/openSync.txt', 'a');  // TODO: 打开文件（同步方式）
console.info("文件描述符: " + fd);
console.log('fs.openSync() Done.');
console.info();
fs.closeSync(fd);
console.info("文件描述符: " + fd);
console.log('fs.closeSync() Done.');
console.info();
