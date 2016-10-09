/**
 * Created by king on 15-6-26.
 *
 * ch14.mysql_pool.js
 */
console.info("------   mysql pool()   ------");
console.info();
var http = require("http"); // TODO: 引入HTTP模块
var mysql = require('/usr/local/lib/node_modules/mysql');   // TODO: 引入mysql模块
console.log("Now start HTTP server on port 6868...");
console.info();
/**
 * 创建数据库连接池
 */
var pool = mysql.createPool({
    host: "localhost",  // TODO: 主机地址
    user: "root",       // TODO: 数据库用户名
    password: "root",   // TODO: 数据库密码
    database: "nodejs", // TODO: 数据库名称
    port: 3306          // TODO: 端口号
});
/**
 * 创建HTTP服务器
 */
http.createServer(function (req, res) {
    /**
     * 获取数据库连接池
     */
    pool.getConnection(function (err, conn) {
        if(err) {
            console.log("POOL ==> " + err);
            console.log();
        }
        /**
         * 定义SQL查询语句
         * @type {string}
         */
        var selectSQL = 'select * from userinfo';
        /**
         * 执行数据查询操作
         */
        conn.query(selectSQL, function(err, rows) {
            if(err) {
                console.log(err);
                console.log();
            }
            console.log("SELECT ==> ");
            for (var i in rows) {
                console.log(rows[i]);
            }
            conn.release(); // TODO: 释放数据库连接
            res.writeHead(200, { "Content-Type": "text/html;charset=utf8" });
            res.write("<h3>测试 Node.js - MySQL 数据库连接池操作!</h3><br/>");
            res.end(JSON.stringify(rows));
        });
    });
}).listen(6868);    // TODO: 监听6868端口号