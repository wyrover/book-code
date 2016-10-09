/**
 * Created by king on 15-6-14.
 *
 * ch12.http_response_header.js
 */
console.info("------   http - create basic server   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
/**
 * 调用http.createServer()函数方法创建服务器
 */
http.createServer(function(req, res) {
    /**
     * 通过res.setHeader()函数方法设定HTTP文件头
     */
    res.setHeader("Content-Type", "text/html");
    res.setHeader("Set-Cookie", ["type=king", "language=javascript"]);
    /**
     * 通过res.getHeader()函数方法获取HTTP文件头
     */
    var content_Type = res.getHeader('Content-Type');
    console.info(content_Type);
    var set_cookie = res.getHeader('Set-Cookie');
    console.info(set_cookie);
    /**
     * 通过res.write()函数方法写页面内容
     */
    res.write('<h3>Node.js --- HTTP</h3>');
    res.write('<p>' + content_Type + '</p>');
    res.write('<p>' + set_cookie + '</p>');
    /**
     * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
     */
    res.end();
}).listen(6868);    // TODO: 监听6868端口号