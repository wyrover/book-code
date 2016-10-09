/**
 * Created by king on 15-4-29.
 *
 * ch08.fs_writeBuf.js
 */
console.info("------   fs write()   ------");
console.info();
var file_path = "txt/write.txt";
var fs = require('fs'); // TODO: 引入文件系统模块;
/**
 * 打开文件（异步方式）
 */
fs.open(file_path, 'w', function (err, fd) {
    if(err){
        throw err;
    } else {
        console.log('fs.open() done.');
        console.info();
        var file_contents_pre = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
        console.info('read txt/write.txt contents: ');
        console.info(file_contents_pre);    // TODO: 打印输出文件内容
        console.log('fs.readFileSync() Done.');
        console.info();
        console.info('write to txt/write.txt : ');
        var buf = new Buffer('fs.write(fd, buffer, offset, length[, position], callback)\n');
        /**
         * 写文件（异步方法）
         */
        fs.write(fd, buf, 0, buf.length, 0, function (err, bytesWritten, buffer) {
            if (err) {
                throw err;
            } else {
                console.log('写入数据的字节数: ' + bytesWritten);
                console.log('写入的数据: ' + buffer);
                /**
                 * 读文件（异步方法）
                 */
                fs.readFile(file_path, 'utf-8', function (err, data) {
                    if (err) {
                        throw err;
                    } else {
                        console.info('read txt/write.txt contents: ');
                        console.info(data);    // TODO: 打印输出文件内容
                        console.log('fs.readFile() Done.');
                        console.info();
                    }
                });
            }
            console.log('fs.write() Done.');
            console.info();
        });
        /**
         * 关闭文件（异步方式）
         */
        fs.close(fd, function (err) {
            if (err) {
                throw err;
            } else {
                console.log('fs.close() done.');
                console.info();
            }
        });
    }
});