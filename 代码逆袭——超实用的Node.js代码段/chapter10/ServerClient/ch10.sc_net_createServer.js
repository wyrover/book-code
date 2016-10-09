/**
 * Created by king on 15-5-18.
 *
 * ch10.sc_net_createServer.js
 */
console.info("------   net ServerClient()   ------");
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
     * 回调函数获得一个参数,该参数自动关联一个socket对象
     * 在每一个“connection”事件中,该回调函数接收到的socket对象是唯一的
     */
    console.log('CONNECTED: ' + sock.remoteAddress + ':' + sock.remotePort);
    console.info();
    /**
     * 为socket实例添加一个"data"事件处理函数
     */
    sock.on('data', function(data) {
        /**
         * 打印输出由客户端发来的消息
         */
        console.info('DATA ' + sock.remoteAddress + ' : "' + data + '"');
        console.info();
        /**
         * 回发该数据,客户端将收到来自服务端的数据
         */
        sock.write('Server write : "' + data + '"');
    });
    /**
     * 为socket实例添加一个"close"事件处理函数
     */
    sock.on('close', function(data) {
        console.log('CLOSED: ' + sock.remoteAddress + ' ' + sock.remotePort);
        console.info();
    });

}).listen(PORT, HOST);