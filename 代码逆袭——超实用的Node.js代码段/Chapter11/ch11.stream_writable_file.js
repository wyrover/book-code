/**
 * Created by king on 15-6-9.
 *
 * ch11.stream_writable_file.js
 */
console.info("------   stream writable file   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 调用文件系统（fs）createWriteStream()方法创建可写（Writable）流
 */
var ws = fs.createWriteStream('data/writable.txt');
ws.write('stream - ');      // TODO: 写数据
ws.write('writable - ');    // TODO: 写数据
ws.write('file - ');        // TODO: 写数据
/**
 * 通过setTimeout()方法设置1000ms时间延迟
 * 自行关闭文件流
 */
setTimeout(function () {
    ws.end('end.\n');   // TODO: 关闭文件流
}, 1000);