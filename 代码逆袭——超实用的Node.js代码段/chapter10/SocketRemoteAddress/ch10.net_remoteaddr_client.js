/**
 * Created by king on 15-5-20.
 *
 * ch10.net_remoteaddr_client.js
 */
console.info("------   net socket.remoteaddress()   ------");
console.info();
var net = require("net");   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 6677;            // TODO: 定义端口号
/**
 * 使用net.connect()函数方法创建一个TCP客户端实例
 */
var client = net.connect(PORT, HOST, function() {
    console.log('client connected');
    console.info();
    client.end();
});
/**
 * 为TCP客户端实例添加一个"end"事件处理函数
 */
client.on('end', function() {
    console.log('client disconnected');
    console.info();
});