/**
 * Created by king on 15-4-22.
 *
 * ch08.fs_fchmod.js
 */
console.info("------   fs fchmod()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var spawn = require('child_process').spawn; // TODO: 引入child_process模块
var ls_txt_pre = spawn('ls', ['-lh', 'txt/fchmod.txt']);  // TODO: 定义命令行‘ls -lh /txt’
/**
 * 捕获控制台输出对象stdout,输出捕获数据
 */
ls_txt_pre.stdout.on('data', function (data) {
    console.log('stdout: ' + data);
    /**
     * 打开文件（异步方式）
     */
    fs.open('txt/fchmod.txt', 'a', function (err, fd) {
        if(err) {
            throw err;
        } else {
            /**
             * 更改文件权限(异步方法)
             */
            fs.fchmod(fd, 777, function (err) {
                if(err) {
                    throw err;
                } else {
                    console.info('fchmod() Done.');
                    console.info();
                }
            });
            var ls_txt_suf = spawn('ls', ['-lh', 'txt/fchmod.txt']);  // TODO: 定义命令行‘ls -lh /txt’
            /**
             * 捕获控制台输出对象stdout,输出捕获数据
             */
            ls_txt_suf.stdout.on('data', function (data) {
                console.log('stdout: ' + data);
            });
            /**
             * 绑定系统‘error’事件
             */
            ls_txt_suf.on('error', function (code) {
                console.log('child process error with code ' + code);
                console.info();
            });
            /**
             * 绑定系统‘close’事件
             */
            ls_txt_suf.on('close', function (code) {
                console.log('child process closed with code ' + code);
                console.info();
            });
        }
        /**
         * 关闭文件（异步方法）
         */
        fs.close(fd, function () {
            console.log('fs.close() Done.');
            console.info();
        });
    });
});
/**
 * 绑定系统‘error’事件
 */
ls_txt_pre.on('error', function (code) {
    console.log('child process error with code ' + code);
    console.info();
});
/**
 * 绑定系统‘close’事件
 */
ls_txt_pre.on('close', function (code) {
    console.log('child process closed with code ' + code);
    console.info();
});