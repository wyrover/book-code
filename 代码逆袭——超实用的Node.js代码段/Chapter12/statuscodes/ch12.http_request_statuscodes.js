/**
 * Created by king on 15-6-16.
 *
 * ch12.http_request_statuscodes.js
 */
console.info("------   http - create basic client   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
var querystring = require('querystring');   // TODO: 引入 Query String 模块
/**
 * 定义响应状态码数组
 */
var status_codes = new Array();
status_codes[0] = "201";
status_codes[1] = "202";
status_codes[2] = "203";
status_codes[3] = "401";
status_codes[4] = "402";
/**
 * 定义查询字段
 */
var postData = new Array();
for(var n=0; n<5; n++) {
    postData[n] = querystring.stringify({
        statuscodes: status_codes[n]
    });
}
/**
 * 模拟HTTP客户端向HTTP服务器端连续发送request请求
 */
for(var i=0; i<5; i++) {
/**
 * 定义服务器参数字段
 * @type {{hostname: string, port: number, path: string, method: string, headers: {Content-Type: string, Content-Length: *}}}
 */
var options = {
    hostname: 'localhost',
    port: 6868,
    path: '/' + postData[i],
    method: 'POST',
    headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
        'Content-Length': postData.length
    }
};
/**
 * 通过http.request()函数方法
 * 由客户端向HTTP服务器发起请求
 */
var req = http.request(options, function(res) {
    console.log('STATUS_CODES: ' + res.statusCode);
    console.log('HEADERS: ' + JSON.stringify(res.headers));
    console.info();
    res.setEncoding('utf8');
    res.on('data', function (chunk) {
        console.log('BODY: ' + chunk);
        console.info();
    });
});
/**
 * 监听request对象的'error'事件
 */
req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
    console.info();
});
/**
 * write data to request body
 */
req.write("\n");
/**
 * write end to request body
 */
req.end();
}