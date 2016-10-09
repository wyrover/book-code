/**
 * Created by king on 15-4-28.
 *
 * ch08.fs_readdirSync.js
 */
console.info("------   fs readdirSync()   ------");
console.info();
var path = "txt";
var fs = require('fs'); // TODO: 引入文件系统模块
var files = fs.readdirSync(path);  // TODO: 读取文件目录（同步方式）
console.info('read directory txt: ');
console.info(files);    // TODO: 打印输出文件目录
console.log('fs.readdirSync() Done.');
console.info();