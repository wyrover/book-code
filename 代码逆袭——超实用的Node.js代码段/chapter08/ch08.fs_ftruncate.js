/**
 * Created by king on 15-4-20.
 *
 * ch08.fs_ftruncate.js
 */
console.info("------   fs ftruncate()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/ftruncate.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/ftruncate.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
        /**
         * 打开文件（异步方式）
         */
        fs.open('txt/ftruncate.txt', 'a', function (err, fd) {
            if (err) {
                throw err;
            }
            /**
             * 截取文件（异步方式）
             */
            fs.ftruncate(fd, 10, function (err) {
                if (err) {
                    throw err;
                }
                console.log("文件描述符: " + fd);    // TODO: 打印输出文件描述符
                /**
                 * 关闭文件（异步方式）
                 */
                fs.close(fd, function () {
                    console.log('fs.ftruncate() Done');
                    console.info();
                });
            });
        });
        var child_suf = exec('cat txt/ftruncate.txt',
            function (error, stdout, stderr) {
                console.info('cat txt/ftruncate.txt stdout: ');
                console.log(stdout);   // TODO: 打印输出 stdout
                console.log(stderr);   // TODO: 打印输出 stdout
            });
    });
console.info();