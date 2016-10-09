/**
 * Created by king on 15-5-18.
 *
 * ch10.udp_server.js
 */
console.info("------   UDP ServerClient   ------");
console.info();
var dgram = require('dgram');   // TODO: 引入网络(UDP)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 12345;       // TODO: 定义端口号
/**
 * 创建UDP服务器
 */
console.info('Now create UDO Server...');
console.info();
/**
 * 使用dgram.createSocket()函数方法创建一个UPD服务器
 */
var server = dgram.createSocket('udp4');
/**
 * 为UDP服务器添加一个"listening"事件处理函数
 */
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
    console.info();
});
/**
 * 为UDP服务器添加一个"message"事件处理函数
 */
server.on('message', function (message, remote) {
    console.log("UDP Server received from " + remote.address + ':' + remote.port);
    console.log(" - " + message);
    console.info();
    server.close();
});
/**
 * 为UDP服务器添加一个"error"事件处理函数
 */
server.on('error', function(err) {
    console.log("server error:\n" + err.stack);
    server.close();
});
/**
 * 为UDP服务器添加一个"close"事件处理函数
 */
server.on('close', function() {
    console.log('server closed');
    console.info();
});
/**
 * 为UDP服务器绑定主机和端口
 */
server.bind(PORT, HOST);