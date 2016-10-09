/**
 * Created by king on 15-6-14.
 *
 * ch12.http_response_writeheader.js
 */
console.info("------   http - server write header   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
console.log("Now start HTTP server...");
console.info();
/**
 * 调用http.createServer()函数方法创建服务器
 */
http.createServer(function(req, res) {
    /**
     * 通过res.writeHead()函数方法写HTTP文件头
     */
    var body = 'write header';
    res.writeHead(200, {
        'Content-Length': body.length,
        'Content-Type': 'text/plain'
    });
    /**
     * 通过res.write()函数方法写页面内容
     */
    res.write("Node.js");
    res.write("HTTP");
    /**
     * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
     */
    res.end();
}).listen(6868);    // TODO: 监听6868端口号