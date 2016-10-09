/**
 * Created by king on 15-4-21.
 *
 * ch08.fs_fstatSync.js
 */
console.info("------   fs fstatSync()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var fd = fs.openSync('txt/fstatSync.txt', 'r'); // TODO: 打开文件（同步方式）
var fstatSync = fs.fstatSync(fd); // TODO: 获取文件信息(同步方法)
console.info('txt/fstatSync.txt file info: ');
console.info(fstatSync); // TODO: 打印输出文件信息
console.info();
console.info('fs.fstatSync() Done.');
fs.closeSync(fd);
console.info();