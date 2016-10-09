/**
 * Created by king on 15-5-19.
 *
 * ch10.net_client.js
 */
console.info("------   net ServerClient()   ------");
console.info();
var net = require("net");   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 9696;            // TODO: 定义端口号
/**
 * 使用net.connect()函数方法创建一个TCP客户端实例
 */
var client = net.connect(PORT, HOST, function() {
    console.log('client connected...');
    console.info();
});
/**
 * 为TCP客户端实例添加一个"data"事件处理函数
 */
client.on('data', function(data) {
    console.info('client on data...');
    console.info();
});
/**
 * 为TCP客户端实例添加一个"end"事件处理函数
 */
client.on('end', function() {
    console.log('client disconnected');
    console.info();
});