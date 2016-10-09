/**
 * Created by king on 15-6-14.
 *
 * ch12.http_createServer_redirectA.js
 */
console.info("------   http - create redirect server   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
console.log("Now start HTTP server on port 8686...");
console.info();
/**
 * 调用http.createServer()函数方法创建服务器
 */
http.createServer(function(req, res) {
    /**
     * 通过res.writeHeader()函数方法写HTTP文件头
     */
    res.writeHead(200, {'Content-type' : 'text/html'});
    /**
     * 通过res.write()函数方法写页面内容
     */
    res.write('<h3>Node.js --- HTTP</h3>');
    /**
     * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
     */
    res.end('<p>Create Redirect HTTP Server on Port 8686!</p>');
}).listen(8686);    // TODO: 监听8686端口号