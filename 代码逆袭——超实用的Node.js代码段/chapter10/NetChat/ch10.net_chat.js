/**
 * Created by king on 15-5-28.
 *
 * ch10.net_chat.js
 */
console.info("------   net chat room   ------");
console.info();
var net = require('net');   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 6688;            // TODO: 定义端口号
var clientList = [];        // TODO: 定义客户端列表
console.info('Now create Chat Server...');
console.info();
/**
 * 创建TCP服务器
 */
var server = net.createServer();
/**
 * 监听"connection"事件
 */
server.on('connection', function(client) {
    clientList.push(client);    // TODO: socket入栈
    client.name = client.remoteAddress + ':' + client.remotePort;   // TODO: 保存客户端地址和端口
    broadcast('hi,' + client.name + ' join in!\r\n', client);   // TODO: 调用broadcast()方法
    client.write('hi,' + client.name + '!\r\n');    // TODO: 向客户端发信息
    /**
     * 监听“data”事件
     */
    client.on('data', function(data) {
        broadcast(client.name + ' say:' + data + '\r\n', client);   // TODO: 调用broadcast()方法
    });
    /**
     * 监听“end”事件
     */
    client.on('end', function() {
        broadcast('hi,' + client.name + ' quit!\r\n', client);  // TODO: 调用broadcast()方法
        clientList.splice(clientList.indexOf(client), 1);   // TODO: 删除客户端socket
    });
})
/**
 * broadcast function - 向全部客户端发通知消息
 * @param message
 * @param client
 */
function broadcast(message, client) {
    var cleanup = [];
    for(var i=0, len=clientList.length; i<len; i++) {
        if(client !== clientList[i]) {
            if(clientList[i].writable) {
                clientList[i].write(message);   // TODO: 向客户端发送信息
            } else {
                cleanup.push(clientList[i]);
                clientList[i].destroy();    // TODO: 清除客户端socket
            }
        }
    }
    for(var i=0, len=cleanup.length; i<len; i++) {
        clientList.splice(clientList.indexOf(cleanup[i]), 1);   // TODO: 删除客户端socket
    }
}
/**
 * listen host and port
 */
server.listen(PORT, HOST);