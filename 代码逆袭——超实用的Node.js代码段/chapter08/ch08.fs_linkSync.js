/**
 * Created by king on 15-4-23.
 *
 * ch08.fs_linkSync.js
 */
console.info("------   fs linkSync()   ------");
console.info();
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/linkSync.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/linkSync.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
    });
console.info();
var srcpath = "txt/linkSync.txt";
var dstpath = "txtlink/linkSync.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
fs.linkSync(srcpath, dstpath);  // TODO: 链接文件（同步方式）
console.log('fs.linkSync() Done.');
console.info();
var child_suf = exec('cat txtlink/linkSync.txt',
    function (error, stdout, stderr) {
        console.info('cat txtlink/linkSync.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
        fs.unlinkSync(dstpath);
        console.log('fs.unlinkSync() Done.');
        console.info();
        var spawn = require('child_process').spawn; // TODO: 引入child_process模块
        var ls_txt = spawn('ls', ['-lh', 'txtlink']);  // TODO: 定义命令行‘ls -lh /txtlink’
        /**
         * 捕获控制台输出对象stdout,输出捕获数据
         */
        ls_txt.stdout.on('data', function (data) {
            console.info('ls -lh /txtlink stdout:');
            console.log('stdout: ' + data);
            console.info();
        });
        /**
         * 绑定系统‘error’事件
         */
        ls_txt.on('error', function (code) {
            console.log('child process error with code ' + code);
            console.info();
        });
        /**
         * 绑定系统‘close’事件
         */
        ls_txt.on('close', function (code) {
            console.log('child process closed with code ' + code);
            console.info();
        });
    });
console.info();