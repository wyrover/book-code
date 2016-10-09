/**
 * Created by king on 15-4-24.
 *
 * ch08.fs_realpathSync.js
 */
console.info("------   fs realpathSync()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var cache = {'/king':'/home/king'}; // TODO: 定义cache
var relpath = 'txt/realpathSync.txt';   // TODO: 定义相对路径
var resolvedPath = fs.realpathSync(relpath, cache); // TODO: 相对路径转绝对路径（同步方法）
console.info('txt/realpathSync.txt realpath is : ');
console.info(resolvedPath); // TODO: 打印输出绝对路径
console.info();