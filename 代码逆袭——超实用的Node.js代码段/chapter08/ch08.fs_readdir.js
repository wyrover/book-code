/**
 * Created by king on 15-4-28.
 *
 * ch08.fs_readdir.js
 */
console.info("------   fs readdir()   ------");
console.info();
var path = "txt";
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 读取文件目录（异步方式）
 */
fs.readdir(path, function (err, files) {
    if (err) {
        // throw err;
        console.info('err: ' + err);
        console.info();
    } else {
        console.info('read directory txt: ');
        console.info(files);    // TODO: 打印输出文件目录
        console.log('fs.readdir() Done.');
        console.info();
    }
});
console.info();