/**
 * Created by king on 15-6-14.
 *
 * ch12.http_createServer_pathname.js
 */
console.info("------   http - server pathname   ------");
console.info();
var http = require('http');     // TODO: 引入http模块
var url = require('url');     // TODO: 引入url模块
console.log("Now start HTTP server on port 6868...");
console.info();
/**
 * 调用http.createServer()函数方法创建服务器
 */
http.createServer(function(req, res) {
    /**
     * 获取url.pathname路径
     * @type {pathname|*|req.pathname|parseTests.pathname|parseTestsWithQueryString.pathname|formatTests.pathname}
     */
    var pathname = url.parse(req.url).pathname;
    /**
     * Responding to multi type of request
     */
    if (pathname === '/') {
        /**
         * 通过res.writeHead()函数方法写HTTP文件头
         */
        res.writeHead(200, {
            'Content-Type': 'text/plain'
        });
        /**
         * 通过res.write()函数方法写页面内容
         */
        res.write('Node.js --- HTTP\n');
        /**
         * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
         */
        res.end('Home Page\n')
    } else if (pathname === '/about') {
        /**
         * 通过res.writeHead()函数方法写HTTP文件头
         */
        res.writeHead(200, {
            'Content-Type': 'text/plain'
        });
        /**
         * 通过res.write()函数方法写页面内容
         */
        res.write('Node.js --- HTTP\n');
        /**
         * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
         */
        res.end('About Us\n')
    } else if (pathname === '/redirect') {
        /**
         * 通过res.writeHead()函数方法写HTTP文件头
         */
        res.writeHead(301, {
            'Location': '/'
        });
        /**
         * 通过res.write()函数方法写页面内容
         */
        res.write('Node.js --- HTTP\n');
        /**
         * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
         */
        res.end();
    } else {
        /**
         * 通过res.writeHead()函数方法写HTTP文件头
         */
        res.writeHead(404, {
            'Content-Type': 'text/plain'
        });
        /**
         * 通过res.write()函数方法写页面内容
         */
        res.write('Node.js --- HTTP\n');
        /**
         * 通过res.end()函数方法发送响应状态码,并通知服务器消息完成
         */
        res.end('Page not found\n')
    }
}).listen(6868);    // TODO: 监听6868端口号