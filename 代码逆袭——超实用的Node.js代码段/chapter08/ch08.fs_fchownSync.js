/**
 * Created by king on 15-4-21.
 *
 * ch08.fs_fchownSync.js
 */
console.info("------   fs fchownSync()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var fd = fs.openSync('txt/fchownSync.txt', 'r'); // TODO: 打开文件（同步方式）
var fstatSync_pre = fs.fstatSync(fd); // TODO: 获取文件信息(同步方法)
console.info('txt/fchownSync.txt file info: ');
console.info(fstatSync_pre); // TODO: 打印输出文件信息
console.info();
console.info('fs.fstatSync() Done.');
fs.fchownSync(fd, 1001, 1001); // TODO: 更改文件所有权(同步方法)
console.info('fs.fchownSync() Done.');
console.info();
var fstatSync_suf = fs.fstatSync(fd); // TODO: 获取文件信息(同步方法)
console.info('txt/fchownSync.txt file info: ');
console.info(fstatSync_suf); // TODO: 打印输出文件信息
console.info();
console.info('fs.fstatSync() Done.');
fs.closeSync(fd);
console.info();