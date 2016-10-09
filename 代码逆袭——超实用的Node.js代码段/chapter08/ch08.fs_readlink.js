/**
 * Created by king on 15-4-23.
 *
 * ch08.fs_readlink.js
 */
console.info("------   fs readlink()   ------");
console.info();
var srcpath = "txt/symlink.txt";
var linkpath = "txtlink/symlink.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 读取链接文件（异步方式）
 */
fs.readlink(srcpath, function (err, linkString) {
    if (err) {
        // throw err;
        console.info('err: ' + err);
        /**
         * 读取链接文件（异步方式）
         */
        fs.readlink(linkpath, function (err, linkString) {
            if (err) {
                throw err;
            } else {
                console.info('read linkpath: ' + linkString);
                console.log('fs.readlink() Done.');
            }
        });
        console.info();
    } else {
        console.info('read srcpath: ' + linkString);
        console.log('fs.readlink() Done.');
    }
});
console.info();