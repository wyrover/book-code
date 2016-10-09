/**
 * Created by king on 15-4-21.
 *
 * ch08.fs_stat.js
 */
console.info("------   fs stat()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 获取文件信息(异步方法)
 */
fs.stat('txt/stat.txt', function (err, stats) {
    if (err) {
        throw err;
    } else {
        console.info('txt/stat.txt file info: ');
        console.info(stats);    // TODO: 打印输出文件信息
        console.info();
        console.info('fs.stat() Done.');
        console.info();
    }
});
console.info();