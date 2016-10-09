/**
 * Created by king on 15-4-20.
 *
 * ch08.fs_ftruncateSync.js
 */
console.info("------   fs ftruncateSync()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/ftruncateSync.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/ftruncateSync.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
        var fd = fs.openSync('txt/ftruncateSync.txt', 'a'); // TODO: 打开文件（同步方式）
        fs.ftruncateSync(fd, 10);  // TODO: 文件内容截取
        console.info("文件描述符: " + fd);   // TODO: 打印输出文件描述符
        console.info();
        console.log('fs.ftruncateSync() Done');
        fs.closeSync(fd);   // TODO: 关闭文件（同步方式）
        var child_suf = exec('cat txt/ftruncateSync.txt',
            function (error, stdout, stderr) {
                console.info('cat txt/ftruncateSync.txt stdout: ');
                console.log(stdout);   // TODO: 打印输出 stdout
                console.log(stderr);   // TODO: 打印输出 stdout
            });
        console.info();
    });
console.info();