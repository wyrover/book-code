/**
 * Created by king on 15-5-20.
 *
 * ch10.net_listening_server.js
 */
console.info("------   net listening()   ------");
console.info();
var net = require('net');   // TODO: 引入网络(Net)模块
var HOST = '127.0.0.1';     // TODO: 定义服务器地址
var PORT = 7788;            // TODO: 定义端口号
var server = require('net').createServer();
server.on('listening', function() {
    console.log('Server is listening on port', PORT);
});
server.on('connection', function(socket) {
    console.log('Server has a new connection');
    socket.end();
    server.close();
});
server.on('close', function() {
    console.log('Server is now closed');
});
server.on('error', function(err) {
    console.log('Error occurred:', err.message);
});
server.listen(PORT, HOST);