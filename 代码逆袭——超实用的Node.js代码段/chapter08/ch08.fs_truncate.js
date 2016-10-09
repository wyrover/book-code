/**
 * Created by king on 15-4-20.
 *
 * ch08.fs_truncate.js
 */
console.info("------   fs truncate()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/truncate.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/truncate.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log('length: ' + stdout.length); // TODO: 输出数据长度
        console.log(stderr);   // TODO: 打印输出 stderr
        console.info();
        /**
         * 修改文件内容长度(异步方法)
         */
        fs.truncate('txt/truncate.txt', 6, function (err) {
            if (err) {
                throw err;
            } else {
                console.info('fs.truncate() Done.');
                console.info();
            }
        });
        var child_suf = exec('cat txt/truncate.txt',
            function (error, stdout, stderr) {
                console.info('cat txt/truncate.txt stdout: ');
                console.log(stdout);   // TODO: 打印输出 stdout
                console.log('length: ' + stdout.length); // TODO: 输出数据长度
                console.log(stderr);   // TODO: 打印输出 stdout
                console.info();
            });
    });
console.info();