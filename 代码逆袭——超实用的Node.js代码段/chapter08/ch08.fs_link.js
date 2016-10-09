/**
 * Created by king on 15-4-23.
 *
 * ch08.fs_link.js
 */
console.info("------   fs open()   ------");
console.info();
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/link.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/link.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
    });
var srcpath = "txt/link.txt";
var dstpath = "txtlink/link.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 链接文件（异步方式）
 */
fs.link(srcpath, dstpath, function (err) {
    if (err) {
        throw err;
    }
});
console.log('fs.link() Done.');
console.info();
var child_suf = exec('cat txtlink/link.txt',
    function (error, stdout, stderr) {
        console.info('cat txtlink/link.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
        fs.unlink(dstpath, function (err) {
            if (err) {
                throw err;
            }
        });
        console.log('fs.unlink() Done.');
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