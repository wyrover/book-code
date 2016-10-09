/**
 * Created by king on 15-6-5.
 *
 * ch11.stream_readable_unpipe.js
 */
console.info("------   stream pipe write file   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 调用文件系统（fs）模块的createReadStream()方法创建可读（Readable）流
 */
var readable = fs.createReadStream('data/srcUnFile.txt');
/**
 * 调用文件系统（fs）模块的createWriteStream()方法创建可写（Writable）流
 */
var writable = fs.createWriteStream('data/destUnFile.txt');
/**
 * 所有来自readable的数据会被写入到'data/destUnFile.txt'文本文件中
 */
readable.pipe(writable);
/**
 * 通过setTimeout()方法设置1ms时间延迟
 * 并通过unpipe()方法解除pipe()方法设置的流
 * 最后自行关闭文件流
 */
setTimeout(function() {
    console.log('停止写入到 data/destUnFile.txt');
    console.info();
    readable.unpipe(writable);  // TODO: 解除设置的流
    console.log('自行关闭文件流');
    console.info();
    writable.end();     // TODO: 关闭文件流
}, 1);