/**
 * Created by king on 15-4-29.
 *
 * ch08.fs_writeFile.js
 */
console.info("------   fs writeFile()   ------");
console.info();
var file_path = "txt/writeFile.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 判断文件是否存在（异步方式）
 */
fs.exists(file_path, function (exists) {
    if (exists) {
        /**
         * 读文件（异步方法）
         */
        fs.readFile(file_path, 'utf-8', function (err, data) {
            if (err) {
                throw err;
                console.info();
            } else {
                console.info('read txt/writeFile.txt contents: ');
                console.info(data);    // TODO: 打印输出文件内容
                console.log('fs.readFile() Done.');
                console.info();
                console.info('write to txt/writeFile.txt : ');
                /**
                 * 写文件（异步方法）
                 */
                fs.writeFile(file_path, 'fs.writeFile()', function (err) {
                    if (err) {
                        throw err;
                    } else {
                        /**
                         * 读文件（异步方法）
                         */
                        fs.readFile(file_path, 'utf-8', function (err, data) {
                            if (err) {
                                throw err;
                                console.info();
                            } else {
                                console.info('read txt/writeFile.txt contents: ');
                                console.info(data);    // TODO: 打印输出文件内容
                                console.log('fs.readFile() Done.');
                                console.info();
                            }
                        });
                    }
                    console.log('fs.writeFile() Done.');
                    console.info();
                });
            }
        });
    } else {
        console.info('err: ' + err);
        console.info();
    }
});
console.info();