/**
 * Created by king on 15-6-3.
 *
 * ch11.stream_readable_pause.js
 */
console.info("------   stream pause() & resume()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 调用文件系统（fs）模块的createReadStream()方法创建可读（Readable）流
 */
var rs = fs.createReadStream('data/pause.txt');
/**
 * 调用readable.setEncoding()函数方法设置编码
 */
rs.setEncoding('utf8');
/**
 * 流（Stream）模块 --- 'readable'事件处理函数
 */
rs.on('readable', function() {
    console.log('readable event emitted.');
    console.info();
});
/**
 * 流（Stream）模块 --- 'data'事件处理函数
 */
rs.on('data', function (chunk) {
    console.info("读取'data/pause.txt'文本文件的内容...");
    console.info();
    console.log('读取到了 %d 字节的数据.', chunk.length);
    console.info();
    rs.pause();
    console.log('接下来3秒内不会有数据...');
    console.info();
    setTimeout(function() {
        console.log('现在数据会再次开始流动...');
        console.info();
        rs.resume();
        console.log('读取到的数据: \n', chunk);
        console.info();
    }, 3000);
});
/**
 * 流（Stream）模块 --- 'error'事件处理函数
 */
rs.on('error', function () {
    console.log('error event emitted.');
    console.info();
});
/**
 * 流（Stream）模块 --- 'end'事件处理函数
 */
rs.on('end', function () {
    console.log('end event emitted.');
    console.info();
});
/**
 * 流（Stream）模块 --- 'close'事件处理函数
 */
rs.on('close', function () {
    console.log('close event emitted.');
    console.info();
});