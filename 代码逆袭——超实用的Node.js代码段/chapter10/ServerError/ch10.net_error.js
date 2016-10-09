/**
 * Created by king on 15-5-21.
 *
 * ch10.net_error.js
 */
console.info("------   net error   ------");
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
 * 监听"error"事件
 */
server.on('error', function(err) {
    console.log('Error occurred:', err.message);
});
/**
 * 调用server.listen()监听服务器端口
 */
server.listen(PORT, HOST);