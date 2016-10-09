/**
 * Created by king on 15-4-20.
 *
 * ch08.fs_truncateSync.js
 */
console.info("------   fs truncateSync()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/truncateSync.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/truncateSync.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log('length: ' + stdout.length); // TODO: 输出数据长度
        console.log(stderr);   // TODO: 打印输出 stderr
        console.info();
        fs.truncateSync('txt/truncateSync.txt', 6);  // TODO: 修改文件内容长度(同步方法)
        console.log('fs.truncateSync() Done');
        var child_suf = exec('cat txt/truncateSync.txt',
            function (error, stdout, stderr) {
                console.info('cat txt/truncateSync.txt stdout: ');
                console.log(stdout);   // TODO: 打印输出 stdout
                console.log('length: ' + stdout.length); // TODO: 输出数据长度
                console.log(stderr);   // TODO: 打印输出 stdout
                console.info();
            });
    });
console.info();