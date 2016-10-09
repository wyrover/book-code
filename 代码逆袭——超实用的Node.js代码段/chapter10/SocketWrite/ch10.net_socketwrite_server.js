/**
 * Created by king on 15-5-20.
 *
 * ch10.net_socketwrite_server.js
 */
console.info("------   net socketwrite   ------");
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
    /**
     * 为socket实例添加一个"data"事件处理函数
     */
    socket.on('data', function(data) {
        /**
         * 打印输出由客户端发来的数据字节长度
         */
        console.info('socket.bytesRead is ' + socket.bytesRead);
        console.info();
        /**
         * 打印输出由客户端发来的数据
         */
        console.info('DATA ' + socket.remoteAddress + ' : "' + data + '"');
        console.info();
        /**
         * 回发该数据,客户端将收到来自服务端的数据
         */
        socket.write('Server write : "' + data + '"');
        /**
         * 打印输出回发到客户端的数据字节长度
         */
        console.info('socket.bytesWritten is ' + socket.bytesWritten);
        console.info();
    });
    /**
     * 关闭服务器
     */
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
 * 调用server.listen()监听服务器端口
 */
server.listen(PORT, HOST);