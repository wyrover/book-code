/**
 * Created by king on 15-5-19.
 *
 * ch10.net_createServer.js
 */
console.info("------   net createServer()   ------");
console.info();
var net = require('net');   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 9696;            // TODO: 定义端口号
/**
 * 使用net.ServerClient()函数方法创建一个TCP服务器实例
 * 同时调用listen()函数方法开始监听指定端口
 * 传入net.ServerClient()的回调函数将作为”connection“事件的处理函数
 */
console.info('Now create Server...');
console.info();
net.createServer(function(sock) {
    /**
     * 打印输出服务器监听提示信息
     */
    console.log('Server listening on ' + HOST +':'+ PORT);  // TODO: 服务器已经建立连接
    console.info();
    /**
     * 为socket实例添加一个"data"事件处理函数
     */
    sock.on('data', function(data) {
       console.log('socket on data...');
    });
    /**
     * 为socket实例添加一个"close"事件处理函数
     */
    sock.on('close', function(data) {
        console.log('socket on close...');
    });

}).listen(PORT, HOST);