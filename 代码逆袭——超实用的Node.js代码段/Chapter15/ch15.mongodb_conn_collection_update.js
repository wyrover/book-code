/**
 * Created by king on 15-6-25.
 *
 * ch15.mongodb_conn_collection_update.js
 */
console.info("------   mongodb connnection collection update()   ------");
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
                    /**
                     * 更新数据集合,方式一
                     */
                    collection.update({userid: "king"}, {$push: {email:'king@email.com'}}, function(errupdate, cols) {
                        res.write('<p>collection.update() is: </p>');
                        res.write(JSON.stringify(cols));
                        console.log(cols);
                    });
                    collection.update({userid: "super"}, {$push: {email:'super@email.com'}}, function(errupdate, cols) {
                        res.write('<p>collection.update() is: </p>');
                        res.write(JSON.stringify(cols));
                        console.log(cols);
                    });
                    /**
                     * 查询数据集合
                     */
                    collection.find().toArray(function(errorfind, cols) {
                        if(!errorfind) {
                            res.write('<p>collection.find() is: </p>');
                            res.write(JSON.stringify(cols));
                            console.log(cols);
                        }
                    });
                    /**
                     * 更新数据集合,方式二
                     */
                    collection.update({userid: "king"}, {$set: {username:'king-update'}}, function(errupdate, cols) {
                        res.write('<p>collection.update() is: </p>');
                        res.write(JSON.stringify(cols));
                        console.log(cols);
                    });
                    collection.update({userid: "super"}, {$set: {username:'super-update'}}, function(errupdate, cols) {
                        res.write('<p>collection.update() is: </p>');
                        res.write(JSON.stringify(cols));
                        console.log(cols);
                    });
                    /**
                     * 查询数据集合
                     */
                    collection.find().toArray(function(errorfind, cols) {
                        if(!errorfind) {
                            res.write('<p>collection.find() is: </p>');
                            res.write(JSON.stringify(cols));
                            console.log(cols);
                        }
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