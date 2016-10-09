/**
 * Created by king on 15-5-20.
 *
 * ch10.net_listening.js
 */
console.info("------   net listening()   ------");
console.info();
var net = require('net');   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 7788;            // TODO: 定义端口号
/**
 * 创建TCP服务器
 */
var server = net.createServer();
/**
 * 监听"listening"事件
 */
server.on('listening', function() {
    console.log('Server is listening on port ', PORT);
    console.info();
});
/**
 * 调用server.listen()监听服务器端口
 */
server.listen(PORT, HOST);