/**
 * Created by king on 15-5-19.
 *
 * ch10.net_connection.js
 */
console.info("------   net connection()   ------");
console.info();
var net = require("net");   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 9696;            // TODO: 定义端口号
/**
 * 创建 TCP服务器
 */
var server = net.createServer();
/**
 * 监听端口和主机
 */
server.listen(PORT, HOST);
console.log('Server listening on ' + server.address());
console.info();
/**
 * 通过显式调用"connection"事件建立TCP连接
 */
server.on('connection', function(sock) {
    console.log('CONNECTED: ' + sock.remoteAddress + ':' + sock.remotePort);
});