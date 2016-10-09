/**
 * Created by king on 15-4-17.
 *
 * ch08.fs_rename.js
 */
console.info("------   fs rename()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
console.time('fs-rename-asyn'); // TODO: 定义时间开始标记
/**
 * 文件重命名
 */
fs.rename('txt/helloworld-asyn.txt', 'txt/helloworld-asyn.txt', function (err) {
    console.time('time-10000x10000');   // TODO: 定义时间开始标记
    for (var i=0; i<10000; i++) {
        for (var j=0; j<10000; j++) {
            ;
        }
    }
    console.timeEnd('time-10000x10000');    // TODO: 定义时间结束标记
    if (err) throw err;
    console.log('renamed complete');
});
console.timeEnd('fs-rename-asyn');  // TODO: 定义时间结束标记
console.info();
var exec = require('child_process').exec;   // TODO: 引入child_process模块
var child_pre = exec('cat txt/helloworld-asyn.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/helloworld-asyn.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stderr
    });
console.info();
var child_suf = exec('cat txt/helloworld-asyn.txt',
    function (error, stdout, stderr) {
        console.info('cat txt/helloworld-asyn.txt stdout: ');
        console.log(stdout);   // TODO: 打印输出 stdout
        console.log(stderr);   // TODO: 打印输出 stdout
    });