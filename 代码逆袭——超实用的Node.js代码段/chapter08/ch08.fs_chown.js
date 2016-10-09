/**
 * Created by king on 15-4-21.
 *
 * ch08.fs_chown.js
 */
console.info("------   fs chown()   ------");
console.info();
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 获取文件信息(异步方法)
 */
fs.stat('txt/chown.txt', function (err, stats) {
    if (err) {
        throw err;
    } else {
        console.info('txt/chown.txt file info: ');
        console.info(stats);    // TODO: 打印输出文件信息
        console.info();
        console.info('fs.stat() Done.');
        console.info();
        /**
         * 更改文件所有权(异步方法)
         */
        fs.chown('txt/chown.txt', 1001, 1001, function(err) {
            if(err){
                console.log(err);
            }else{
                console.log("fs.chown() Done");
                console.info();
                /**
                 * 获取文件信息(异步方法)
                 */
                fs.stat('txt/chown.txt', function (err, stats) {
                    if (err) {
                        throw err;
                    } else {
                        console.info('txt/chown.txt file info: ');
                        console.info(stats);    // TODO: 打印输出文件信息
                        console.info();
                        console.info('fs.stat() Done.');
                        console.info();
                    }
                });
            }
        });
    }
});
console.info();