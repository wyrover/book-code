/**
 * Created by king on 15-6-18.
 *
 * ch12.http_client_ping.js
 */
console.info("------   http - client ping   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
console.log("Now start ping HTTP server...");
console.info();
/**
 * 定义查询字段
 */
var options = {
    host: 'localhost',
    port: 6868,
    path: '/'
};
/**
 * 自定义函数 ping_server()
 */
function ping_server() {
    /**
     * 发送 HTTP GET 请求
     */
    http.get(options, function(res) {
        if (res.statusCode  == 200) {
            console.log("The site is up!");
        }
        else {
            console.log("The site is down!");
        }
    }).on('error', function(e) {
        console.log("There was an error: " + e.message);
    });
}
/**
 * 通过setInterval()函数方法设定时间间隔
 */
setInterval(ping_server, 1000);