/**
 * Created by king on 15-4-21.
 *
 * ch08.fs_fstat.js
 */
console.info("------   fs fstat()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
fs.open('txt/fstat.txt', 'r', function (err, fd) {
    if (err) {
        throw err;
    }
    /**
     * 获取文件信息(异步方法)
     */
    fs.fstat(fd, function (err, stats) {
        if (err) {
            throw err;
        } else {
            console.info('txt/fstat.txt file info: ');
            console.info(stats);    // TODO: 打印输出文件信息
            console.info();
            console.info('fs.fstat() Done.');
            console.info();
        }
    });
});
console.info();