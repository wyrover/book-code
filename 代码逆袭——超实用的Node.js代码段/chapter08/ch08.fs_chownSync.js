/**
 * Created by king on 15-4-21.
 *
 * ch08.fs_chownSync.js
 */
console.info("------   fs chownSync()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var statSync_pre = fs.statSync('txt/chownSync.txt'); // TODO: 获取文件信息(同步方法)
console.info('txt/chownSync.txt file info: ');
console.info(statSync_pre); // TODO: 打印输出文件信息
console.info();
console.info('fs.chownSync() Done.');
console.info();
fs.chownSync('txt/chownSync.txt', 1001, 1001); // TODO: 更改文件所有权(同步方法)
console.info('fs.chownSync() Done.');
console.info();
var statSync_suf = fs.statSync('txt/chownSync.txt'); // TODO: 获取文件信息(同步方法)
console.info('txt/chownSync.txt file info: ');
console.info(statSync_suf); // TODO: 打印输出文件信息
console.info();