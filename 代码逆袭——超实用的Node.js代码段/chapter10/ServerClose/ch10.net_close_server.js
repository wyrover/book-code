/**
 * Created by king on 15-5-21.
 *
 * ch10.net_close_server.js
 */
console.info("------   net close()   ------");
console.info();
var net = require('net');   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 8877;            // TODO: 定义端口号
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
});
/**
 * 监听"connection"事件
 */
server.on('connection', function(socket) {
    console.log('Server has a new connection');
    console.info();
    server.close();     // TODO: 调用server.close()方法
});
/**
 * 监听"close"事件
 */
server.on('close', function() {
    console.log('Server is now closed');
    console.info();
});
/**
 * 调用server.listen()监听服务器端口
 */
server.listen(PORT, HOST);