/**
 * Created by king on 15-6-14.
 *
 * ch12.http_response_get.js
 */
console.info("------   http - server get   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
var url = require('url');       // TODO: 引入url模块
var qs = require('querystring');    // TODO: 引入querystring模块
/**
 * 调用http.createServer()函数方法创建服务器
 */
http.createServer(function(req, res) {
    /**
     * 通过res.writeHeader()函数方法写HTTP文件头
     */
    res.writeHead(200, {'Content-type' : 'text/plain'});
    /**
     * 通过url.parse()函数方法获取查询字段
     */
    var query = url.parse(req.url).query;
    console.info(query);
    console.info();
    /**
     * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
     */
    var qs_parse = qs.parse(query);
    console.info(qs_parse);
    console.info();
    res.end(JSON.stringify(qs_parse));
}).listen(6868);    // TODO: 监听6868端口号