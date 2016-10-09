/**
 * Created by king on 15-4-21.
 *
 * ch08.fs_statSync.js
 */
console.info("------   fs statSync()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var statSync = fs.statSync('txt/statSync.txt'); // TODO: 获取文件信息(同步方法)
console.info('txt/statSync.txt file info: ');
console.info(statSync); // TODO: 打印输出文件信息
console.info();
console.info('fs.statSync() Done.');
console.info();