/**
 * Created by king on 15-4-24.
 *
 * ch08.fs_realpath.js
 */
console.info("------   fs realpath()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
var cache = {'/king':'/home/king'}; // TODO: 定义cache
var relpath = 'txt/realpath.txt';   // TODO: 定义相对路径
/**
 * 相对路径转绝对路径（异步方法）
 */
fs.realpath(relpath, cache, function (err, resolvedPath) {
    if (err) {
        throw err;
    }
    console.info('txt/realpath.txt realpath is : ');
    console.info(resolvedPath); // TODO: 打印输出绝对路径
});
console.info();