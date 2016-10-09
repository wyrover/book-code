/**
 * Created by king on 15-6-16.
 *
 * ch12.http_request_get.js
 */
console.info("------   http - client get   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
/**
 * 发送 HTTP GET 请求
 */
http.get("http://localhost:6868/wechat?signature=12345678&echostr=78787878&timestamp=168",
    /**
     * GET 回调函数
     * @param res
     */
    function(res) {
        console.log('STATUS: ' + res.statusCode);   // TODO: 打印输出Status_Codes响应状态码
        console.info();
        res.setEncoding('utf8');
        /**
         * 监听"data"事件处理函数
         */
        res.on('data', function (chunk) {
            console.log('BODY: ' + chunk);  // TODO: 打印输出服务器回写内容
            console.info();
        });
        console.info();
}).on('error',
    /**
     * “error”事件回调函数
     * @param e
     */
    function(e) {
        console.log("Got error: " + e.message); // TODO: 打印输出“error”信息
        console.info();
});