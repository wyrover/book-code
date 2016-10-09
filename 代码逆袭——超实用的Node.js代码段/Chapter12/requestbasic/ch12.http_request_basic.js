/**
 * Created by king on 15-6-16.
 *
 * ch12.http_request_basic.js
 */
console.info("------   http - create basic client   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
/**
 * 定义服务器参数字段
 * @type {{hostname: string, port: number, path: string, method: string}}
 */
var options = {
    hostname: 'localhost',  // TODO: 定义服务器主机地址
    port: 6868,             // TODO: 定义服务器主机端口号
    path: '/',              // TODO: 定义服务器路径
    method: 'POST'          // TODO: 定义服务器访问方式i
};
/**
 * 通过http.request()函数方法
 * 由客户端向HTTP服务器发起请求
 */
var req = http.request(options, function(res) {
    console.log('STATUS: ' + res.statusCode);
    console.log('HEADERS: ' + JSON.stringify(res.headers));
    res.setEncoding('utf8');
    res.on('data', function (chunk) {
        console.log('BODY: ' + chunk);
    });
});
/**
 * 监听request对象的'error'事件
 */
req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
});
/**
 * write data to request body
 */
req.write('data\n');
/**
 * write end to request body
 */
req.end();