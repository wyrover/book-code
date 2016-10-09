/**
 * Created by king on 15-6-5.
 *
 * ch11.stream_readable_pipe.js
 */
console.info("------   stream pipe write file   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 调用文件系统（fs）模块的createReadStream()方法创建可读（Readable）流
 */
var readable = fs.createReadStream('data/srcFile.txt');
/**
 * 调用文件系统（fs）模块的createWriteStream()方法创建可写（Writable）流
 */
var writable = fs.createWriteStream('data/destFile.txt');
/**
 * 所有来自readable的数据会被写入到'data/destFile.txt'文本文件中
 */
readable.pipe(writable);