/**
 * Created by king on 15-6-10.
 *
 * ch12.http_response_statuscodes.js
 */
console.info("------   http STSTUS_CODES   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
console.log("Now start HTTP server...");
console.info();
/**
 * 调用http.createServer()函数方法创建服务器
 */
http.createServer(function(req, res) {
    var status = req.url.substr(1);     // TODO: 获取url查询字段
    var status_codes = status.substring(12); // TODO: 获取HTTP.STATUS_CODES
    /**
     * 判断http.STATUS_CODES响应状态码集合是否有效
     */
    if(!http.STATUS_CODES[status_codes]) {
        status_codes = '404';
    }
    /**
     * 通过res.writeHeader()函数方法写HTTP文件头
     */
    res.writeHeader(status_codes, {'Content-Type':'text/plain'});
    /**
     * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
     */
    res.end(http.STATUS_CODES[status_codes]);
}).listen(6868);    // TODO: 监听6868端口号