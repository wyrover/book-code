/**
 * Created by king on 15-6-25.
 *
 * ch15.mongodb_conn_collection_insert.js
 */
console.info("------   mongodb connnection collection insert()   ------");
console.info();
var http = require("http"); // TODO: 引入HTTP模块
var mongodb = require('/usr/local/lib/node_modules/mongodb');   // TODO: 引入 mongodb 模块
console.log("Now start HTTP server on port 6868...");
console.info();
/**
 * 创建数据库服务器连接
 */
var server = new mongodb.Server(
    'localhost',    // TODO: 主机地址
    27017,          // TODO: 端口号
    {
        auto_reconnect: true    // TODO: 自动重连
    }
);
/**
 * 创建数据库连接
 */
var db = new mongodb.Db(
    'nodejs',   // TODO: 数据库名称
    server,     // TODO: 数据库服务器
    {
        safe: true
    }
);
/**
 * 测试数据库连接
 */
db.open(function(err, db){
    if(!err)
    {
        console.log('log - connect mongdb successfully!');
    } else {
        console.log('log - ' + err);
    }

});
/**
 * 创建HTTP服务器
 */
http.createServer(function (req, res) {
    res.writeHead(200, { "Content-Type" : "text/html;charset=utf8" });
    res.write("<h3>测试 Node.js - MongoDB 数据库操作!</h3><br/>");
    /**
     * 测试数据库连接
     */
    db.open(function(err, db) {
        if(!err) {
            res.write('<p>Connected MongoDB:nodejs successfully!</p>');
            /**
             * 连接数据集合
             */
            db.collection('userinfo', { safe: true }, function(errcollection, collection) {
                if(!errcollection) {
                    res.write('<p>Connected MongoDB:nodejs:userinfo successfully!</p>');
                    var insertCol = {userid: 'wang', pwd: '123456', username: 'wang'};
                    /**
                     * 插入数据集合
                     */
                    collection.insert(insertCol, {safe: true}, function(errinsert, result) {
                        res.write('<p>collection.insert() is: </p>');
                        res.write(JSON.stringify(result));
                        console.log(result);
                    });
                } else {
                    console.log('log - ' + err);
                }
            });
        } else {
            res.end('<p>Error Connected to MySQL!</p>');
            console.log('log - ' + err);
        }
        //res.end("<h5>------ end operations ------</h5><br/>");
    });
}).listen(6868);    // TODO: 监听6868端口号