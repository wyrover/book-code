/**
 * Created by king on 15-5-20.
 *
 * ch10.net_server_address.js
 */
console.info("------   net server.address()   ------");
console.info();
var net = require('net');   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 7878;            // TODO: 定义端口号
/**
 * 创建 TCP服务器
 */
var server = net.createServer();
/**
 * 监听"listening"事件
 */
server.on('listening', function() {
    console.log('Server is listening on port', PORT);
    console.info();
    var addr = server.address();
    console.info("opened server on ");
    console.info("%j", addr);
    console.info();
    server.close();
});
/**
 * 监听"close"事件
 */
server.on('close', function() {
    console.log('Server is now closed');
    console.info();
});
/**
 * 监听"error"事件
 */
server.on('error', function(err) {
    console.log('Error occurred:', err.message);
    console.info();
});
/**
 * 调用server.listen()监听服务器端口
 */
server.listen(PORT, HOST);