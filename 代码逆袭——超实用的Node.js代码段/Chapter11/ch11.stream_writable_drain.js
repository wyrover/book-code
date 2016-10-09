/**
 * Created by king on 15-6-10.
 *
 * ch11.stream_writable_drain.js
 */
console.info("------   stream writable file   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 调用文件系统（fs）模块的createReadStream()方法创建可读（Readable）流
 */
var rs = fs.createReadStream("data/srcDrain.mp3");
/**
 * 调用文件系统（fs）模块的createWriteStream()方法创建可写（Writable）流
 */
var ws = fs.createWriteStream("data/destDrain.mp3");
/**
 * 调用readable.setEncoding()函数方法设置编码
 */
rs.setEncoding('utf8');
/**
 * 流（Stream）模块 --- 'data'事件处理函数
 */
rs.on('data', function(chunk) {
    console.log('data event emitted.');
    console.info();
    /**
     * 通过ws.write()函数方法返回值判断
     * 数据是已经写入目标
     * 还是仅仅写入缓存中
     */
    if(ws.write(chunk) === false) {
        console.log('数据写入缓存...');
        console.info();
        rs.pause();     // TODO: 暂停可读（Readable）流
    }
});
ws.on('drain', function() {
    console.log('drain event emitted.');
    console.info();
    rs.resume();        // TODO: 恢复可读（Readable）流
});
/**
 * 流（Stream）模块 --- 'end'事件处理函数
 */
rs.on('end', function() {
    console.log('end event emitted.');
    console.info();
    ws.end();
});