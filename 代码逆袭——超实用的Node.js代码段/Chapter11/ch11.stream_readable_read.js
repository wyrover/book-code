/**
 * Created by king on 15-6-3.
 *
 * ch11.stream_readable_read.js
 */
console.info("------   stream read()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 调用文件系统（fs）模块的createReadStream()方法创建可读（Readable）流
 */
var rs = fs.createReadStream('data/read.txt');
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
    var chunk;
    while (null !== (chunk = rs.read())) {
        console.log('得到了 %d 字节的数据', chunk.length);
        console.info();
        console.log('读取到的数据: ', chunk);
        console.info();
    }
});
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